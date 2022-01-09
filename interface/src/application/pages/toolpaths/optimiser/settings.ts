import { CameraToMovementsSettings } from './camera'
import { GPencilToMovementsSettings } from './gpencil'
import { LightToMovementsSettings } from './light'
import { ParticlesToMovementsSettings } from './particles'

export interface Settings {
  // Global object type settings
  objectSettings: {
    gpencil: GPencilToMovementsSettings
    particles: ParticlesToMovementsSettings
    light: LightToMovementsSettings
    camera: CameraToMovementsSettings
  }

  // Per-object overrides
  objectOverrides: {
    [id: string]:
      | Partial<GPencilToMovementsSettings>
      | Partial<ParticlesToMovementsSettings>
      | Partial<LightToMovementsSettings>
      | Partial<CameraToMovementsSettings>
  }

  // For disabling the rendering of objects
  skippedObjects: {
    [id: string]: boolean
  }

  // Optimisation settings
  optimisation: OptimisationSettings
}

export interface OptimisationSettings {
  waitAtStartDuration: number

  /**
   * Whether to smooth interline transitions
   */
  smoothInterlineTransitions: boolean

  /**
   * The angle in degrees below which lines will be taken in a single pass
   */
  interLineTransitionAngle: number

  /**
   * Distance in mm to shave off lines when doing inter line transitions
   */
  interLineTransitionShaveDistance: number

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

export function getToMovementSettings(
  settings: Settings,
  objType: 'light',
  overrideKeys: string[],
): LightToMovementsSettings

export function getToMovementSettings(
  settings: Settings,
  objType: 'camera',
  overrideKeys: string[],
): CameraToMovementsSettings

export function getToMovementSettings<
  ReturnType =
    | GPencilToMovementsSettings
    | ParticlesToMovementsSettings
    | LightToMovementsSettings
    | CameraToMovementsSettings,
>(
  settings: Settings,
  objType: 'gpencil' | 'particles' | 'light' | 'camera',
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
    if (settings.skippedObjects[objName]) {
      return true
    }
  }

  return false
}
