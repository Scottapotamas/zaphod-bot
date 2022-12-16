import { TreeNodeInfo } from '@blueprintjs/core'
import { IconNames } from '@blueprintjs/icons'
import { Color, Vector3 } from 'three'
import { NodeInfo, NodeTypes } from '../interface/RenderableTree'
import { ObjectNameTree } from './files'
import { importMaterial, MaterialJSON } from './material'
import { isSimpleColorMaterial, SimpleColorMaterial } from './materials/Color'
import { Point, Line, Movement, MovementGroup } from './movements'
import { getShouldSkip, getToMovementSettings, Settings } from './settings'

export function map(num: number, in_min: number, in_max: number, out_min: number, out_max: number) {
  return ((num - in_min) * (out_max - out_min)) / (in_max - in_min) + out_min
}

export interface Particle {
  id: string
  location: [number, number, number]
  quaternion: [number, number, number, number]
  velocity: [number, number, number]
  occluded: boolean
  lifecycle: number // Float 0-1, 0 is just born, 1 is just dead.
}

export class ParticleSystem {
  constructor(public name: string, public material: MaterialJSON) {}

  public particles: Particle[] = []

  public addParticle = (layer: Particle) => {
    this.particles.push(layer)
  }
}

export interface ParticlesToMovementsSettings {
  // How long to wait at the particle's position before going bright.
  preWait?: number

  // How long to be on for.
  onDuration?: number

  // How long to wait at the particle's position after going bright.
  postWait?: number

  // Pass through the point in the direction of its velocity, otherwise, stops at it from any direction.
  drawInVelocityOrientation?: boolean

  /**
   * If the particle is occluded, don't render it at all.
   *
   * Note this takes precidence even over material overrides, due to pipeline dependency requirements.
   */
  hideIfOccluded?: boolean

  // Ramp up time, a percentage of time spent going from 0 to full opacity.
  fullBrightnessBy?: number // 0 - 1

  // Ramp down time, a percentage of time spent going from 0 to full opacity.
  fullBrightnessUntil?: number // 0 - 1
}

export class Particles {
  readonly type = 'particles'

  constructor(public name: string) {}

  private systems: ParticleSystem[] = []

  public addSystem = (layer: ParticleSystem) => {
    this.systems.push(layer)
  }

  public getObjectTree: () => TreeNodeInfo<NodeInfo> = () => {
    const node: TreeNodeInfo<NodeInfo> = {
      id: this.name,
      label: this.name,
      icon: IconNames.LAYOUT_SKEW_GRID,
      nodeData: {
        type: NodeTypes.PARTICLES,
      },
      childNodes: this.systems.map(system => ({
        id: `${this.name}-${system.name}`,
        label: system.name,
        icon: IconNames.LAYOUT_GROUP_BY,
        nodeData: {
          type: NodeTypes.PARTICLE_SYSTEM,
        },
      })),
    }

    return node
  }

  public getOriginalMaterialJSON = (objectID: string) => {
    for (const system of this.systems) {
      const systemID = `${this.name}-${system.name}`

      if (systemID === objectID) {
        return system.material
      }
    }

    return null
  }

  public toMovements = (settings: Settings) => {
    const movements: Movement[] = []

    for (const system of this.systems) {
      const objectID = `${this.name}-${system.name}`
      const overrideKeys = [this.name, objectID]

      if (getShouldSkip(settings, overrideKeys)) {
        continue
      }

      const settingsWithOverride = getToMovementSettings(settings, 'particles', overrideKeys)

      for (const particle of system.particles) {
        if (settings.objectSettings.particles.hideIfOccluded && particle.occluded) {
          continue
        }

        // Convert the location to a Vector3
        const location = new Vector3(particle.location[0], particle.location[1], particle.location[2])

        const duration =
          (settingsWithOverride.preWait ?? 0) +
          (settingsWithOverride.onDuration ?? 0) +
          (settingsWithOverride.postWait ?? 0)

        let mat = importMaterial(system.material)

        if (isSimpleColorMaterial(mat)) {
          const fullBrightnessBy = settingsWithOverride.fullBrightnessBy ?? 0
          const fullBrightnessTill = settingsWithOverride.fullBrightnessUntil ?? 1

          let brightness = 1

          // during ramp up
          if (particle.lifecycle < fullBrightnessBy) {
            brightness = map(particle.lifecycle, 0, fullBrightnessBy, 0, 1)
          } else if (particle.lifecycle > fullBrightnessTill) {
            brightness = map(particle.lifecycle, fullBrightnessTill, 1, 1, 0)
          }

          // mutate opacity
          mat.color[3] *= brightness
        }

        const point = new Point(location, duration, mat, objectID, overrideKeys)

        // This ID is guaranteed to be stable
        point.interFrameID = particle.id

        if (settingsWithOverride.drawInVelocityOrientation) {
          point.velocity.set(particle.velocity[0], particle.velocity[1], particle.velocity[2])
        }

        movements.push(point)
      }
    }

    return movements
  }
}

export interface ParticlesJSON {
  type: 'particles'
  frame: number
  name: string
  systems: {
    name: string
    material: MaterialJSON
    particles: {
      id: string
      location: [number, number, number]
      quaternion: [number, number, number, number]
      velocity: [number, number, number]
      occluded: boolean
      lifecycle: number // float 0-1 representing the lifecycle of the particle
    }[]
  }[]
}

export function importParticles(json: ParticlesJSON) {
  const particles = new Particles(json.name)

  for (const jSystem of json.systems) {
    let system = new ParticleSystem(jSystem.name, jSystem.material)
    particles.addSystem(system)

    for (const jParticle of jSystem.particles) {
      system.addParticle(jParticle)
    }
  }

  return particles
}
