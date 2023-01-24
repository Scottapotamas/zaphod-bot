import { TreeNodeInfo, Tree, Icon } from '@blueprintjs/core'
import { Renderable } from '../optimiser/import'
import React, { useCallback } from 'react'
import { IconNames } from '@blueprintjs/icons'
import { changeState, incrementViewportFrameVersion, useStore } from './state'
import { GLOBAL_OVERRIDE_OBJECT_ID, TRANSITION_OBJECT_ID } from '../optimiser/movements'

export enum NodeTypes {
  GLOBAL = 'global',
  TRANSITION = 'transition',
  CAMERA = 'camera',
  CAMERA_ALIGNMENT = 'camera-alignment',
  CAMERA_RULER = 'camera-ruler',
  CAMERA_COLOR_LINES = 'camera-color-lines',
  GPENCIL = 'gpencil',
  GPENCIL_LAYER = 'gpencil-layer',
  PARTICLES = 'particles',
  PARTICLE_SYSTEM = 'particle-system',
  LIGHT = 'light',
  EMPTY = 'empty',
  EFFECTOR = 'effector',
  GNODES_MESH = 'gnodes-mesh',
  GNODES_VERTICES = 'gnodes-vertices',
}

export type NodeID = string | number
export type NodePath = NodeID[]

export interface NodeInfo {
  type: NodeTypes
  parentID?: string | number
}

interface SecondaryLabelProps {
  id: string | number
}

const marginRight = { marginRight: 3 }

function SecondaryLabelFactory(props: SecondaryLabelProps) {
  const skippedIcon = useStore(state => {
    const skipped = state.settings.skippedObjects[props.id]

    return skipped ? IconNames.CROSS : IconNames.TICK
  })

  const hiddenIcon = useStore(state => {
    const hidden = state.visualisationSettings.hiddenObjects[props.id]
    const skipped = state.settings.skippedObjects[props.id]

    return hidden || skipped ? IconNames.EYE_OFF : IconNames.EYE_OPEN
  })

  const materialOverrideIcon = useStore(state => {
    // If this node has a material override, use the tint icon
    if (state.visualisationSettings.objectMaterialOverrides[props.id]) {
      return IconNames.TINT
    }

    // If there's a global override in play, use the DOT
    if (state.visualisationSettings.objectMaterialOverrides[GLOBAL_OVERRIDE_OBJECT_ID]) {
      return IconNames.DOT
    }

    let hasOverride = false

    // If this node or any of its children have an override, display the icon
    forNodeWithIDAndChildren(state.treeStore.tree, props.id, node => {
      if (state.visualisationSettings.objectMaterialOverrides[node.id]) {
        hasOverride = true
      }
    })
    // If a child has an override, use the dot
    return hasOverride ? IconNames.DOT : null
  })

  const hideOnClickHandler = useCallback(() => {
    changeState(state => {
      const skipped = state.settings.skippedObjects[props.id]
      const hidden = state.visualisationSettings.hiddenObjects[props.id]

      const skippedOrHidden = skipped || hidden

      if (skippedOrHidden) {
        // if this node is currently hidden, then we're about to reveal it,
        // in which case all parents must also be revealed
        forNodeAndParentsRecursive(state.treeStore.tree, props.id, node => {
          state.visualisationSettings.hiddenObjects[node.id] = false
          // Also un-skip if we're about to unhide
          state.settings.skippedObjects[node.id] = false
        })
      }

      // Hide or show everything including this node and below
      forNodeWithIDAndChildren(state.treeStore.tree, props.id, node => {
        state.visualisationSettings.hiddenObjects[node.id] = !skippedOrHidden

        if (skipped) {
          // Unskip if we were skipped
          state.settings.skippedObjects[node.id] = false
        }
      })

      incrementViewportFrameVersion(state)
    })
  }, [props.id])

  const skipOnClickHandler = useCallback(() => {
    changeState(state => {
      const skipped = state.settings.skippedObjects[props.id]

      if (skipped) {
        // if this node is currently skipped, then we're about to reveal it,
        // in which case all parents must also be revealed
        forNodeAndParentsRecursive(state.treeStore.tree, props.id, node => {
          state.settings.skippedObjects[node.id] = false
        })
      }
      // Hide or show everything including this node and below
      forNodeWithIDAndChildren(state.treeStore.tree, props.id, node => {
        state.settings.skippedObjects[node.id] = !skipped
      })
    })
  }, [props.id])

  // const selectDeepestMaterialOverride: React.MouseEventHandler<HTMLElement> =
  //   useCallback(
  //     event => {
  //       changeState(state => {
  //         // Select the deepest node with a material override
  //         forNodeWithIDAndChildren(state.treeStore.tree, props.id, node => {
  //           if (state.visualisationSettings.objectMaterialOverrides[node.id]) {
  //             state.treeStore.selectedItemID = node.id
  //             console.log(`attempting to select ${node.id}`)
  //           }
  //         })
  //       })

  //       // Stop the event from reaching the actual row since we're handling the selection logic
  //       event.stopPropagation()
  //     },
  //     [props.id],
  //   )

  return (
    <>
      {materialOverrideIcon ? (
        <Icon
          icon={materialOverrideIcon}
          style={marginRight}
          // onClick={selectDeepestMaterialOverride}
        />
      ) : null}

      {props.id === TRANSITION_OBJECT_ID || props.id === GLOBAL_OVERRIDE_OBJECT_ID ? null : (
        <Icon icon={skippedIcon} onClick={skipOnClickHandler} style={marginRight} />
      )}

      {props.id === GLOBAL_OVERRIDE_OBJECT_ID ? null : <Icon icon={hiddenIcon} onClick={hideOnClickHandler} />}
    </>
  )
}

/**
 * Generate the initial scene tree data structure, which will be used as the state of the Tree
 * component.
 */
export function renderablesToSceneTree(renderables: Renderable[]) {
  const nameSet: Set<string> = new Set()
  const nodes: TreeNodeInfo<NodeInfo>[] = [
    {
      id: GLOBAL_OVERRIDE_OBJECT_ID,
      label: 'Global Override',
      icon: IconNames.GLOBE,
      nodeData: {
        type: NodeTypes.GLOBAL,
      },
    },
    {
      id: TRANSITION_OBJECT_ID,
      label: 'Transitions',
      icon: IconNames.RANDOM,
      nodeData: {
        type: NodeTypes.TRANSITION,
      },
    },
  ]

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
function forEachNode(nodes: TreeNodeInfo<NodeInfo>[] | undefined, callback: (node: TreeNodeInfo<NodeInfo>) => void) {
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
export function findNodeWithID(
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
  const tree = useStore(state => state.treeStore.tree)

  const handleNodeClick = useCallback(
    (_node: TreeNodeInfo<NodeInfo>, nodePath: NodeID[], e: React.MouseEvent<HTMLElement>) => {
      changeState(state => {
        // Deselect everything first
        forEachNode(state.treeStore.tree, node => {
          node.isSelected = false
        })

        // Select our node
        forNodeAtPath(state.treeStore.tree, nodePath, node => {
          node.isSelected = true

          // Update the selected item
          state.treeStore.selectedItemID = node.id
        })
      })
    },
    [],
  )

  const handleNodeCollapse = useCallback(
    (_node: TreeNodeInfo<NodeInfo>, nodePath: NodeID[], e: React.MouseEvent<HTMLElement>) => {
      changeState(state => {
        const node = findNodeWithID(state.treeStore.tree, _node.id)

        if (node) {
          node.isExpanded = false
        }

        // If any of the children were selected, unselect them
        forNodeWithIDAndChildren(state.treeStore.tree, _node.id, node => {
          if (node.id === _node.id) {
            // Don't unselect the collapsing node if it were selected
            return
          }

          if (state.treeStore.selectedItemID === node.id) {
            node.isSelected = false
            state.treeStore.selectedItemID = null
          }
        })
      })
    },
    [],
  )

  const handleNodeExpand = useCallback(
    (_node: TreeNodeInfo<NodeInfo>, nodePath: NodeID[], e: React.MouseEvent<HTMLElement>) => {
      changeState(state => {
        forNodeAtPath(state.treeStore.tree, nodePath, node => {
          node.isExpanded = true
        })
      })
    },
    [],
  )

  const handleNodeMouseEnter = useCallback(
    (_node: TreeNodeInfo<NodeInfo>, nodePath: NodeID[], e: React.MouseEvent<HTMLElement>) => {
      changeState(state => {
        const hoveredIDs: NodeID[] = []

        // If we're hovering over the global override, select everything that doesn't have a material override
        if (_node.id === GLOBAL_OVERRIDE_OBJECT_ID) {
          forEachNode(state.treeStore.tree, node => {
            if (!state.visualisationSettings.objectMaterialOverrides[node.id]) {
              hoveredIDs.push(node.id)
            }
          })

          state.treeStore.hoveredObjectIDs = hoveredIDs
          return
        }

        forNodeWithIDAndChildren(state.treeStore.tree, _node.id, node => {
          hoveredIDs.push(node.id)
        })

        state.treeStore.hoveredObjectIDs = hoveredIDs
      })
    },
    [],
  )

  const onNodeMouseLeave = useCallback(
    (node: TreeNodeInfo<NodeInfo>, nodePath: NodeID[], e: React.MouseEvent<HTMLElement>) => {
      changeState(state => {
        state.treeStore.hoveredObjectIDs = []
      })
    },
    [],
  )

  return (
    <Tree
      contents={tree}
      onNodeClick={handleNodeClick}
      onNodeCollapse={handleNodeCollapse}
      onNodeExpand={handleNodeExpand}
      onNodeMouseEnter={handleNodeMouseEnter}
      onNodeMouseLeave={onNodeMouseLeave}
    />
  )
}
