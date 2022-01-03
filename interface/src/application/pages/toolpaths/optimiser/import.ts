import { CameraJSON, Camera, importCamera } from './camera'
import {
  GPencil,
  GPencilJSON,
  GPencilToMovementsSettings,
  importGPencil,
} from './gpencil'
import { Material, MaterialJSON } from './material'
import {
  importParticles,
  Particles,
  ParticlesJSON,
  ParticlesToMovementsSettings,
} from './particles'

export type MovementJSON = GPencilJSON | ParticlesJSON | CameraJSON
export type Renderable = GPencil | Particles | Camera

export function importJson(json: MovementJSON): Renderable {
  switch (json.type) {
    case 'gpencil':
      return importGPencil(json)

    case 'particles':
      return importParticles(json)

    case 'camera':
      return importCamera(json)

    default:
      throw new Error(`Error importing movement, unknown type ${json['type']}`)
  }
}
