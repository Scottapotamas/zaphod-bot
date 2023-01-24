import { TreeNodeInfo } from '@blueprintjs/core'
import { IconNames } from '@blueprintjs/icons'
import { Vector3 } from 'three'
import { NodeInfo, NodeTypes } from '../interface/RenderableTree'
import { SimpleColorMaterial } from './materials/Color'
import { ColorRampMaterial } from './materials/ColorRamp'
import {
  Movement,
  Line,
  MovementGroup,
  RGB,
  CatmullChain,
  RGBA,
  Point,
} from './movements'
import { getShouldSkip, getToMovementSettings, Settings } from './settings'

export interface GNodesVerticesVertex {
  id: string
  co: [number, number, number] // position
  color: [number, number, number, number] // Vertex color
}

export interface GNodesVerticesToMovementsSettings {
  // How long to wait at the particle's position before going bright.
  preWait?: number

  // How long to be on for.
  onDuration?: number

  // How long to wait at the particle's position after going bright.
  postWait?: number

  // Ramp up time, a percentage of time spent going from 0 to full opacity.
  // fullBrightnessBy?: number // 0 - 1

  // Ramp down time, a percentage of time spent going from 0 to full opacity.
  // fullBrightnessUntil?: number // 0 - 1
}

export class GNodesVertices {
  readonly type = 'gnodes_vertices'

  constructor(public name: string) {}

  private points: GNodesVerticesVertex[] = []

  public addPoint = (point: GNodesVerticesVertex) => {
    this.points.push(point)
  }

  public getObjectTree: () => TreeNodeInfo<NodeInfo> = () => {
    const node: TreeNodeInfo<NodeInfo> = {
      id: this.name,
      label: this.name,
      icon: IconNames.DRAW,
      nodeData: {
        type: NodeTypes.GNODES_VERTICES,
      },
    }

    return node
  }

  public getOriginalMaterialJSON = (objectID: string) => {
    for (const point of this.points) {
      if (point.id === objectID) {
      }
    }

    return null
  }

  public toMovements = (settings: Settings) => {
    const movements: Movement[] = []

    const objectID = this.name
    const overrideKeys = [this.name]

    if (getShouldSkip(settings, overrideKeys)) {
      return movements
    }

    const settingsWithOverride = getToMovementSettings(
      settings,
      'gnodesVertices',
      overrideKeys,
    )

    for (let index = 0; index < this.points.length; index++) {
      const p = this.points[index]

      // Convert the location to a Vector3
      const location = new Vector3(p.co[0], p.co[1], p.co[2])

      const duration =
        (settingsWithOverride.preWait ?? 0) +
        (settingsWithOverride.onDuration ?? 0) +
        (settingsWithOverride.postWait ?? 0)

      const mat = new SimpleColorMaterial(p.color)

      const point = new Point(location, duration, mat, objectID, overrideKeys)

      // This ID is guaranteed to be stable
      point.interFrameID = p.id

      movements.push(point)
    }

    return movements
  }
}

export interface GNodesVerticesJSON {
  type: 'gn_vertices'
  frame: number
  name: string
  points: GNodesVerticesVertex[]
}

export function importGNodesVertices(json: GNodesVerticesJSON) {
  const gnodesvertices = new GNodesVertices(json.name)

  for (const point of json.points) {
    gnodesvertices.addPoint(point)
  }

  return gnodesvertices
}
