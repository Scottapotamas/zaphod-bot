import { TreeNodeInfo } from '@blueprintjs/core'
import { IconNames } from '@blueprintjs/icons'
import { Line3, Vector3 } from 'three'
import { NodeInfo, NodeTypes } from '../interface/RenderableTree'
import { ObjectNameTree } from './files'
import { importMaterial, MaterialJSON } from './material'
import { isSimpleColorMaterial, SimpleColorMaterial } from './materials/Color'
import { ColorRampMaterial } from './materials/ColorRamp'
import { lerpRGBA } from './materials/utilities'
import { Point, Movement, Line, MovementGroup, RGB, CatmullChain, RGBA } from './movements'
import { getShouldSkip, getToMovementSettings, Settings } from './settings'
import { InvisibleMaterialDefaultJSON } from './materials/Invisible'

export interface GNodesMeshVertex {
  id: string
  co: [number, number, number] // position
  color: [number, number, number, number] // Vertex color
}

export interface GNodesMeshEdge {
  edge_index: number
  points: GNodesMeshVertex[]
}

export enum GNodesMeshOutputType {
  LINES = 0,
  LINE_GROUP = 1,
}

export interface GNodesMeshToMovementsSettings {
  /**
   * Determine the type of output
   */
  outputType?: GNodesMeshOutputType
}

export class GNodesMesh {
  readonly type = 'gnodes_mesh'

  constructor(public name: string) {}

  private edges: GNodesMeshEdge[] = []

  public addEdge = (edge: GNodesMeshEdge) => {
    this.edges.push(edge)
  }

  public getObjectTree: () => TreeNodeInfo<NodeInfo> = () => {
    const node: TreeNodeInfo<NodeInfo> = {
      id: this.name,
      label: this.name,
      icon: IconNames.DRAW,
      nodeData: {
        type: NodeTypes.GNODES_MESH,
      },
    }

    return node
  }

  public getOriginalMaterialJSON = (objectID: string) => {
    for (const edge of this.edges) {
      for (const point of edge.points) {
        if (point.id === objectID) {
        }
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

    const settingsWithOverride = getToMovementSettings(settings, 'gnodesMesh', overrideKeys)

    for (const edge of this.edges) {
      // A stroke needs at least two points to form a line
      if (edge.points.length < 2) {
        continue
      }

      let lastPoint = new Vector3(edge.points[0].co[0], edge.points[0].co[1], edge.points[0].co[2])

      let previousPointBlendedColor = edge.points[0].color

      const orderedMovements = new MovementGroup()

      // Start at the second point, the first is located above
      for (let index = 1; index < edge.points.length; index++) {
        const point = edge.points[index]
        const co = point.co

        let currentPoint = new Vector3(co[0], co[1], co[2])
        const vertexMat = new ColorRampMaterial(previousPointBlendedColor, point.color)
        previousPointBlendedColor = point.color

        // Create a line from the lastPoint to the currentPoint
        const line: Movement = new Line(lastPoint, currentPoint, vertexMat, objectID, overrideKeys)

        // This ID isn't guaranteed to be stable, but it'll probably be close at least some of the time
        line.interFrameID = point.id

        orderedMovements.addMovement(line)

        lastPoint = currentPoint
      }

      if (settingsWithOverride.outputType === GNodesMeshOutputType.LINES) {
        movements.push(...orderedMovements.getMovements())
      } else {
        movements.push(orderedMovements)
      }
    }

    return movements
  }
}

export interface GNodesMeshJSON {
  type: 'gn_mesh'
  frame: number
  name: string
  edges: GNodesMeshEdge[]
}

export function importGNodesMesh(json: GNodesMeshJSON) {
  const gnodesmesh = new GNodesMesh(json.name)

  for (const edge of json.edges) {
    gnodesmesh.addEdge(edge)
  }

  return gnodesmesh
}
