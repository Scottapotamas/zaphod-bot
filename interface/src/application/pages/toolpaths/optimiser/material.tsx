import {
  ColorMaterialDefaultJSON,
  ColorMaterialJSON,
  importColorMaterial,
} from './materials/Color'
import {
  ColorRampMaterialDefaultJSON,
  ColorRampMaterialJSON,
  importColorRampMaterial,
} from './materials/ColorRamp'
import {
  FlickerMaterialDefaultJSON,
  FlickerMaterialJSON,
  importFlickerMaterial,
} from './materials/Flicker'
import {
  importInvisibleMaterial,
  InvisibleMaterial,
  InvisibleMaterialDefaultJSON,
  InvisibleMaterialJSON,
} from './materials/Invisible'
import {
  importRandomColorMaterial,
  RandomColorMaterialDefaultJSON,
  RandomColorMaterialJSON,
} from './materials/Random'
import { MATERIALS } from './materials/utilities'
import {
  importVelocityMaterial,
  VelocityMaterialDefaultJSON,
  VelocityMaterialJSON,
} from './materials/Velocity'
import {
  importZGradientMaterial,
  ZGradientMaterialDefaultJSON,
  ZGradientMaterialJSON,
} from './materials/ZGradient'
import {
  importZDepthMaterial,
  ZDepthMaterialDefaultJSON,
  ZDepthMaterialJSON,
} from './materials/ZDepth'
import {
  importBlendMaterial,
  BlendMaterialDefaultJSON,
  BlendMaterialJSON,
} from './materials/Blend'
import { VisualisationSettings } from '../interface/state'
import { Material } from './materials/Base'

export type MaterialJSON =
  | ColorMaterialJSON
  | ColorRampMaterialJSON
  | VelocityMaterialJSON
  | InvisibleMaterialJSON
  | RandomColorMaterialJSON
  | FlickerMaterialJSON
  | ZGradientMaterialJSON
  | ZDepthMaterialJSON
  | BlendMaterialJSON

export const defaultTransitionMaterial = new InvisibleMaterial([
  19 / 255,
  124 / 255,
  189 / 255,
  1
])

export function importMaterial(json: MaterialJSON) {
  try {
    if (!json.type) {
      console.log(json)
      throw new Error(`Error importing material, no material type defined`)
    }

    switch (json.type) {
      case MATERIALS.COLOR:
        return importColorMaterial(json)
      case MATERIALS.COLOR_RAMP:
        return importColorRampMaterial(json)
      case MATERIALS.VELOCITY:
        return importVelocityMaterial(json)
      case MATERIALS.INVISIBLE:
        return importInvisibleMaterial(json)
      case MATERIALS.RANDOM:
        return importRandomColorMaterial(json)
      case MATERIALS.FLICKER:
        return importFlickerMaterial(json)
      case MATERIALS.Z_GRADIENT:
        return importZGradientMaterial(json)
      case MATERIALS.Z_DEPTH:
        return importZDepthMaterial(json)
      case MATERIALS.BLEND:
        return importBlendMaterial(json)

      default:
        throw new Error(
          `Error importing material, unknown type ${json['type']}`,
        )
    }
  } catch (e) {
    console.error(e)
    return importInvisibleMaterial({
      type: MATERIALS.INVISIBLE,
      color: [0, 0, 0, 0],
    })
  }
}

export function getDefaultJSONForType(type: MATERIALS) {
  switch (type) {
    case MATERIALS.DEFAULT:
      return null // No JSON override for a default
    case MATERIALS.COLOR:
      return ColorMaterialDefaultJSON
    case MATERIALS.COLOR_RAMP:
      return ColorRampMaterialDefaultJSON
    case MATERIALS.VELOCITY:
      return VelocityMaterialDefaultJSON
    case MATERIALS.INVISIBLE:
      return InvisibleMaterialDefaultJSON
    case MATERIALS.RANDOM:
      return RandomColorMaterialDefaultJSON
    case MATERIALS.FLICKER:
      return FlickerMaterialDefaultJSON
    case MATERIALS.Z_GRADIENT:
      return ZGradientMaterialDefaultJSON
    case MATERIALS.Z_DEPTH:
      return ZDepthMaterialDefaultJSON
    case MATERIALS.BLEND:
      return BlendMaterialDefaultJSON

    default:
      throw new Error(
        `Error creating default for material, unknown type ${type}`,
      )
  }
}

export function getMaterialOverride(
  visualisationSettings: VisualisationSettings,
  providedMaterial: Material,
  overrideKeys: string[],
) {
  let material = providedMaterial

  // Check all override keys in precidence order
  for (let index = 0; index < overrideKeys.length; index++) {
    const overrideKey = overrideKeys[index]

    const movementMaterialOverride =
      visualisationSettings.objectMaterialOverrides[overrideKey]

    // Specific movement overrides take highest precidence
    if (movementMaterialOverride) {
      material = importMaterial(movementMaterialOverride)
    }
  }

  // can probably cache this

  return material
}
