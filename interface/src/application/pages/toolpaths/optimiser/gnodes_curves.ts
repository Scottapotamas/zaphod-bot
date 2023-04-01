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
  Bezier,
  ConstantSpeedBezier,
} from './movements'
import { getShouldSkip, getToMovementSettings, Settings } from './settings'

export type GNodesCurvesSpline =
  | {
      type: BlenderCurveType.CATMULL_ROM
      cyclic: boolean
      points: GNodesCatmullRomPoint[]
    }
  | {
      type: BlenderCurveType.POLY
      cyclic: boolean
      points: GNodesPolyLinePoint[]
    }
  | {
      type: BlenderCurveType.BEZIER
      cyclic: boolean
      points: GNodesBezierPoint[]
    }
  | {
      type: BlenderCurveType.NURBS
      cyclic: boolean
      points: GNodesNurbsPoint[]
    }

export type GNodesCatmullRomPoint = {
  co: [number, number, number] // position
  color: [number, number, number, number] // Vertex color
}

export type GNodesPolyLinePoint = {
  co: [number, number, number] // position
  color: [number, number, number, number] // Vertex color
}

export type GNodesBezierPoint = {
  co: [number, number, number] // position
  color: [number, number, number, number] // Vertex color

  handle_left: [number, number, number] // position
  handle_right: [number, number, number] // position
  handle_type_left: BlenderBezierHandleType
  handle_type_right: BlenderBezierHandleType
}

export type GNodesNurbsPoint = {
  co: [number, number, number] // position
  color: [number, number, number, number] // Vertex color
}

export enum BlenderCurveType {
  CATMULL_ROM = 0,
  POLY = 1,
  BEZIER = 2,
  NURBS = 3,
}

export enum BlenderBezierHandleType {
  BEZIER_HANDLE_FREE = 0,
  BEZIER_HANDLE_AUTO = 1,
  BEZIER_HANDLE_VECTOR = 2,
  BEZIER_HANDLE_ALIGN = 3,
}

export enum GNodesCurvesOutputType {
  BARE_MOVEMENTS = 0,
  GROUP_PER_SPLINE = 1,
}

export interface GNodesCurvesToMovementsSettings {
  /**
   * Determine the type of output
   */
  outputType?: GNodesCurvesOutputType
}

export class GNodesCurves {
  readonly type = 'gnodes_vertices'

  constructor(public name: string) {}

  private splines: GNodesCurvesSpline[] = []

  public addSpline = (spline: GNodesCurvesSpline) => {
    this.splines.push(spline)
  }

  public getObjectTree: () => TreeNodeInfo<NodeInfo> = () => {
    const node: TreeNodeInfo<NodeInfo> = {
      id: this.name,
      label: this.name,
      icon: IconNames.WIND,
      nodeData: {
        type: NodeTypes.GNODES_CURVES,
      },
    }

    return node
  }

  public getOriginalMaterialJSON = (objectID: string) => {
    // for (const point of this.splines) {
    //   if (point.id === objectID) {
    //   }
    // }

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
      'gnodesCurves',
      overrideKeys,
    )

    let i = 0
    for (const spline of this.splines) {
      let orderedMovements: MovementGroup
      switch (spline.type) {
        case BlenderCurveType.POLY:
          orderedMovements = polySplineToMovementGroup(
            spline,
            objectID,
            overrideKeys,
            i,
            settings,
          )
          break
        case BlenderCurveType.BEZIER:
          orderedMovements = bezierSplineToMovementGroup(
            spline,
            objectID,
            overrideKeys,
            i,
            settings,
          )
          break

        default:
          throw new Error(`unimplemented curve type ${spline.type}`)
      }

      if (
        settingsWithOverride.outputType ===
        GNodesCurvesOutputType.BARE_MOVEMENTS
      ) {
        movements.push(...orderedMovements.getMovements())
      } else {
        movements.push(orderedMovements)
      }
      i++
    }

    return movements
  }
}

export interface GNodesCurvesJSON {
  type: 'gn_curves'
  frame: number
  name: string
  splines: GNodesCurvesSpline[]
}

export function importGNodesCurves(json: GNodesCurvesJSON) {
  const gnodesCurves = new GNodesCurves(json.name)

  for (const spline of json.splines) {
    gnodesCurves.addSpline(spline)
  }

  return gnodesCurves
}

function polySplineToMovementGroup(
  spline: GNodesCurvesSpline,
  objectID: string,
  overrideKeys: string[],
  splineIndex: number,
  settings: Settings,
) {
  const points = spline.points as GNodesPolyLinePoint[]

  const orderedMovements = new MovementGroup()
  orderedMovements.interFrameID = `${objectID}-${splineIndex}`

  // for (const edge of this.edges) {
  //   // A stroke needs at least two points to form a line
  //   if (edge.points.length < 2) {
  //     continue
  //   }

  //   let lastPoint = new Vector3(
  //     edge.points[0].co[0],
  //     edge.points[0].co[1],
  //     edge.points[0].co[2],
  //   )

  //   let previousPointBlendedColor = edge.points[0].color

  //   // Start at the second point, the first is located above
  //   for (let index = 1; index < edge.points.length; index++) {
  //     const point = edge.points[index]
  //     const co = point.co

  //     let currentPoint = new Vector3(co[0], co[1], co[2])
  //     const vertexMat = new ColorRampMaterial(
  //       previousPointBlendedColor,
  //       point.color,
  //     )
  //     previousPointBlendedColor = point.color

  //     // Create a line from the lastPoint to the currentPoint
  //     const line: Movement = new Line(
  //       lastPoint,
  //       currentPoint,
  //       vertexMat,
  //       objectID,
  //       overrideKeys,
  //     )

  //     // This ID isn't guaranteed to be stable, but it'll probably be close at least some of the time
  //     line.interFrameID = point.id

  //     orderedMovements.addMovement(line)

  //     lastPoint = currentPoint
  //   }
  // }

  return orderedMovements
}

function bezierSplineToMovementGroup(
  spline: GNodesCurvesSpline,
  objectID: string,
  overrideKeys: string[],
  splineIndex: number,
  settings: Settings,
) {
  const points = spline.points as GNodesBezierPoint[]

  const movementGroup = new MovementGroup()
  movementGroup.interFrameID = `${objectID}-${splineIndex}`

  for (const [leftPoint, rightPoint] of window(points, 2)) {
    const c0 = new Vector3(...leftPoint.co)
    const c1 = new Vector3(...leftPoint.handle_right)
    const c2 = new Vector3(...rightPoint.handle_left)
    const c3 = new Vector3(...rightPoint.co)

    const bezier = new ConstantSpeedBezier(
      c0,
      c1,
      c2,
      c3,
      new ColorRampMaterial(leftPoint.color, rightPoint.color),
      objectID,
    )

    movementGroup.addMovement(bezier)
  }

  return movementGroup
}

function* window<T>(inputArray: T[], size: number) {
  for (let index = 0; index + size <= inputArray.length; index++) {
    yield inputArray.slice(index, index + size)
  }
}
