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
  importInvisibleMaterial,
  InvisibleMaterial,
  InvisibleMaterialDefaultJSON,
  InvisibleMaterialJSON,
} from './materials/Invisible'
import { MATERIALS } from './materials/utilities'
import {
  importVelocityMaterial,
  VelocityMaterialDefaultJSON,
  VelocityMaterialJSON,
} from './materials/Velocity'

export type MaterialJSON =
  | ColorMaterialJSON
  | ColorRampMaterialJSON
  | VelocityMaterialJSON
  | InvisibleMaterialJSON

export const defaultTransitionMaterial = new InvisibleMaterial([
  19 / 255,
  124 / 255,
  189 / 255,
])

export function importMaterial(json: MaterialJSON) {
  switch (json.type) {
    case MATERIALS.COLOR:
      return importColorMaterial(json)
    case MATERIALS.COLOR_RAMP:
      return importColorRampMaterial(json)
    case MATERIALS.VELOCITY:
      return importVelocityMaterial(json)
    case MATERIALS.INVISIBLE:
      return importInvisibleMaterial(json)

    default:
      throw new Error(`Error importing material, unknown type ${json['type']}`)
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

    default:
      throw new Error(
        `Error creating default for material, unknown type ${type}`,
      )
  }
}
