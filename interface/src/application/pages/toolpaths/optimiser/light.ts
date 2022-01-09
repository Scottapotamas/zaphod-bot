import { TreeNodeInfo } from '@blueprintjs/core'
import { IconNames } from '@blueprintjs/icons'
import { Color, Material, Vector3 } from 'three'
import { NodeInfo, NodeTypes } from '../interface/RenderableTree'
import { ObjectNameTree } from './files'
import { importMaterial, MaterialJSON } from './material'
import { isSimpleColorMaterial } from './materials/Color'
import { Point, Line, Movement, MovementGroup } from './movements'
import { getShouldSkip, getToMovementSettings, Settings } from './settings'

export interface LightToMovementsSettings {
  // How long to wait at the light before moving on.
  stopDelay?: number

  /**
   * If the colour of the light is black, don't render it at all.
   *
   * Note this takes precidence even over material overrides, due to pipeline dependency requirements.
   */
  hideIfBlack?: boolean
}

export class Light {
  readonly type = 'light'

  constructor(
    public name: string,
    public material: MaterialJSON,
    public position: [number, number, number],
  ) {}

  public getObjectTree: () => TreeNodeInfo<NodeInfo> = () => {
    const node: TreeNodeInfo<NodeInfo> = {
      id: this.name,
      label: this.name,
      icon: IconNames.LIGHTBULB,
      nodeData: {
        type: NodeTypes.LIGHT,
      },
    }

    return node
  }

  public toMovements = (settings: Settings) => {
    const movements: Movement[] = []

    const objectID = this.name
    const overrideKeys = [this.name]

    if (getShouldSkip(settings, overrideKeys)) {
      return movements
    }

    const mat = importMaterial(this.material)
    if (
      isSimpleColorMaterial(mat) &&
      settings.objectSettings.light.hideIfBlack &&
      mat.color[0] === 0 &&
      mat.color[1] === 0 &&
      mat.color[2] === 0
    ) {
      // If the original material is black, hide this
      return movements
    }

    const settingsWithOverride = getToMovementSettings(settings, 'light', [
      this.name,
      objectID,
    ])

    // Convert the position to a Vector3
    const position = new Vector3(
      this.position[0],
      this.position[1],
      this.position[2],
    )

    const point = new Point(
      position,
      settingsWithOverride.stopDelay ?? 0,
      mat,
      objectID,
    )

    // This ID is guaranteed to be stable
    point.interFrameID = this.name

    movements.push(point)

    return movements
  }
}

export interface LightJSON {
  type: 'light'
  frame: number
  name: string
  material: MaterialJSON
  position: [number, number, number]
}

export function importLight(json: LightJSON) {
  const particles = new Light(json.name, json.material, json.position)

  return particles
}
