import create, { GetState, SetState, StateSelector } from 'zustand'
import {
  StoreApiWithSubscribeWithSelector,
  subscribeWithSelector,
} from 'zustand/middleware'
import produce from 'immer'
import type { Settings } from '../optimiser/settings'

import { importMaterial, MaterialJSON } from '../optimiser/material'
import type { Material } from '../optimiser/materials/Base'
import { useCallback } from 'react'
import type { Renderable } from '../optimiser/import'
import type { Movement, SerialisedTour } from '../optimiser/movements'
import type { OrbitControls as OrbitControlsImpl } from 'three-stdlib'
import type { PerspectiveCamera as PerspectiveCameraImpl } from 'three'
import type { WritableDraft } from 'immer/dist/internal'
import type { TreeNodeInfo } from '@blueprintjs/core'
import type { NodeID, NodeInfo } from './RenderableTree'
import type { FRAME_STATE, ToolpathGenerator } from './../optimiser/main'
import type { GPencilOutputType } from '../optimiser/gpencil'
import type { GNodesMeshOutputType } from '../optimiser/gnodes_mesh'

export const defaultSettings: Settings = {
  objectSettings: {
    gpencil: {
      outputType: 1 as GPencilOutputType.LINE_GROUP, // circular dep
      simplificationTolerance: 0.1,
    },
    particles: {
      drawInVelocityOrientation: false,
      preWait: 5,
      onDuration: 5,
      postWait: 5,
      hideIfOccluded: false,
      fullBrightnessBy: 0.1, // 10% ramp up time
      fullBrightnessUntil: 0.9, // 10% ramp down time
    },
    light: {
      preWait: 5,
      onDuration: 5,
      postWait: 5,
      hideIfBlack: true,
    },
    camera: {
      drawAlignmentHelpers: false,
      drawExtrinsicCalibrators: false,
      drawRulers: false,
      drawColorCalibrationChart: false,
    },
    effector: {
      preWait: 250,
      postWait: 250,
    },
    gnodesMesh: {
      outputType: 0 as GNodesMeshOutputType.LINES, // circular dep
    },
    gnodesVertices: {
      preWait: 5,
      onDuration: 5,
      postWait: 5,
    },
  },

  // Do object level overrides here. Particle subsystems can be `object -> subsystem name`
  objectOverrides: {},

  skippedObjects: {},

  optimisation: {
    maxSpeed: 300,
    rampToMaxSpeedDistance: 0,
    transitionSize: 0.1, // 1 / 3,
    waitAtStartDuration: 3000,
    interLineTransitionAngle: 50,
    interLineTransitionShaveDistance: 2,
    smoothInterlineTransitions: true,
    mergeColinearLines: true,
    lineRunUp: 0,
    disableShapedTransitions: false,
    passes: {
      nearestNeighbour: true,
      bruteForceSmall: true,
      twoOpt: true,
    },
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

  // Global brightness modifier, 0-1 float
  brightness: number
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

  targetPosition: {
    x: number
    y: number
    z: number
  }

  hardwareMode: string

  // Tree view store
  treeStore: {
    tree: TreeNodeInfo<NodeInfo>[]
    selectedItemID: NodeID | null // Store the ID of the item that's currently selected
    hoveredItems: (string | number)[] // Store the objectIDs or movementIndexes of all currently hovered items
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

  // Whether the settings need to be saved. Don't bother with a deep calculation, just mark it as dirty after any change and unmark on save / reload.
  settingsDirty: boolean
}

export const initialState: Store = {
  folder: null,

  // Serialised
  settings: defaultSettings,
  visualisationSettings: {
    annotateDrawOrder: false,
    objectMaterialOverrides: {},
    curveSegments: 20, // 20 segments per curve by default
    hiddenObjects: {},
    previewProgress: false,
    frameProgress: 0,
    brightness: 1,
  },

  settingsDirty: false,

  // Local
  sceneMinFrame: 1,
  sceneMaxFrame: 1,
  sceneTotalFrames: 0,
  selectedMinFrame: 1,
  selectedMaxFrame: 1,
  viewportFrame: 1,
  currentlyRenderingFrame: 0,
  persistentOptimiser: null,

  viewportFrameVersion: 0,

  endEffector: {
    x: 0,
    y: 0,
    z: 0,
  },

  targetPosition: {
    x: 0,
    y: 0,
    z: 0,
  },

  hardwareMode: '', // What is this?

  arbitrary: {},

  treeStore: {
    tree: [],
    selectedItemID: null,
    hoveredItems: [],
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
  matchCameraAnimation: false,

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

// Changes a setting and marks the settings object as dirty
export const setSetting = (recipe: (draft: WritableDraft<Store>) => void) => {
  useStore.setState(state => {
    return produce(state, (draft: WritableDraft<Store>) => {
      // Apply the actual changes
      recipe(draft)
      // Mark it as dirty
      draft.settingsDirty = true
    })
  })
}

// Just changes state, doesn't mutate the settings dirty flag
export const changeState = (recipe: (draft: WritableDraft<Store>) => void) => {
  useStore.setState(state => {
    return produce(state, recipe)
  })
}

export const markClean = () => {
  useStore.setState(state => {
    return produce(state, (draft: WritableDraft<Store>) => {
      // Mark it as clean
      draft.settingsDirty = false
    })
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
