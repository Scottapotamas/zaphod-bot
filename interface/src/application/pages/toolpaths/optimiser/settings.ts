import { GPencilToMovementsSettings } from './gpencil'
import { importMaterial, Material, MaterialJSON } from './material'
import { ParticlesToMovementsSettings } from './particles'
import { Vector3 } from 'three'
import { LRUCache } from 'typescript-lru-cache'

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

  // Materials
  transitionMaterial: MaterialJSON
  materialOverrides: MaterialOverrides

  // Optimisation settings
  optimisation: OptimisationSettings
}

export interface MaterialOverrides {
  globalOveride: MaterialJSON | null

  // Do object level material overrides here.
  objectMaterialOverrides: {
    [objectName: string]: MaterialJSON
  }
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

const materialCache = new LRUCache<string, Material>({
  maxSize: 10_000, // store up to 10k materials before dumping them
})

// Cache the transition material
export function getTransitionMaterial(settings: Settings) {
  const key = JSON.stringify(settings.transitionMaterial)

  if (materialCache.has(key)) {
    return materialCache.get(key)!
  }

  const material = importMaterial(settings.transitionMaterial)
  materialCache.set(key, material)

  return material
}

export function getMaterialOverride(
  settings: Settings,
  providedMaterial: MaterialJSON,
  overrideKeys: string[],
) {
  let mat = providedMaterial

  if (settings.materialOverrides.globalOveride) {
    mat = settings.materialOverrides.globalOveride
  }

  // Iterate over every override key, merging in the layers
  for (const objName of overrideKeys) {
    if (settings.materialOverrides.objectMaterialOverrides[objName]) {
      mat = settings.materialOverrides.objectMaterialOverrides[objName]
    }
  }

  // See if we've imported it before
  const key = JSON.stringify(mat)

  if (materialCache.has(key)) {
    return materialCache.get(key)!
  }

  const material = importMaterial(mat)
  materialCache.set(key, material)

  return material
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
