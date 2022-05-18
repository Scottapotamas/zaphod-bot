import {
  Color,
  PerspectiveCamera,
  Vector3,
  Quaternion,
  Euler,
  MathUtils,
  Plane,
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
  // Whether to draw the extrinsic calibration rects
  drawExtrinsicCalibrators?: boolean
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

    if (settings.objectSettings.camera.drawExtrinsicCalibrators) {
      const red = new SimpleColorMaterial([0.4, 0.0, 0.0])
      const green = new SimpleColorMaterial([0.0, 0.4, 0.0])

      const objectID = `${this.name}-calibration`

      // Red rect
      addMovement(new Line( new Vector3(200, 0,  50),  new Vector3(10,  0,  50), red, objectID)) // prettier-ignore
      addMovement(new Line( new Vector3(10,  0,  50),  new Vector3(10,  0, 150), red, objectID)) // prettier-ignore
      addMovement(new Line( new Vector3(200, 0, 150),  new Vector3(10,  0, 150), red, objectID)) // prettier-ignore
      addMovement(new Line( new Vector3(200, 0,  50),  new Vector3(200, 0, 150), red, objectID)) // prettier-ignore

      // green rect
      addMovement(new Line( new Vector3(0, 200,  50),  new Vector3(0, 10,   50), green, objectID)) // prettier-ignore
      addMovement(new Line( new Vector3(0, 10,   50),  new Vector3(0, 10,  150), green, objectID)) // prettier-ignore
      addMovement(new Line( new Vector3(0, 200, 150),  new Vector3(0, 10,  150), green, objectID)) // prettier-ignore
      addMovement(new Line( new Vector3(0, 200,  50),  new Vector3(0, 200, 150), green, objectID)) // prettier-ignore
    }

    if (settings.objectSettings.camera.drawAlignmentHelpers) {
      const grey = new SimpleColorMaterial([0.4, 0.4, 0.4])
      const red = new SimpleColorMaterial([0.4, 0.0, 0.0])
      const green = new SimpleColorMaterial([0.0, 0.4, 0.0])
      const yellow = new SimpleColorMaterial([0.4, 0.4, 0.0])
      const black = new SimpleColorMaterial([0.0, 0.0, 0.0])
      const objectID = `${this.name}-alignment`

      // From the center
      const center = new Vector3(0, 0, 100)

      // A line toward camera, from the center
      const directionOfCamera = new Vector3(
        this.position[0],
        this.position[1],
        this.position[2],
      )
        .sub(center)
        .normalize()

      // Generate the quaternion of the rotation of the camera's position relative to the point
      const directionEuler = new Euler(
        directionOfCamera.x,
        directionOfCamera.y,
        directionOfCamera.z,
      )

      function align(index: number) {
        const right = directionOfCamera
          .clone()
          .cross(new Vector3(0, 0, 1))
          .normalize()

        const up = directionOfCamera
          .clone()
          .cross(new Vector3(0, 1, 0))
          .normalize()

        switch (index) {
          case 0:
            return right.multiplyScalar(-squareSize).add(up.multiplyScalar(-squareSize)) // prettier-ignore
          case 1:
            return right.multiplyScalar(-squareSize).add(up.multiplyScalar(squareSize)) // prettier-ignore
          case 2:
            return right.multiplyScalar(squareSize).add(up.multiplyScalar(squareSize)) // prettier-ignore
          case 3:
            return right.multiplyScalar(squareSize).add(up.multiplyScalar(-squareSize)) // prettier-ignore
        }

        return new Vector3(0, 0, 0)
      }

      // Square points at center, aimed 'at' the camera
      const squareSize = 25

      const behindCamera50 = center.clone().add(directionOfCamera.clone().multiplyScalar(-50)) // prettier-ignore

      const pointA = align(0).add(behindCamera50) // prettier-ignore
      const pointB = align(1).add(behindCamera50) // prettier-ignore
      const pointC = align(2).add(behindCamera50) // prettier-ignore
      const pointD = align(3).add(behindCamera50) // prettier-ignore

      // Square points at center
      const pointE = align(0).add(center) // prettier-ignore
      const pointF = align(1).add(center) // prettier-ignore
      const pointG = align(2).add(center) // prettier-ignore
      const pointH = align(3).add(center) // prettier-ignore

      const towardCamera50 = center.clone().add(directionOfCamera.clone().multiplyScalar(50)) // prettier-ignore

      // Cross 50mm toward camera, aimed at camera
      const pointI = align(0).add(towardCamera50) // prettier-ignore
      const pointJ = align(1).add(towardCamera50) // prettier-ignore
      const pointK = align(2).add(towardCamera50) // prettier-ignore
      const pointL = align(3).add(towardCamera50) // prettier-ignore

      // Square at center
      addMovement(new Line( pointA, pointB, red, objectID)) // prettier-ignore
      addMovement(new Line( pointB, pointC, red, objectID)) // prettier-ignore
      addMovement(new Line( pointC, pointD, red, objectID)) // prettier-ignore
      addMovement(new Line( pointD, pointA, red, objectID)) // prettier-ignore

      // Square at 50mm toward camera
      addMovement(new Line( pointE, pointF, green, objectID)) // prettier-ignore
      addMovement(new Line( pointF, pointG, green, objectID)) // prettier-ignore
      addMovement(new Line( pointG, pointH, green, objectID)) // prettier-ignore
      addMovement(new Line( pointH, pointE, green, objectID)) // prettier-ignore

      // Cross 100mm toward camera
      addMovement(new Line( pointI, pointK, yellow, objectID)) // prettier-ignore
      addMovement(new Line( pointJ, pointL, yellow, objectID)) // prettier-ignore

      // Dot at center
      addMovement(new Line( center, center.clone().add(directionOfCamera.clone().multiplyScalar(1)), grey, objectID)) // prettier-ignore

      // A 50mm line from 100mm up (the center of the draw volume) aimed directly at the camera
      // addMovement(
      //   new Line(
      //     center.clone().add(directionOfCamera.clone().multiplyScalar(10)),
      //     center.clone().add(directionOfCamera.clone().multiplyScalar(60)),
      //     grey,
      //     objectID,
      //   ),
      // )

      // Add a 10mm line in the direction of the camera's quaternion for debugging
      const blenderCameraOrientation = new Quaternion(
        this.quaternion[0],
        this.quaternion[1],
        this.quaternion[2],
        this.quaternion[3],
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
    if (!this.quaternion) return

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
