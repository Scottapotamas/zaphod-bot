import { TreeNodeInfo } from '@blueprintjs/core'
import { IconNames } from '@blueprintjs/icons'
import { Color, Vector3 } from 'three'
import { NodeInfo, NodeTypes } from '../interface/RenderableTree'
import { ObjectNameTree } from './files'
import { importMaterial, MaterialJSON } from './material'
import { Point, Line, Movement, MovementGroup } from './movements'
import { getShouldSkip, getToMovementSettings, Settings } from './settings'

export interface ParticlesStrokePoint {
  co: [number, number, number] // position
  pressure: number // Pressure of tablet at point when drawing it
  strength: number // Color intensity (alpha factor)
  vertexColor: [number, number, number, number] // Vertex color
}

export interface Particle {
  id: string
  location: [number, number, number]
  quaternion: [number, number, number, number]
  velocity: [number, number, number]
}

export class ParticleSystem {
  constructor(public name: string, public material: MaterialJSON) {}

  public particles: Particle[] = []

  public addParticle = (layer: Particle) => {
    this.particles.push(layer)
  }
}

export interface ParticlesToMovementsSettings {
  // How long to wait at each particle before moving on.
  stopDelay?: number

  // Pass through the point in the direction of its velocity, otherwise, stops at it from any direction.
  drawInVelocityOrientation?: boolean
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

  public toMovements = (settings: Settings) => {
    const movements: Movement[] = []

    for (const system of this.systems) {
      for (const particle of system.particles) {
        const objectID = `${this.name}-${system.name}`
        const overrideKeys = [this.name, objectID]

        if (getShouldSkip(settings, overrideKeys)) {
          continue
        }

        const settingsWithOverride = getToMovementSettings(
          settings,
          'particles',
          [this.name, objectID],
        )

        // Convert the location to a Vector3
        const location = new Vector3(
          particle.location[0],
          particle.location[1],
          particle.location[2],
        )

        const point = new Point(
          location,
          settingsWithOverride.stopDelay ?? 0,
          importMaterial(system.material),
          objectID,
        )

        // This ID is guaranteed to be stable
        point.interFrameID = particle.id

        if (settingsWithOverride.drawInVelocityOrientation) {
          point.velocity.set(
            particle.velocity[0],
            particle.velocity[1],
            particle.velocity[2],
          )
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
