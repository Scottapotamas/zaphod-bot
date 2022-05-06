import create, { GetState, SetState, StateSelector } from 'zustand'
import {
  StoreApiWithSubscribeWithSelector,
  subscribeWithSelector,
} from 'zustand/middleware'
import produce, { Draft } from 'immer'
import { Settings } from '../optimiser/settings'
import { Quaternion, Vector3 } from 'three'

import { importMaterial, MaterialJSON } from '../optimiser/material'
import { useCallback } from 'react'
import { Renderable } from '../optimiser/import'
import {
  GLOBAL_OVERRIDE_OBJECT_ID,
  Movement,
  SerialisedTour,
} from '../optimiser/movements'
import shallow from 'zustand/shallow'
import { OrbitControls as OrbitControlsImpl } from 'three-stdlib'
import { PerspectiveCamera as PerspectiveCameraImpl } from 'three'
import { ObjectNameTree } from '../optimiser/files'
import { WritableDraft } from 'immer/dist/internal'
import { TreeNodeInfo } from '@blueprintjs/core'
import { NodeID, NodeInfo } from './RenderableTree'
import type { FRAME_STATE, ToolpathGenerator } from './../optimiser/main'
import { GPencilOutputType } from '../optimiser/gpencil'

export const defaultSettings: Settings = {
  objectSettings: {
    gpencil: {
      outputType: GPencilOutputType.LINE_GROUP,
      simplificationTolerance: 0,
    },
    particles: {
      drawInVelocityOrientation: false,
      preWait: 5,
      onDuration: 5,
      postWait: 5,
    },
    light: {
      preWait: 5,
      onDuration: 5,
      postWait: 5,
      hideIfBlack: true,
    },
    camera: {
      drawAlignmentHelpers: false,
      drawRulers: false,
      drawColorCalibrationChart: false,
    },
  },

  // Do object level overrides here. Particle subsystems can be `object -> subsystem name`
  objectOverrides: {},

  skippedObjects: {},

  optimisation: {
    maxSpeed: 150,
    transitionMaxSpeed: 150,
    waitAtStartDuration: 1000,
    interLineTransitionAngle: 50,
    interLineTransitionShaveDistance: 5,
    smoothInterlineTransitions: false,
    lineRunUp: 0.25,
    disableShapedTransitions: false,
    transitionSize: 0.04,
  },

  // Tools for debugging
  lightFadeOffset: 7,
}

/**
 * Settings that affect the visualisation but don't require a toolpath regeneration
 *
 * Increment the viewportFrameVersion when modifying these
 */
export interface VisualisationSettings {
  // Whether to draw tags with the movement's ordering nearby
  annotateDrawOrder: boolean

  // The curve detail level for splines.
  curveSegments: number

  // Do object level material overrides here.
  // Transition materials are overriden with the 'transition' key
  objectMaterialOverrides: {
    [objectID: string]: MaterialJSON
  }

  // Objects that are rendered, but hidden from view
  hiddenObjects: {
    [objectID: string]: boolean
  }

  // The virtualised timeline within a frame
  previewProgress: boolean
  frameProgress: number
}

export function getMaterialOverride(
  visualisationSettings: VisualisationSettings,
  providedMaterial: MaterialJSON,
  objectID: string,
) {
  let mat = providedMaterial

  // If there's an override key for this object ID, replace the material
  if (
    visualisationSettings.objectMaterialOverrides[GLOBAL_OVERRIDE_OBJECT_ID]
  ) {
    mat =
      visualisationSettings.objectMaterialOverrides[GLOBAL_OVERRIDE_OBJECT_ID]
  }

  // If there's an override key for this object ID, replace the material
  if (visualisationSettings.objectMaterialOverrides[objectID]) {
    mat = visualisationSettings.objectMaterialOverrides[objectID]
  }

  // See if we've imported it before
  // const key = JSON.stringify(mat)

  // if (materialCache.has(key)) {
  //   return materialCache.get(key)!
  // }

  // const material = importMaterial(mat)
  // materialCache.set(key, material)

  return importMaterial(mat)
}

export interface Store {
  folder: string | null
  settings: Settings
  sceneMinFrame: number
  sceneMaxFrame: number
  sceneTotalFrames: number
  selectedMinFrame: number
  selectedMaxFrame: number
  priorityFrame: number
  currentlyOptimising: boolean

  // The currently viewed frame
  viewportFrame: number

  // The currently rendering frame
  currentlyRenderingFrame: number

  /**
   * A reference to the current optimiser, for requesting a toolpath
   */
  persistentOptimiser: ToolpathGenerator | null

  /**
   * The version number of the currently viewed frame's toolpath.
   *
   * Reset on new frame, and on update, triggers a regeneration of the toolpath visualisation
   */
  viewportFrameVersion: number

  visualisationSettings: VisualisationSettings

  endEffector: {
    x: number
    y: number
    z: number
  }

  // Tree view store
  treeStore: {
    tree: TreeNodeInfo<NodeInfo>[]
    selectedItemID: NodeID | null // Store the ID of the item that's currently selected
    hoveredObjectIDs: NodeID[] // Store the IDs of all currently hovered objects
  }

  // Arbitrary data store from Empty objects in Blender
  arbitrary: {
    [frameNumber: number]: {
      [key: string]: any
    }
  }

  // The SerialisedTour per frame, used to reconstruct movements on the UI side
  movementOrdering: {
    [frameNumber: number]: SerialisedTour
  }
  // An array of all renderables through the entire scene stored here, for a list of object names, etc.
  allRenderables: Renderable[]
  // Renderables by frame are stored here
  renderablesByFrame: {
    [frame: number]: Renderable[]
  }
  // As the optimiser orders movements, the UI copy of the movements will be stored here
  unorderedMovementsByFrame: {
    [frameNumber: number]: Movement[]
  }
  // The current estimated duration of each frame, in milliseconds
  estimatedDurationByFrame: {
    [frameNumber: number]: number
  }
  // The state of the optimisation of each frame,
  frameOptimisationState: {
    [frameNumber: number]: FRAME_STATE
  }

  // Camera references
  camera: PerspectiveCameraImpl | null
  orbitControls: OrbitControlsImpl | null
  matchCameraAnimation: boolean

  // Queue depths for sending data to hardware
  movementQueueUI: number
  lightQueueUI: number

  // 0 means don't override, any number above is a time in milliseconds to trigger the camera for
  cameraOverrideDuration: number
}

const initialState: Store = {
  folder: null,
  settings: defaultSettings,
  sceneMinFrame: 1,
  sceneMaxFrame: 1,
  sceneTotalFrames: 0,
  selectedMinFrame: 1,
  selectedMaxFrame: 1,
  viewportFrame: 1,
  currentlyRenderingFrame: 0,
  persistentOptimiser: null,

  viewportFrameVersion: 0,
  visualisationSettings: {
    annotateDrawOrder: false,
    objectMaterialOverrides: {},
    curveSegments: 20, // 20 segments per curve by default
    hiddenObjects: {},
    previewProgress: false,
    frameProgress: 0,
  },

  endEffector: {
    x: 0,
    y: 0,
    z: 0,
  },

  arbitrary: {},

  treeStore: {
    tree: [],
    selectedItemID: null,
    hoveredObjectIDs: [],
  },

  priorityFrame: 1,
  currentlyOptimising: false,
  movementOrdering: {},
  allRenderables: [],
  renderablesByFrame: {},
  unorderedMovementsByFrame: {},
  estimatedDurationByFrame: {},
  frameOptimisationState: {},

  camera: null,
  orbitControls: null,
  matchCameraAnimation: true,

  movementQueueUI: 0,
  lightQueueUI: 0,

  cameraOverrideDuration: 0,
}

export const useStore = create<
  Store,
  SetState<Store>,
  GetState<Store>,
  StoreApiWithSubscribeWithSelector<Store>
>(subscribeWithSelector(() => initialState))

export const resetStore = () => useStore.setState(initialState)

export const setFolder = (folder: string | null) =>
  useStore.setState(state => {
    return produce(state, draft => {
      draft.folder = folder
    })
  })

export const useSetting = <U>(selector: StateSelector<Store, U>) => {
  const setting = useStore(useCallback(selector, [selector]))

  return setting
}

export const getSetting = <U>(selector: StateSelector<Store, U>) => {
  const settings = useStore.getState()
  const setting = selector(settings)

  return setting
}

export const setSetting = (recipe: (draft: Draft<Store>) => void) => {
  useStore.setState(state => {
    return produce(state, recipe)
  })
}

export const getCurrentSettings = () => useStore.getState().settings

export function incrementViewportFrameVersion(state: WritableDraft<Store>) {
  state.viewportFrameVersion += 1
  if (state.viewportFrameVersion === 255) {
    state.viewportFrameVersion = 0
  }
}

export function useViewportFrameDuration() {
  return useSetting(
    state => state.estimatedDurationByFrame[state.viewportFrame] ?? 0,
  )
}

export function useViewportFrameState() {
  return useSetting(
    state => state.frameOptimisationState[state.viewportFrame] ?? 2,
  ) // UNOPTIMISED = 2, avoiding circular dependency
}
