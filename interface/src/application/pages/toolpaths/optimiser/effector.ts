import { TreeNodeInfo } from '@blueprintjs/core'
import { IconNames } from '@blueprintjs/icons'
import { Quaternion, Vector3 } from 'three'
import { NodeInfo, NodeTypes } from '../interface/RenderableTree'
import { importMaterial, MaterialJSON } from './material'
import { InvisibleMaterialJSON } from './materials/Invisible'
import { MATERIALS } from './materials/utilities'
import { Point, Line, Movement, MovementGroup } from './movements'
import { getShouldSkip, getToMovementSettings, Settings } from './settings'

export interface EffectorToMovementSettings {
  // How long to wait at the effector's position before executing the movement.
  preWait?: number

  // How long to wait at the effector's position after executing the movement.
  postWait?: number
}

export type EffectorDisplayType = 'SINGLE_ARROW'
export type EffectorCenter = 'center' | 'start' | 'end'

export class Effector {
  readonly type = 'effector'

  // By default, always be an invisible material
  material: InvisibleMaterialJSON = {
    type: MATERIALS.INVISIBLE,
    color: [0.3, 0.1, 0.1, 1],
  }

  constructor(
    public name: string,
    public enabled: boolean,
    public position: [number, number, number],
    public quaternion: [number, number, number, number],
    public display_size: number,
    public display_type: EffectorDisplayType,
    public center: EffectorCenter,
  ) {}

  public getObjectTree: () => TreeNodeInfo<NodeInfo> = () => {
    const node: TreeNodeInfo<NodeInfo> = {
      id: this.name,
      label: this.name,
      icon: IconNames.DRAW,
      nodeData: {
        type: NodeTypes.EFFECTOR,
      },
    }

    return node
  }

  public getOriginalMaterialJSON = (objectID: string) => {
    if (this.name === objectID) {
      return this.material
    }

    return null
  }

  public toMovements = (settings: Settings) => {
    const objectID = this.name
    const overrideKeys = [this.name, objectID]

    if (getShouldSkip(settings, overrideKeys)) {
      return []
    }

    if (!this.enabled) {
      return []
    }

    const mat = importMaterial(this.material)

    const settingsWithOverride = getToMovementSettings(settings, 'effector', overrideKeys)

    // Depending on the 'center' value, calculate the start and the end
    let start = new Vector3(this.position[0], this.position[1], this.position[2])
    let end = new Vector3(this.position[0], this.position[1], this.position[2])
    const directionVector = new Vector3(0, 1, 0).applyQuaternion(
      new Quaternion(this.quaternion[0], this.quaternion[1], this.quaternion[2], this.quaternion[3]),
    )

    if (this.center === 'start') {
      // the position is the start, move 'size' amount in the direction for the end
      end = end.add(directionVector.clone().multiplyScalar(this.display_size))
    } else if (this.center === 'end') {
      // the position is the end, move 'size' amount in the negative direction for the end
      start = start.add(directionVector.clone().multiplyScalar(-this.display_size))
    } else if (this.center === 'center') {
      // the position is the center, move 1/2 'size' amount forward for start, 1/2 'size' amount backward for the end
      start = start.add(directionVector.clone().multiplyScalar(this.display_size / 2))
      end = end.add(directionVector.clone().multiplyScalar(-this.display_size / 2))
    }

    const orderedMovements = new MovementGroup()

    // Add the trigger

    if (settingsWithOverride.preWait && settingsWithOverride.preWait > 0) {
      const startMovement = new Point(start, settingsWithOverride.preWait ?? 0, mat, objectID, overrideKeys)
      startMovement.interFrameID = `${this.name}-pre`
      orderedMovements.addMovement(startMovement)
    }

    const move = new Line(start, end, mat, objectID, overrideKeys)
    move.interFrameID = this.name
    orderedMovements.addMovement(move)

    // Add the trigger

    if (settingsWithOverride.postWait && settingsWithOverride.postWait > 0) {
      const endMovement = new Point(end, settingsWithOverride.postWait ?? 0, mat, objectID, overrideKeys)
      endMovement.interFrameID = `${this.name}-post`
      orderedMovements.addMovement(endMovement)
    }

    // Return the group
    return [orderedMovements]
  }
}

export interface EffectorJSON {
  type: 'effector'
  frame: number
  name: string
  position: [number, number, number]
  quaternion: [number, number, number, number]
  display_size: number // mm
  display_type: EffectorDisplayType
  center: EffectorCenter
  enabled: boolean
}

export function importEffector(json: EffectorJSON) {
  const particles = new Effector(
    json.name,
    json.enabled,
    json.position,
    json.quaternion,
    json.display_size,
    json.display_type,
    json.center,
  )

  return particles
}

// {
//   "type": "effector",
//   "frame": 1,
//   "position": [0.0, 0.0, 53.333334],
//   "quaternion": [-0.103893, 0.183962, -0.028817, 0.977003],
//   "display_size": 56.99999928474426,
//   "display_type": "SINGLE_ARROW"
// }
