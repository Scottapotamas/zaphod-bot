import type { CameraToMovementsSettings } from './camera'
import type { GPencilToMovementsSettings } from './gpencil'
import type { LightToMovementsSettings } from './light'
import type { ParticlesToMovementsSettings } from './particles'
import type { EffectorToMovementSettings } from './effector'
import type { GNodesMeshToMovementsSettings } from './gnodes_mesh'

const overwriteMerge = (destinationArray: any[], sourceArray: any[]) =>
  sourceArray

type DeepPartial<T> = T extends object
  ? {
      [P in keyof T]?: DeepPartial<T[P]>
    }
  : T

export interface Settings {
  // Global object type settings
  objectSettings: {
    gpencil: GPencilToMovementsSettings
    particles: ParticlesToMovementsSettings
    light: LightToMovementsSettings
    camera: CameraToMovementsSettings
    effector: EffectorToMovementSettings
    gnodesMesh: GNodesMeshToMovementsSettings
  }

  // Per-object overrides
  objectOverrides: {
    [id: string]:
      | Partial<GPencilToMovementsSettings>
      | Partial<ParticlesToMovementsSettings>
      | Partial<LightToMovementsSettings>
      | Partial<CameraToMovementsSettings>
      | Partial<EffectorToMovementSettings>
      | Partial<GNodesMeshToMovementsSettings>
  }

  // For disabling the rendering of objects
  skippedObjects: {
    [id: string]: boolean
  }

  // Optimisation settings
  optimisation: OptimisationSettings

  // Tools for debugging
  lightFadeOffset: number
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

  /**
   * Line runups, mitigates velocity changes
   */
  lineRunUp: number

  /**
   * Disable shaped transitions
   */
  disableShapedTransitions: boolean

  /**
   * How long the transitions should go out from the lines
   */
  transitionSize: number

  maxSpeed: number // mm/s

  /**
   * Which optimisation passes are enabled
   */
  passes: {
    nearestNeighbour: boolean
    bruteForceSmall: boolean
    twoOpt: boolean
  }
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

export function getToMovementSettings(
  settings: Settings,
  objType: 'effector',
  overrideKeys: string[],
): EffectorToMovementSettings

export function getToMovementSettings(
  settings: Settings,
  objType: 'gnodesMesh',
  overrideKeys: string[],
): GNodesMeshToMovementsSettings

export function getToMovementSettings<
  ReturnType extends
    | GPencilToMovementsSettings
    | ParticlesToMovementsSettings
    | LightToMovementsSettings
    | CameraToMovementsSettings
    | EffectorToMovementSettings
    | GNodesMeshToMovementsSettings,
>(
  settings: Settings,
  objType:
    | 'gpencil'
    | 'particles'
    | 'light'
    | 'camera'
    | 'effector'
    | 'gnodesMesh',
  overrideKeys: string[],
): ReturnType {
  let objSettings = settings.objectSettings[objType]

  // console.log(`objType ${objType} overrideKeys ${overrideKeys}`)

  let lazilyGeneratedOverride = false

  for (let index = 0; index < overrideKeys.length; index++) {
    const overrideKey = overrideKeys[index]

    // If this key matches
    if (settings.objectOverrides[overrideKey]) {
      // Lazily generate the override copy
      if (!lazilyGeneratedOverride) {
        lazilyGeneratedOverride = true

        // Form a shallow copy
        objSettings = Object.assign(
          {},
          objSettings,
          settings.objectOverrides[overrideKey],
        ) as ReturnType
        continue
      }

      // If we have more than one level of nesting, merge in the other keys
      objSettings = deepmerge(
        objSettings,
        settings.objectOverrides[overrideKey],
        {
          arrayMerge: overwriteMerge,
        },
      ) as ReturnType
    }
  }

  return objSettings as ReturnType
}

export function getShouldSkip(settings: Settings, keys: string[]) {
  // Iterate over every key, if any are marked as toggle off, skip
  for (const key of keys) {
    if (settings.skippedObjects[key]) {
      return true
    }
  }

  return false
}
