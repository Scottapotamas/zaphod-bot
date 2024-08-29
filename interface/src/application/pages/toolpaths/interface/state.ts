import create, { GetState, SetState, StateSelector } from 'zustand'
import { StoreApiWithSubscribeWithSelector, subscribeWithSelector } from 'zustand/middleware'
import produce from 'immer'
import type { Settings } from '../optimiser/settings'

import { importMaterial, MaterialJSON } from '../optimiser/material'
import type { Material } from '../optimiser/materials/Base'
import { useCallback } from 'react'
import type { Renderable } from '../optimiser/import'
import { deserialiseTour, type Movement, type SerialisedTour } from '../optimiser/movements'
import type { OrbitControls as OrbitControlsImpl } from 'three-stdlib'
import type { PerspectiveCamera as PerspectiveCameraImpl } from 'three'
import type { WritableDraft } from 'immer/dist/internal'
import type { TreeNodeInfo } from '@blueprintjs/core'
import type { NodeID, NodeInfo } from './RenderableTree'
import type { FRAME_STATE, ToolpathGenerator } from './../optimiser/main'
import type { GPencilOutputType } from '../optimiser/gpencil'
import type { GNodesMeshOutputType } from '../optimiser/gnodes_mesh'
import { GNodesCurvesOutputType } from '../optimiser/gnodes_curves'
import { Camera } from '../optimiser/camera'
import { DenseMovements } from '../optimiser/movement_utilities'
import { sparseToDense } from '../optimiser/passes'
import { CancellationToken, Deferred } from '@electricui/async-utilities'
import { renderablesToMovements } from '../optimiser/files'

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
    gnodesCurves: {
      outputType: 1 as GNodesCurvesOutputType.GROUP_PER_SPLINE,
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
    interLineTransitionAngle: 50, // degrees
    interLineTransitionMaxGap: 2, // mm
    interLineTransitionShaveDistance: 2,
    interLineTransitionLessAngle: 2, // degrees
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

  // One-by-one movement hover
  stepThrough: boolean

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

  // The camera Renderable per frame, might not be defined
  perFrameCamera: {
    [frameNumber: number]: Camera | undefined
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
    stepThrough: false,
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
  perFrameCamera: {},
  estimatedDurationByFrame: {},
  frameOptimisationState: {},

  camera: null,
  orbitControls: null,
  matchCameraAnimation: false,

  movementQueueUI: 0,
  lightQueueUI: 0,

  cameraOverrideDuration: 0,
}

export const useStore = create<Store, SetState<Store>, GetState<Store>, StoreApiWithSubscribeWithSelector<Store>>(
  subscribeWithSelector(() => initialState),
)

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
  return useSetting(state => state.estimatedDurationByFrame[state.viewportFrame] ?? 0)
}

export function useViewportFrameState() {
  return useSetting(state => state.frameOptimisationState[state.viewportFrame] ?? 2) // UNOPTIMISED = 2, avoiding circular dependency
}

/**
 * Does this force you to
 */
class ExternalSingleton {
  /** The current viewport frame number */
  private viewportFrameNumber = 0

  /** The latest serialisedTour per frame */
  private serialisedMovementsCache: Map<number, SerialisedTour> = new Map()

  /** The renderables per frame, injested once on import */
  private renderablesByFrame: Map<number, Renderable[]> = new Map()

  private progressUpdateWaitsPerFrame: Map<number, { deferred: Deferred<void>; cancellationToken: CancellationToken }> =
    new Map()

  private frameSubscriptions: Set<() => void> = new Set()

  public onViewportFrameVersionChange = async (
    frameNumber: number,
    settings: Settings,
    cancellationToken: CancellationToken,
  ) => {
    this.viewportFrameNumber = frameNumber
    // const start = performance.now()

    // Recalculate dense movements for this frame
    const movements = await this.getDenseMovementsForFrame(this.viewportFrameNumber, settings, cancellationToken)

    // const end = performance.now()

    // console.log(
    //   `took ${Math.round((end - start) * 10) / 10}ms to process dense movements for frame ${this.viewportFrameNumber}`,
    // )

    this.thisFrameCachedDenseMovements = movements

    // Then notify everything of the changes
    this.notify()
  }

  public onProgressUpdate = (frameNumber: number, serialisedTour: SerialisedTour) => {
    this.serialisedMovementsCache.set(frameNumber, serialisedTour)

    // don't async call notify, that will be handled by the
    // incrementViewportFrameVersion call in the onProgress callback in the optimiser

    const waits = this.progressUpdateWaitsPerFrame.get(frameNumber)
    if (waits) {
      waits.deferred.resolve()
      this.progressUpdateWaitsPerFrame.delete(frameNumber)
    }
  }

  public onIngest = (renderablesByFrame: Map<number, Renderable[]>, frameNumberToBegin: number) => {
    this.viewportFrameNumber = frameNumberToBegin

    // Wipe the cache
    this.serialisedMovementsCache.clear()

    // Store the renderables per frame
    this.renderablesByFrame.clear()
    this.renderablesByFrame = renderablesByFrame

    // Reject every pending wait
    this.progressUpdateWaitsPerFrame.forEach(({ deferred, cancellationToken }) => {
      deferred.reject(cancellationToken.token)
    })

    this.progressUpdateWaitsPerFrame.clear()

    // Trigger a render of 'nothing'
    this.notify()
  }

  public getVisibleRenderableViaOriginalMaterialJSONWithObjectID = (objectID: string) => {
    const renderables = this.renderablesByFrame.get(this.viewportFrameNumber)
    
    if (!renderables) {
      return null
    }

    const renderable = renderables.find(renderable => renderable.matchesObjectID(objectID))

    return renderable ?? null
  }

  private waitForFrameProgressUpdate = (frameNumber: number, cancellationToken: CancellationToken) => {
    const waits = this.progressUpdateWaitsPerFrame.get(frameNumber)
    if (waits) {
      return waits.deferred.promise
    }

    const deferred = new Deferred<void>()
    this.progressUpdateWaitsPerFrame.set(frameNumber, { deferred, cancellationToken })

    return deferred.promise
  }

  public getDenseMovementsForFrame = async (
    frameNumber: number,
    settings: Settings,
    cancellationToken: CancellationToken,
  ): Promise<DenseMovements> => {
    const renderables = this.renderablesByFrame.get(frameNumber)

    if (!renderables) {
      // TODO: Wait for renderables to arrive?
      throw new Error(`No renderables for frame ${frameNumber}`)
    }

    let cachedOrder = this.serialisedMovementsCache.get(frameNumber)

    if (!cachedOrder) {
      console.log(`Couldn't find cached ordering for frame ${frameNumber}`)

      await this.waitForFrameProgressUpdate(frameNumber, cancellationToken)
      cancellationToken.haltIfCancelled()

      cachedOrder = this.serialisedMovementsCache.get(frameNumber)!

      if (!cachedOrder) {
        throw new Error(`cachedOrder wasn't there after explicit wait`)
      }
    }

    const sparseBag = await renderablesToMovements(renderables, settings, cancellationToken)

    const deserialised = deserialiseTour(sparseBag, cachedOrder)

    return sparseToDense(deserialised, settings)
  }

  private thisFrameCachedDenseMovements: DenseMovements | null = null

  /**
   * Synchronously get the dense movements this frame.
   *
   * Returns null if not ready.
   */
  public getDenseMovementsThisFrame = (): DenseMovements | null => {
    if (!this.thisFrameCachedDenseMovements) {
      return null
    }

    return this.thisFrameCachedDenseMovements
  }

  public getNumberOfMovements = () => {
    if (this.thisFrameCachedDenseMovements) {
      return this.thisFrameCachedDenseMovements.length
    }

    return 0
  }

  private notify = () => {
    this.frameSubscriptions.forEach(cb => cb())
  }

  /**
   * Subscribe to any changes, very coarse.
   */
  public subscribe(cb: () => void): () => void {
    this.frameSubscriptions.add(cb)

    return () => {
      this.frameSubscriptions.delete(cb)
    }
  }
}

export const singleton = new ExternalSingleton()