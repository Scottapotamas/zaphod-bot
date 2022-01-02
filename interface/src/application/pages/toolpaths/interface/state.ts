import create, { GetState, SetState, StateSelector } from 'zustand'
import {
  StoreApiWithSubscribeWithSelector,
  subscribeWithSelector,
} from 'zustand/middleware'
import produce, { Draft } from 'immer'
import { Settings } from '../optimiser/settings'
import { Vector3 } from 'three'

import { blankMaterial } from '../optimiser/material'
import { useCallback } from 'react'
import { OrderingCache, Toolpath } from '../optimiser/passes'
import { Renderable } from '../optimiser/import'
import { Movement } from '../optimiser/movements'
import shallow from 'zustand/shallow'

const defaultSettings: Settings = {
  objectSettings: {
    gpencil: {
      breakUpStrokes: true,
    },
    particles: {
      drawInVelocityOrientation: false,
      stopDelay: 1,
    },
  },

  // Do object level overrides here. Particle subsystems can be `object -> subsystem name`
  objectOverrides: {},

  objectToggles: {},

  transitionMaterial: blankMaterial,
  materialOverrides: {
    globalOveride: null,
    objectMaterialOverrides: {},
  },

  optimisation: {
    startingPoint: [0, 0, 0],
    endingPoint: [0, 0, 0],
    maxSpeed: 100,
    transitionMaxSpeed: 100,
    waitAtStartDuration: 1000,
  },
}

interface Store {
  folder: string | null
  settings: Settings
  sceneMinFrame: number
  sceneMaxFrame: number
  sceneTotalFrames: number
  selectedMinFrame: number
  selectedMaxFrame: number
  currentlyRenderingFrame: number
  priorityFrame: number
  currentlyOptimising: boolean

  // The currently viewed frame
  viewportFrame: number

  /**
   * The version number of the currently viewed frame's toolpath.
   *
   * Reset on new frame, and on update
   */
  viewportFrameVersion: number

  // The OrderingCache per frame, used to reconstruct movements on the UI side
  movementOrdering: {
    [frameNumber: number]: OrderingCache
  }
  // An array of all renderables through the entire scene stored here, for a list of object names, etc.
  allRenderables: Renderable[]
  // Renderables by frame are stored here, no use case yet.
  renderablesByFrame: {
    [frame: number]: Renderable[]
  }
  // Latest toolpaths for sending to hardware
  toolpaths: {
    [frameNumber: number]: Toolpath
  }
  // As the optimiser orders movements, the UI copy of the movements will be stored here
  orderedMovementsByFrame: {
    [frameNumber: number]: Movement[]
  }
}

const initialState: Store = {
  folder: null,
  settings: defaultSettings,
  sceneMinFrame: 1,
  sceneMaxFrame: 1,
  sceneTotalFrames: 0,
  selectedMinFrame: 1,
  selectedMaxFrame: 1,
  currentlyRenderingFrame: 1,
  viewportFrame: 1,
  viewportFrameVersion: 0,
  priorityFrame: 1,
  currentlyOptimising: false,
  toolpaths: {},
  movementOrdering: {},
  allRenderables: [],
  renderablesByFrame: {},
  orderedMovementsByFrame: {},
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

export const useViewportFrameToolpath = () => {
  const setting = useStore(state => state.toolpaths[state.viewportFrame])

  return setting ?? null
}
