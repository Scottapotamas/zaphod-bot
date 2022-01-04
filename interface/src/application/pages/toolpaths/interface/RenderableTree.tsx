import { TreeNodeInfo, IconName, Tree, Card, Icon } from '@blueprintjs/core'
import { Renderable } from '../optimiser/import'
import create, { GetState, SetState, StateSelector } from 'zustand'
import React, { useCallback, useState } from 'react'
import {
  StoreApiWithSubscribeWithSelector,
  subscribeWithSelector,
} from 'zustand/middleware'
import produce, { Draft } from 'immer'
import { IconNames } from '@blueprintjs/icons'
import { setSetting } from './state'

export enum NodeTypes {
  CAMERA = 'camera',
  CAMERA_ALIGNMENT = 'camera-alignment',
  GPENCIL = 'gpencil',
  GPENCIL_LAYER = 'gpencil-layer',
  PARTICLES = 'particles',
  PARTICLE_SYSTEM = 'particle-system',
}

type NodePath = (string | number)[]

export interface NodeInfo {
  type: NodeTypes
  hidden: boolean
  parentID?: string | number
}

export interface TreeStore {
  tree: TreeNodeInfo<NodeInfo>[]
  selectedItemID: string | null // Store the ID of the item that's currently selected
}

const initialState: TreeStore = {
  tree: [],
  selectedItemID: null,
}

export const useTreeStore = create<
  TreeStore,
  SetState<TreeStore>,
  GetState<TreeStore>,
  StoreApiWithSubscribeWithSelector<TreeStore>
>(subscribeWithSelector(() => initialState))

export const resetTreeStore = () => useTreeStore.setState(initialState)

export const mutateTree = (recipe: (draft: Draft<TreeStore>) => void) => {
  useTreeStore.setState(state => {
    return produce(state, recipe)
  })
}

interface SecondaryLabelProps {
  id: string | number
}

function SecondaryLabelFactory(props: SecondaryLabelProps) {
  const thisIcon = useTreeStore(state => {
    const ourNode = findNodeWithID(state.tree, props.id)
    const hidden = ourNode?.nodeData!.hidden

    return hidden ? IconNames.EYE_OFF : IconNames.EYE_OPEN
  })

  const onClickHandler = useCallback(() => {
    // Create our list of override keys
    const overrideKeysToMerge: {
      [key: string]: boolean
    } = {}

    // Update the Tree UI
    mutateTree(state => {
      // Find this node's state, otherwise we have a toggle with depth
      const ourNode = findNodeWithID(state.tree, props.id)
      const hidden = ourNode?.nodeData!.hidden

      if (hidden) {
        // if this node is currently hidden, then we're about to reveal it,
        // in which case all parents must also be revealed
        forNodeAndParentsRecursive(state.tree, props.id, node => {
          node.nodeData!.hidden = false
          overrideKeysToMerge[node.id] = false
        })
      }

      // Hide or show everything including this node and below
      forNodeWithIDAndChildren(state.tree, props.id, node => {
        node.nodeData!.hidden = !hidden
        // Update our list of override keys
        overrideKeysToMerge[node.id] = !hidden
      })
    })

    // Update the settings
    setSetting(state => {
      for (const key of Object.keys(overrideKeysToMerge)) {
        const hidden = overrideKeysToMerge[key]

        state.settings.hiddenObjects[key] = hidden
      }
    })
  }, [props.id])

  return (
    <>
      <Icon icon={thisIcon} onClick={onClickHandler} />
    </>
  )
}

/**
 * Generate the initial scene tree data structure, which will be used as the state of the Tree
 * component.
 */
export function renderablesToSceneTree(renderables: Renderable[]) {
  const nameSet: Set<string> = new Set()
  const nodes: TreeNodeInfo<NodeInfo>[] = []

  for (const renderable of renderables) {
    const node = renderable.getObjectTree()

    // If it already has this key, continue
    if (nameSet.has(node.id as string)) {
      continue
    }

    nameSet.add(node.id as string)

    nodes.push(node)
  }

  // Annotate every Node with a view / hide button
  forEachNode(nodes, node => {
    node.secondaryLabel = <SecondaryLabelFactory id={node.id} />
  })

  // Annotate every node with its parent ID
  forEachNodeWithParentInformation(nodes, (node, parentID) => {
    if (parentID) {
      node.nodeData!.parentID = parentID
    }
  })

  return nodes
}

// TODO: When Blueprint fixes the Tree node type, remove this any cast
// For referencing a node by its path
function forNodeAtPath(
  nodes: TreeNodeInfo<NodeInfo>[],
  path: NodePath,
  callback: (node: TreeNodeInfo<NodeInfo>) => void,
) {
  callback(Tree.nodeFromPath(path as any, nodes))
}

// Walk the entire node tree
function forEachNode(
  nodes: TreeNodeInfo<NodeInfo>[] | undefined,
  callback: (node: TreeNodeInfo<NodeInfo>) => void,
) {
  if (nodes === undefined) {
    return
  }

  for (const node of nodes) {
    callback(node)
    forEachNode(node.childNodes, callback)
  }
}

// Walk the entire node tree providing parent information down the tree
function forEachNodeWithParentInformation(
  nodes: TreeNodeInfo<NodeInfo>[] | undefined,
  callback: (node: TreeNodeInfo<NodeInfo>, parentID?: string | number) => void,
  parentID?: string | number, //
) {
  if (nodes === undefined) {
    return
  }

  for (const node of nodes) {
    callback(node, parentID)
    forEachNodeWithParentInformation(node.childNodes, callback, node.id)
  }
}

// For referencing a node by the ID
function findNodeWithID(
  nodes: TreeNodeInfo<NodeInfo>[] | undefined,
  id: string | number,
): TreeNodeInfo<NodeInfo> | null {
  if (nodes === undefined) {
    return null
  }

  for (const node of nodes) {
    if (node.id === id) {
      // Found it on this level
      return node
    } else {
      // Search children
      const potential = findNodeWithID(node.childNodes, id)
      // A child had it
      if (potential) return potential
    }
  }
  // Couldn't find it
  return null
}

// For referencing a node by the ID, and all its children
function forNodeWithIDAndChildren(
  nodes: TreeNodeInfo<NodeInfo>[] | undefined,
  id: string | number,
  callback: (node: TreeNodeInfo<NodeInfo>) => void,
) {
  if (nodes === undefined) {
    return
  }

  for (const node of nodes) {
    if (node.id === id) {
      // Found it, do the thing
      callback(node)

      // Do it to all children
      forEachNode(node.childNodes, callback)
    }

    // Search children
    forNodeWithIDAndChildren(node.childNodes, id, callback)
  }
}

// Referemce a node and each of its parents recursively
function forNodeAndParentsRecursive(
  nodes: TreeNodeInfo<NodeInfo>[] | undefined,
  id: string | number,
  callback: (node: TreeNodeInfo<NodeInfo>) => void,
) {
  if (nodes === undefined) {
    return
  }

  let parentID: string | number | undefined = id

  while (parentID) {
    const node = findNodeWithID(nodes, parentID)

    if (node) {
      callback(node)
      parentID = node.nodeData?.parentID
    } else {
      break
    }
  }
}

export function RenderableTree() {
  const tree = useTreeStore(state => state.tree)

  const handleNodeClick = useCallback(
    (
      node: TreeNodeInfo<NodeInfo>,
      nodePath: (string | number)[],
      e: React.MouseEvent<HTMLElement>,
    ) => {
      mutateTree(state => {
        // Deselect everything first
        forEachNode(state.tree, node => {
          node.isSelected = false
        })

        // Select our node
        forNodeAtPath(state.tree, nodePath, node => {
          node.isSelected = true
        })
      })
    },
    [],
  )

  const handleNodeCollapse = useCallback(
    (
      node: TreeNodeInfo<NodeInfo>,
      nodePath: (string | number)[],
      e: React.MouseEvent<HTMLElement>,
    ) => {
      mutateTree(state => {
        forNodeAtPath(state.tree, nodePath, node => {
          node.isExpanded = false
        })
      })
    },
    [],
  )

  const handleNodeExpand = useCallback(
    (
      node: TreeNodeInfo<NodeInfo>,
      nodePath: (string | number)[],
      e: React.MouseEvent<HTMLElement>,
    ) => {
      mutateTree(state => {
        forNodeAtPath(state.tree, nodePath, node => {
          node.isExpanded = true
        })
      })
    },
    [],
  )

  return (
    <Card>
      <Tree
        contents={tree}
        onNodeClick={handleNodeClick}
        onNodeCollapse={handleNodeCollapse}
        onNodeExpand={handleNodeExpand}
      />
    </Card>
  )
}
