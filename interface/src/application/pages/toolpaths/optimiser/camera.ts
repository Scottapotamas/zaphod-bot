import { Color, PerspectiveCamera, Vector3, Quaternion } from 'three'
import { Renderable } from './import'
import { importMaterial, MaterialJSON } from './material'
import { Point, Line, Movement, MovementGroup } from './movements'
import {
  getMaterialOverride,
  getShouldSkip,
  getToMovementSettings,
  Settings,
} from './settings'

export interface CameraToMovementsSettings {
  // Whether to draw alignment helpers in real space
  drawAlignmentHelpers?: boolean
}

const quaternionTransform = new Quaternion()
quaternionTransform.setFromAxisAngle(new Vector3(0, 1, 0), -Math.PI / 2)

export class Camera {
  readonly type = 'camera'

  constructor(
    public name: string,
    public focalLength: number,
    public sensorHeight: number,
    public sensorWidth: number,
    public position: [number, number, number],
    public quaternion: [number, number, number, number],
    public near: number,
    public far: number,
  ) {}

  public getObjectNameTree = () => {
    return {
      [this.name]: {
        frustum: `${this.name}-alignment`,
      },
    }
  }

  public toMovements = (settings: Settings) => {
    const movements: Movement[] = []

    return movements
  }

  public alignCamera = (camera: PerspectiveCamera) => {
    camera.filmGauge = this.sensorWidth
    camera.setFocalLength(this.focalLength)

    console.log(this.position, this.quaternion)

    // Do the blender -> threejs transform
    camera.position.set(this.position[0], this.position[2], -this.position[1])
    // camera.quaternion.set(
    //   this.quaternion[0],
    //   this.quaternion[1],
    //   this.quaternion[2],
    //   this.quaternion[3],
    // )
    // camera.quaternion.multiply(quaternionTransform)
    camera.lookAt(0, 0, 0)

    camera.updateProjectionMatrix()
  }
}

export function isCamera(renderable: Renderable): renderable is Camera {
  return renderable.type === 'camera'
}

export interface CameraJSON {
  type: 'camera'
  frame: number
  name: string
  focal_length: number
  sensor_height: number
  sensor_width: number
  position: [number, number, number]
  quaternion: [number, number, number, number]
  near: number
  far: number
}

export function importCamera(json: CameraJSON) {
  const camera = new Camera(
    json.name,
    json.focal_length,
    json.sensor_height,
    json.sensor_width,
    json.position,
    json.quaternion,
    json.near,
    json.far,
  )

  return camera
}
