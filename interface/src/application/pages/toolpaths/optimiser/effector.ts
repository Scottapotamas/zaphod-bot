import { TreeNodeInfo } from '@blueprintjs/core'
import { IconNames } from '@blueprintjs/icons'
import { Quaternion, Vector3 } from 'three'
import { NodeInfo, NodeTypes } from '../interface/RenderableTree'
import { importMaterial, MaterialJSON } from './material'
import { InvisibleMaterialJSON } from './materials/Invisible'
import { MATERIALS } from './materials/utilities'
import { Point, Line, Movement, MovementGroup, MILLISECONDS_IN_SECOND } from './movements'
import { getShouldSkip, getToMovementSettings, Settings } from './settings'
import { TriggerAlignment, TriggerCall, TriggerCallDMX, TriggerType } from './triggers'

export interface EffectorToMovementSettings {
  // How long to wait at the effector's position before executing the movement.
  preWait?: number

  // How long to wait at the effector's position after executing the movement.
  postWait?: number

  // A scaler for the DMX lighting value
  dmxValueScaler?: number
}

export type EffectorDisplayType = 'SINGLE_ARROW'
export type EffectorAlign = 'center' | 'start' | 'end'

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
    public align: EffectorAlign,
    public duration_override: number,
    public dmx_val: number,
  ) {}

  public getObjectTree: () => TreeNodeInfo<NodeInfo> = () => {
    const node: TreeNodeInfo<NodeInfo> = {
      id: this.name,
      label: this.name,
      icon: IconNames.DOT,
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
    const directionVector = new Vector3(0, 0, 1).applyQuaternion(
      new Quaternion(this.quaternion[0], this.quaternion[1], this.quaternion[2], this.quaternion[3]),
    )

    if (this.align === 'start') {
      // the position is the start, move 'size' amount in the direction for the end
      end = end.add(directionVector.clone().multiplyScalar(this.display_size))
    } else if (this.align === 'end') {
      // the position is the end, move 'size' amount in the negative direction for the end
      start = start.add(directionVector.clone().multiplyScalar(-this.display_size))
    } else if (this.align === 'center') {
      // the position is the center, move 1/2 'size' amount forward for start, 1/2 'size' amount backward for the end
      start = start.add(directionVector.clone().multiplyScalar(this.display_size / 2))
      end = end.add(directionVector.clone().multiplyScalar(-this.display_size / 2))
    }

    const orderedMovements = new MovementGroup()
    // Freeze internal movement ordering
    orderedMovements.frozen = true

    // Add the trigger

    if (settingsWithOverride.preWait && settingsWithOverride.preWait > 0) {
      const startMovement = new Point(start, settingsWithOverride.preWait ?? 0, mat, objectID, overrideKeys)
      startMovement.interFrameID = `${this.name}-pre`
      orderedMovements.addMovement(startMovement)
    }

    const move = new Line(start, end, mat, objectID, overrideKeys)
    move.interFrameID = this.name
    orderedMovements.addMovement(move)

    const scalingFactor = settings.objectSettings.effector.dmxValueScaler ?? 1

    const preLightLevels: TriggerCallDMX = {
      type: TriggerType.DMX,
      align: TriggerAlignment.START,
      args: {
        level: this.dmx_val * scalingFactor,
      },
    }
    const postLightLevels: TriggerCallDMX = {
      type: TriggerType.DMX,
      align: TriggerAlignment.END,
      args: {
        level: 0,
      },
    }

    move.triggers.push(preLightLevels)
    move.triggers.push(postLightLevels)

    const distance = start.distanceTo(end)

    if (this.duration_override && Number.isFinite(this.duration_override) && this.duration_override > 0) {
      let proposedSpeed = distance / (this.duration_override / MILLISECONDS_IN_SECOND)

      const OVERRIDE_SPEED_LIMIT = 500

      // This is a sanity check at this level, the duration will never exceed the max speed anyway.
      if (proposedSpeed > OVERRIDE_SPEED_LIMIT) {
        console.warn(
          `Effector movement suggested duration override of ${this.duration_override}ms and a distance of ${distance}mm, giving a speed of ${proposedSpeed}mm/s which is above the limit of OVERRIDE_SPEED_LIMIT`,
        )
        proposedSpeed = OVERRIDE_SPEED_LIMIT
      }

      // Lock the speed
      move.lockSpeed = proposedSpeed
    }

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
  align: EffectorAlign
  enabled: boolean
  duration: number // ms
  dmx_val: number // 0 - 100 DMX lighting value
}

export function importEffector(json: EffectorJSON) {
  const particles = new Effector(
    json.name,
    json.enabled,
    json.position,
    json.quaternion,
    json.display_size,
    json.display_type,
    json.align,
    json.duration,
    json.dmx_val,
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
