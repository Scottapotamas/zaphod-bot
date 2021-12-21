import { Color, Vector3 } from 'three'
import { importMaterial, MaterialJSON } from './material'
import { Point, Line, Movement, MovementGroup } from './movements'
import {
  getMaterialOverride,
  getShouldSkip,
  getToMovementSettings,
  Settings,
} from './settings'

export interface ParticlesStrokePoint {
  co: [number, number, number] // position
  pressure: number // Pressure of tablet at point when drawing it
  strength: number // Color intensity (alpha factor)
  vertexColor: [number, number, number, number] // Vertex color
}

export interface Particle {
  id: string
  location: [number, number, number]
  rotation: [number, number, number]
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
  constructor(public name: string) {}

  private systems: ParticleSystem[] = []

  public addSystem = (layer: ParticleSystem) => {
    this.systems.push(layer)
  }

  public getObjectNameTree = () => {
    return {
      [this.name]: this.systems.map(system => `${this.name}-${system.name}`),
    }
  }

  public toMovements = (settings: Settings) => {
    const movements: Movement[] = []

    for (const system of this.systems) {
      for (const particle of system.particles) {
        const overrideKeys = [this.name, `${this.name}-${system.name}`]

        if (getShouldSkip(settings, overrideKeys)) {
          continue
        }

        const settingsWithOverride = getToMovementSettings(
          settings,
          'particles',
          [this.name, `${this.name}-${system.name}`],
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

          getMaterialOverride(settings, system.material, overrideKeys),
        )

        // This ID is guaranteed to be stable
        point.id = particle.id

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
      rotation: [number, number, number]
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
