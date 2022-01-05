import { Color, PerspectiveCamera, Vector3, Quaternion, Euler } from 'three'
import { NodeInfo, NodeTypes } from '../interface/RenderableTree'
import { TreeNodeInfo } from '@blueprintjs/core'
import { ObjectNameTree } from './files'
import { Renderable } from './import'
import { importMaterial, MaterialJSON } from './material'
import { Point, Line, Movement, MovementGroup } from './movements'
import { getShouldSkip, getToMovementSettings, Settings } from './settings'
import { IconNames } from '@blueprintjs/icons'

export interface CameraToMovementsSettings {
  // Whether to draw alignment helpers in real space
  drawAlignmentHelpers?: boolean
}

const euler = new Euler(0, 0, 0, 'XYZ')

export class Camera {
  readonly type = 'camera'

  constructor(
    public name: string,
    public focalLength: number,
    public sensorHeight: number,
    public sensorWidth: number,
    public position: [number, number, number],
    public rotation: [number, number, number],
    public near: number,
    public far: number,
  ) {}

  public getObjectTree: () => TreeNodeInfo<NodeInfo> = () => {
    const node: TreeNodeInfo<NodeInfo> = {
      id: this.name,
      label: this.name,
      icon: IconNames.CAMERA,
      nodeData: {
        type: NodeTypes.CAMERA,
        hidden: true,
      },
      childNodes: [
        {
          id: `${this.name}-alignment`,
          label: 'Alignment Helper',
          icon: IconNames.SEND_TO_GRAPH,
          nodeData: {
            type: NodeTypes.CAMERA_ALIGNMENT,
            hidden: true,
          },
        },
      ],
    }

    return node
  }

  public toMovements = (settings: Settings) => {
    const movements: Movement[] = []

    // TODO: Create the alignment helper, if it's skipped, skip it

    return movements
  }

  public alignCamera = (camera: PerspectiveCamera) => {
    camera.filmGauge = this.sensorWidth
    camera.setFocalLength(this.focalLength)

    // Do the blender -> threejs transform
    camera.position.set(this.position[0], this.position[2], -this.position[1])
    euler.set(this.rotation[0], this.rotation[2], this.rotation[1], 'XYZ')

    camera.quaternion.setFromEuler(euler)

    const quaternionTransform = new Quaternion()

    // No idea what this transform should be...
    quaternionTransform.setFromEuler(new Euler(0, (Math.PI * 8.4) / 4, 0))

    camera.quaternion.multiply(quaternionTransform)

    // camera.lookAt(0, 0, 0)

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
  rotation: [number, number, number]
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
    json.rotation,
    json.near,
    json.far,
  )

  return camera
}
