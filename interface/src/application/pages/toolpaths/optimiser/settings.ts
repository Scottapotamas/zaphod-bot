import { CameraToMovementsSettings } from './camera'
import { GPencilToMovementsSettings } from './gpencil'
import { LightToMovementsSettings } from './light'
import { ParticlesToMovementsSettings } from './particles'

type DeepPartial<T> = T extends object ? {
  [P in keyof T]?: DeepPartial<T[P]>;
} : T;

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
}

export function getToMovementSettings(
  settings: Settings,
  objType: 'gpencil',
  name: string,
  objectID: string,
): GPencilToMovementsSettings

export function getToMovementSettings(
  settings: Settings,
  objType: 'particles',
  name: string,
  objectID: string,
): ParticlesToMovementsSettings

export function getToMovementSettings(
  settings: Settings,
  objType: 'light',
  name: string,
  objectID: string,
): LightToMovementsSettings

export function getToMovementSettings(
  settings: Settings,
  objType: 'camera',
  name: string,
  objectID: string,
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
    name: string,
    objectID: string,
): ReturnType {

  let objSettings = settings.objectSettings[objType]

  // console.log(`objType ${objType} overrideKeys ${overrideKeys}`)

  if (settings.objectOverrides[name]) {
    return Object.assign(
      {},
      objSettings,
      settings.objectOverrides[name],
    ) as ReturnType
  }

  if (settings.objectOverrides[objectID]) {
    return Object.assign(
      {},
      objSettings,
      settings.objectOverrides[objectID],
    ) as ReturnType
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