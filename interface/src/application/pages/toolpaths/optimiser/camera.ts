import {
  Color,
  PerspectiveCamera,
  Vector3,
  Quaternion,
  Euler,
  MathUtils,
} from 'three'
import { NodeInfo, NodeTypes } from '../interface/RenderableTree'
import { TreeNodeInfo } from '@blueprintjs/core'
import { ObjectNameTree } from './files'
import { Renderable } from './import'
import { importMaterial, MaterialJSON } from './material'
import { Point, Line, Movement, MovementGroup } from './movements'
import { getShouldSkip, getToMovementSettings, Settings } from './settings'
import { IconNames } from '@blueprintjs/icons'
import { SimpleColorMaterial } from './materials/Color'
import { DelayMaterial } from './materials/DelayMaterial'
import { optimalFlippingForTour } from './passes'

export interface CameraToMovementsSettings {
  // Whether to draw alignment helpers in real space
  drawAlignmentHelpers?: boolean
  // Enable rulers for debugging
  drawRulers?: boolean
  // Enable coloured lines for debugging
  drawColorCalibrationChart?: boolean
  // Enable lines for debugging
  drawMoveCalibrationChart?: boolean
}

const conversionQuaternion = new Quaternion()
conversionQuaternion
  .setFromAxisAngle(new Vector3(0, 1, 0), Math.PI / 2)
  .invert() // Invert the three -> blender transform

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

  public getObjectTree: () => TreeNodeInfo<NodeInfo> = () => {
    const node: TreeNodeInfo<NodeInfo> = {
      id: this.name,
      label: this.name,
      icon: IconNames.CAMERA,
      nodeData: {
        type: NodeTypes.CAMERA,
      },
      childNodes: [
        {
          id: `${this.name}-alignment`,
          label: 'Alignment Helper',
          icon: IconNames.SEND_TO_GRAPH,
          nodeData: {
            type: NodeTypes.CAMERA_ALIGNMENT,
          },
        },
        {
          id: `${this.name}-ruler`,
          label: 'Ruler',
          icon: IconNames.MAXIMIZE,
          nodeData: {
            type: NodeTypes.CAMERA_RULER,
          },
        },
        {
          id: `${this.name}-color-lines`,
          label: 'Ruler',
          icon: IconNames.TIMELINE_BAR_CHART,
          nodeData: {
            type: NodeTypes.CAMERA_COLOR_LINES,
          },
        },
      ],
    }

    return node
  }

  public getOriginalMaterialJSON = (objectID: string) => {
    return null
  }

  public toMovements = (settings: Settings) => {
    const groups: {
      [height: number]: MovementGroup
    } = {}

    function addMovement(movement: Movement) {
      movement.interFrameID = `(${movement.getStart().x},${
        movement.getStart().y
      },${movement.getStart().z}->${movement.getEnd().x},${
        movement.getEnd().y
      },${movement.getEnd().z})`

      const height = movement.getApproximateCentroid().z

      if (!groups[height]) {
        groups[height] = new MovementGroup()
      }

      const group = groups[height]

      group.addMovement(movement)
    }

    if (settings.objectSettings.camera.drawRulers) {
      const grey = new SimpleColorMaterial([0.4, 0.4, 0.4])
      const objectID = `${this.name}-ruler`

      // Horizontal line
      addMovement(
        new Line(
          new Vector3(0, 0, 100),
          new Vector3(100, 0, 100),
          grey,
          objectID,
        ),
      )

      // Vertical lines
      for (let index = 0; index <= 10; index++) {
        addMovement(
          new Line(
            new Vector3(index * 10, 0, 100),
            new Vector3(index * 10, 0, index % 5 === 0 ? 120 : 110),
            grey,
            objectID,
          ),
        )
      }
    }

    if (settings.objectSettings.camera.drawAlignmentHelpers) {
      const grey = new SimpleColorMaterial([0.4, 0.4, 0.4])
      const red = new SimpleColorMaterial([0.4, 0.0, 0.0])
      const green = new SimpleColorMaterial([0.0, 0.4, 0.0])
      const blue = new SimpleColorMaterial([0.0, 0.0, 0.4])
      const black = new SimpleColorMaterial([0.0, 0.0, 0.0])
      const objectID = `${this.name}-alignment`

      // A line toward camera

      const directionOfCamera = new Vector3(
        this.position[0],
        this.position[1],
        this.position[2],
      ).normalize()

      const center = new Vector3(0, 0, 100)

      // A 50mm line from 100mm up (the center of the draw volume) aimed directly at the camera
      addMovement(
        new Line(
          center.clone().add(directionOfCamera.clone().multiplyScalar(10)),
          center.clone().add(directionOfCamera.clone().multiplyScalar(60)),
          grey,
          objectID,
        ),
      )

      // Add a 10mm line in the direction of the camera's quaternion for debugging
      const blenderCameraOrientation = new Quaternion(
        this.quaternion[0],
        this.quaternion[1],
        this.quaternion[2],
        this.quaternion[3],
      )
      const xVec = new Vector3(1, 0, 0)
      xVec.applyQuaternion(blenderCameraOrientation)
      const yVec = new Vector3(0, 1, 0)
      yVec.applyQuaternion(blenderCameraOrientation)
      const zVec = new Vector3(0, 0, 1)
      zVec.applyQuaternion(blenderCameraOrientation)

      addMovement(
        new Line(
          center.clone().add(xVec.clone().multiplyScalar(0)),
          center.clone().add(xVec.clone().multiplyScalar(10)),
          red,
          objectID,
        ),
      )
      addMovement(
        new Line(
          center.clone().add(yVec.clone().multiplyScalar(0)),
          center.clone().add(yVec.clone().multiplyScalar(10)),
          green,
          objectID,
        ),
      )
      addMovement(
        new Line(
          center.clone().add(zVec.clone().multiplyScalar(0)),
          center.clone().add(zVec.clone().multiplyScalar(10)),
          blue,
          objectID,
        ),
      )

      // The camera should aim at the end point of this line
      addMovement(
        new Line(
          center.clone().add(zVec.clone().multiplyScalar(0)),
          center.clone().add(zVec.clone().multiplyScalar(-20)),
          black,
          objectID,
        ),
      )
    }

    if (settings.objectSettings.camera.drawColorCalibrationChart) {
      const objectID = `${this.name}-color-lines`

      // Left side, 20 linear gradations of black to white
      for (let index = 0; index <= 20; index++) {
        addMovement(
          new Line(
            new Vector3(-50, 0, index * 5 + 50),
            new Vector3(-40, 0, index * 5 + 50),
            new SimpleColorMaterial([index / 10, index / 10, index / 10]),
            objectID,
          ),
        )
      }

      // Left side, 20 linear gradations of black to red
      for (let index = 0; index <= 20; index++) {
        addMovement(
          new Line(
            new Vector3(-30, 0, index * 5 + 50),
            new Vector3(-20, 0, index * 5 + 50),
            new SimpleColorMaterial([index / 10, 0, 0]),
            objectID,
          ),
        )
      }

      // Left side, 20 linear gradations of black to green
      for (let index = 0; index <= 20; index++) {
        addMovement(
          new Line(
            new Vector3(-10, 0, index * 5 + 50),
            new Vector3(0, 0, index * 5 + 50),
            new SimpleColorMaterial([0, index / 10, 0]),
            objectID,
          ),
        )
      }

      // Left side, 20 linear gradations of black to blue
      for (let index = 0; index <= 20; index++) {
        addMovement(
          new Line(
            new Vector3(10, 0, index * 5 + 50),
            new Vector3(20, 0, index * 5 + 50),
            new SimpleColorMaterial([0, 0, index / 10]),
            objectID,
          ),
        )
      }

      const white = new SimpleColorMaterial([1, 1, 1])

      // Left side, 20 linear gradations of white at different speeds, from 10mm/s to 210mm/s
      for (let index = 0; index <= 20; index++) {
        const movement = new Line(
          new Vector3(30, 0, index * 5 + 50),
          new Vector3(40, 0, index * 5 + 50),
          white,
          objectID,
        )

        movement.maxSpeed = (index + 1) * 10
        movement.lockSpeed = true

        addMovement(movement)
      }

      const grey = new SimpleColorMaterial([0.5, 0.5, 0.5])

      // Left side, 20 linear gradations of grey at different speeds, from 10mm/s to 210mm/s
      for (let index = 0; index <= 20; index++) {
        const movement = new Line(
          new Vector3(50, 0, index * 5 + 50),
          new Vector3(60, 0, index * 5 + 50),
          grey,
          objectID,
        )

        movement.maxSpeed = (index + 1) * 10
        movement.lockSpeed = true

        addMovement(movement)
      }
    }

    if (settings.objectSettings.camera.drawMoveCalibrationChart) {
      const objectID = `${this.name}-moves-lines`

      // A point with two lines pointing at it in the bottom right
      addMovement(
        new Line(
          new Vector3(30, 0, 70),
          new Vector3(40, 0, 70),
          new SimpleColorMaterial([0.5, 0.5, 0.5]),
          objectID,
        ),
      )

      const duration =
        (settings.objectSettings.light.preWait ?? 0) +
        (settings.objectSettings.light.onDuration ?? 0) +
        (settings.objectSettings.light.postWait ?? 0)

      addMovement(
        new Point(
          new Vector3(50, 0, 70),
          duration,
          new DelayMaterial(
            new SimpleColorMaterial([0.5, 0.5, 0.5]),
            settings.objectSettings.light.preWait ?? 0,
            settings.objectSettings.light.onDuration ?? 0,
            settings.objectSettings.light.postWait ?? 0,
          ),
          objectID,
        ),
      )

      addMovement(
        new Line(
          new Vector3(50, 0, 50),
          new Vector3(50, 0, 60),
          new SimpleColorMaterial([0.5, 0.5, 0.5]),
          objectID,
        ),
      )

      // A series of angles

      let i = 0
      for (const iterator of [0, 11.25, 22.5, 45, 90]) {
        const angle = new Euler(0, MathUtils.degToRad(-iterator), 0)

        const delta = new Vector3(1, 0, 0).applyEuler(angle).multiplyScalar(10)

        addMovement(
          // 0 degrees
          new Line(
            new Vector3(30, 0, 80 + i * 5),
            new Vector3(40, 0, 80 + i * 5),
            new SimpleColorMaterial([1, 1, 0]),
            objectID,
          ),
        )
        addMovement(
          // 90
          new Line(
            new Vector3(40, 0, 80 + i * 5),
            new Vector3(40, 0, 80 + i * 5).add(delta),
            new SimpleColorMaterial([1, 0, 1]),
            objectID,
          ),
        )
        i++
      }

      i = 0
      for (const iterator of [0, 11.25, 22.5, 33.75, 45]) {
        const angle = new Euler(0, MathUtils.degToRad(-iterator - 135), 0)

        const delta = new Vector3(1, 0, 0).applyEuler(angle).multiplyScalar(10)

        addMovement(
          // 0 degrees
          new Line(
            new Vector3(40, 0, 110 + i * 10),
            new Vector3(50, 0, 110 + i * 10),
            new SimpleColorMaterial([1, 1, 0]),
            objectID,
          ),
        )
        addMovement(
          // 90
          new Line(
            new Vector3(50, 0, 110 + i * 10),
            new Vector3(50, 0, 110 + i * 10).add(delta),
            new SimpleColorMaterial([1, 0, 1]),
            objectID,
          ),
        )
        i++
      }
    }

    // Give the TSP solver a nudge in the right direction
    // movements.sort((a, b) => {
    //   return b.samplePoint(0).z - a.samplePoint(0).z
    // })

    const movements: Movement[] = []

    let flip = false
    for (const group of Object.values(groups)) {
      if (flip) {
        group.flip()
        flip = !flip
      }
      movements.push(group)
    }

    optimalFlippingForTour(movements)

    return movements
  }

  public alignCamera = (camera: PerspectiveCamera) => {
    camera.filmGauge = this.sensorWidth
    camera.setFocalLength(this.focalLength)

    // Do the blender -> threejs transform
    camera.position.set(this.position[0], this.position[2], -this.position[1])

    camera.quaternion.set(
      this.quaternion[0],
      this.quaternion[1],
      this.quaternion[2],
      this.quaternion[3],
    )

    if (false) {
      const blenderCameraOrientation = new Quaternion(
        this.quaternion[0],
        this.quaternion[1],
        this.quaternion[2],
        this.quaternion[3],
      )

      const zVec = new Vector3(0, 0, 1)
      zVec.applyQuaternion(blenderCameraOrientation)

      const toLook = new Vector3(
        this.position[0],
        this.position[2],
        -this.position[1],
      ).add(zVec.clone().multiplyScalar(-100))

      camera.lookAt(toLook)
    }

    camera.updateMatrixWorld()
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
  quaternion: [x: number, y: number, z: number, w: number]
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
