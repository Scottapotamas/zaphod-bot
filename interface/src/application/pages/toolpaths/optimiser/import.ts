import { CameraJSON, Camera, importCamera } from './camera'
import { GPencil, GPencilJSON, importGPencil } from './gpencil'
import { importParticles, Particles, ParticlesJSON } from './particles'
import { importLight, Light, LightJSON } from './light'
import { importEmpty, Empty, EmptyJSON } from './empty'
import { Effector, EffectorJSON, importEffector } from './effector'

export type MovementJSON = GPencilJSON | ParticlesJSON | CameraJSON | LightJSON | EmptyJSON | EffectorJSON
export type Renderable = GPencil | Particles | Camera | Light | Empty | Effector

export function importJson(json: MovementJSON): Renderable {
  switch (json.type) {
    case 'gpencil':
      return importGPencil(json)

    case 'particles':
      return importParticles(json)

    case 'light':
      return importLight(json)

    case 'camera':
      return importCamera(json)

    case 'empty':
      return importEmpty(json)

    case 'effector':
      return importEffector(json)

    default:
      throw new Error(`Error importing movement, unknown type ${json['type']}`)
  }
}
