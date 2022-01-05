import { GPencilToMovementsSettings } from './gpencil'
import { ParticlesToMovementsSettings } from './particles'

export interface Settings {
  // Global object type settings
  objectSettings: {
    gpencil: GPencilToMovementsSettings
    particles: ParticlesToMovementsSettings
  }

  // Per-object overrides
  objectOverrides: {
    [id: string]:
      | Partial<GPencilToMovementsSettings>
      | Partial<ParticlesToMovementsSettings>
  }

  // For disabling the rendering of objects
  hiddenObjects: {
    [id: string]: boolean
  }

  // Optimisation settings
  optimisation: OptimisationSettings
}

export interface OptimisationSettings {
  waitAtStartDuration: number

  startingPoint: [x: number, y: number, z: number]
  endingPoint: [x: number, y: number, z: number]

  maxSpeed: number // mm/s
  transitionMaxSpeed: number // mm/s
}

export function getToMovementSettings(
  settings: Settings,
  objType: 'gpencil',
  overrideKeys: string[],
): GPencilToMovementsSettings

export function getToMovementSettings(
  settings: Settings,
  objType: 'particles',
  overrideKeys: string[],
): ParticlesToMovementsSettings

export function getToMovementSettings<
  ReturnType = GPencilToMovementsSettings | ParticlesToMovementsSettings,
>(
  settings: Settings,
  objType: 'gpencil' | 'particles',
  overrideKeys: string[],
): ReturnType {
  let objSettings = settings.objectSettings[objType]

  // Iterate over every override key, merging in the layers
  for (const objName of overrideKeys) {
    if (settings.objectOverrides[objName]) {
      objSettings = Object.assign(
        {},
        objSettings,
        settings.objectOverrides[objName],
      )
    }
  }

  return objSettings as ReturnType
}

export function getShouldSkip(settings: Settings, keys: string[]) {
  // Iterate over every key, if any are marked as toggle off, skip
  for (const objName of keys) {
    if (settings.hiddenObjects[objName]) {
      return true
    }
  }

  return false
}
