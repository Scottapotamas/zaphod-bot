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

const defaultSettings = {
  objectSettings: {
    gpencil: {
      breakUpStrokes: true,
    },
    particles: {
      drawInVelocityOrientation: true,
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
    startingPoint: new Vector3(0, 0, 0),
    endingPoint: new Vector3(0, 0, 0),
    maxSpeed: 100,
    waitAtStartDuration: 1000,
  },
}

interface Store {
  folder: string | null
  settings: Settings
}

export const useStore = create<
  Store,
  SetState<Store>,
  GetState<Store>,
  StoreApiWithSubscribeWithSelector<Store>
>(
  subscribeWithSelector(() => ({
    folder: null,
    settings: defaultSettings,
  })),
)

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
