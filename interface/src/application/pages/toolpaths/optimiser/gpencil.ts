import { TreeNodeInfo } from '@blueprintjs/core'
import { IconNames } from '@blueprintjs/icons'
import { Line3, Vector3 } from 'three'
import { NodeInfo, NodeTypes } from '../interface/RenderableTree'
import { ObjectNameTree } from './files'
import { importMaterial, MaterialJSON } from './material'
import { isSimpleColorMaterial, SimpleColorMaterial } from './materials/Color'
import { ColorRampMaterial } from './materials/ColorRamp'
import { lerpRGBA } from './materials/utilities'
import {
  Point,
  Movement,
  Line,
  MovementGroup,
  RGB,
  CatmullChain,
  RGBA,
} from './movements'
import { getShouldSkip, getToMovementSettings } from './settings'
import type { Settings } from './settings'
import { InvisibleMaterialDefaultJSON } from './materials/Invisible'

export class GPencilLayer {
  public strokes: GPencilStroke[] = []
  public info: string

  constructor(info: string) {
    this.info = info
  }

  public addStroke = (stroke: GPencilStroke) => {
    this.strokes.push(stroke)
  }
}

export class GPencilStroke {
  public material: MaterialJSON
  public useCyclic: boolean
  public points: GPencilStrokePoint[] = []
  public id: string

  constructor(id: string, material: MaterialJSON | null, useCyclic: boolean) {
    this.id = id
    this.material = material ?? InvisibleMaterialDefaultJSON
    this.useCyclic = useCyclic
  }

  public addPoint = (point: GPencilStrokePoint) => {
    this.points.push(point)
  }
}

export interface GPencilStrokePoint {
  id: string
  co: [number, number, number] // position
  pressure: number // Pressure of tablet at point when drawing it
  strength: number // Color intensity (alpha factor)
  vertexColor: [number, number, number, number] // Vertex color
}

export enum GPencilOutputType {
  LINES = 0,
  LINE_GROUP = 1,
  CATMULL_CHAIN = 2,
}

export interface GPencilToMovementsSettings {
  /**
   * Determine the type of output
   */
  outputType?: GPencilOutputType
  /**
   * Simplification tolerance
   */
  simplificationTolerance?: number
}

export class GPencil {
  readonly type = 'gpencil'

  constructor(public name: string) {}

  private layers: GPencilLayer[] = []

  public addLayer = (layer: GPencilLayer) => {
    this.layers.push(layer)
  }

  public getObjectTree: () => TreeNodeInfo<NodeInfo> = () => {
    const node: TreeNodeInfo<NodeInfo> = {
      id: this.name,
      label: this.name,
      icon: IconNames.DRAW,
      nodeData: {
        type: NodeTypes.GPENCIL,
      },
      childNodes: this.layers.map(layer => ({
        id: `${this.name}-${layer.info}`,
        icon: IconNames.LAYERS,
        label: layer.info,
        nodeData: {
          type: NodeTypes.GPENCIL_LAYER,
          hidden: false,
        },
      })),
    }

    return node
  }

  public getOriginalMaterialJSON = (objectID: string) => {
    for (const layer of this.layers) {
      const layerID = `${this.name}-${layer.info}`

      if (objectID.startsWith(layerID)) {
        for (const stroke of layer.strokes) {
          if (stroke.id === objectID) {
            return stroke.material
          }
        }
      }
    }

    return null
  }

  public toMovements = (settings: Settings) => {
    const movements: Movement[] = []

    for (const layer of this.layers) {
      const objectID = `${this.name}-${layer.info}`
      const overrideKeys = [this.name, objectID]

      if (getShouldSkip(settings, overrideKeys)) {
        continue
      }

      const settingsWithOverride = getToMovementSettings(settings, 'gpencil', overrideKeys)

      for (const stroke of layer.strokes) {
        // A stroke needs at least two points to form a line
        if (stroke.points.length < 2) {
          continue
        }

        const simplified = simplify(stroke.points, settingsWithOverride.simplificationTolerance ?? 0)

        const material = importMaterial(stroke.material)

        if (settingsWithOverride.outputType === GPencilOutputType.CATMULL_CHAIN && simplified.length >= 4) {
          const points = [
            simplified[0], // duplicate first
            ...simplified,
            simplified[simplified.length - 1], // and last points
          ]

          const movement = new CatmullChain(
            points.map(gPencilPoint => new Vector3(gPencilPoint.co[0], gPencilPoint.co[1], gPencilPoint.co[2])),
            material,
            objectID,
            overrideKeys,
          )
          movement.interFrameID = `${simplified[0].id}->${simplified[simplified.length - 1].id}`

          movements.push(movement)
          continue
        }

        let lastPoint = new Vector3(simplified[0].co[0], simplified[0].co[1], simplified[0].co[2])

        const orderedMovements = new MovementGroup()

        let doVertexColoring = isSimpleColorMaterial(material)

        let previousPointBlendedColor = doVertexColoring
          ? lerpRGBA(
              (material as SimpleColorMaterial).color,
              [
                simplified[0].vertexColor[0],
                simplified[0].vertexColor[1],
                simplified[0].vertexColor[2],
                simplified[0].vertexColor[3] * simplified[0].strength,
              ],
              simplified[0].vertexColor[3],
            )
          : ([0, 0, 0, 0] as RGBA)

        // Start at the second point, the first is located above
        for (let index = 1; index < simplified.length; index++) {
          const point = simplified[index]
          const co = point.co

          let currentPoint = new Vector3(co[0], co[1], co[2])

          let vertexMat = material

          if (doVertexColoring) {
            // Lerp the base color to the vertex colour based on the vertex color alpha. The vertex color alpha is multiplied by the strength
            const currentPointBlendedColor = lerpRGBA(
              (material as SimpleColorMaterial).color,
              [point.vertexColor[0], point.vertexColor[1], point.vertexColor[2], point.vertexColor[3] * point.strength],
              point.vertexColor[3], // Multiply the vertex by the strength to get a transparency effect
            )

            vertexMat = new ColorRampMaterial(previousPointBlendedColor, currentPointBlendedColor)

            previousPointBlendedColor = currentPointBlendedColor
          }

          // Create a line from the lastPoint to the currentPoint
          const line: Movement = new Line(lastPoint, currentPoint, vertexMat, objectID, overrideKeys)

          // This ID isn't guaranteed to be stable, but it'll probably be close at least some of the time
          line.interFrameID = point.id

          orderedMovements.addMovement(line)

          lastPoint = currentPoint
        }

        if (settingsWithOverride.outputType === GPencilOutputType.LINES) {
          movements.push(...orderedMovements.getMovements())
        } else {
          movements.push(orderedMovements)
        }
      }
    }

    return movements
  }
}

export interface GPencilJSON {
  type: 'gpencil'
  frame: number
  name: string
  layers: {
    info: string
    strokes: {
      id: string
      material: MaterialJSON
      useCyclic: boolean
      points: {
        id: string
        co: [number, number, number]
        pressure: number
        strength: number
        vertexColor: [number, number, number, number]
      }[]
    }[]
  }[]
}
export function importGPencil(json: GPencilJSON) {
  const gPencil = new GPencil(json.name)

  for (const jLayer of json.layers) {
    let layer = new GPencilLayer(jLayer.info)
    gPencil.addLayer(layer)

    for (const jStroke of jLayer.strokes) {
      const stroke = new GPencilStroke(jStroke.id, jStroke.material, jStroke.useCyclic)
      layer.addStroke(stroke)

      for (const jPoint of jStroke.points) {
        stroke.addPoint(jPoint)
      }
    }
  }

  return gPencil
}

/**
 * An efficiently packed array of bits
 */
class BitField {
  private internal: Uint32Array

  constructor(num: number) {
    this.internal = new Uint32Array(Math.ceil(num / 32))
  }

  public get(i: number) {
    const index = Math.floor(i / 32)
    const bit = i % 32
    return (this.internal[index] & (1 << bit)) !== 0
  }

  public set(i: number, val: boolean) {
    const index = Math.floor(i / 32)
    const bit = i % 32

    if (val) {
      this.internal[index] |= 1 << bit
    } else {
      this.internal[index] &= ~(1 << bit)
    }
  }
}

/**
 * Simplifies a gpencil stroke using the Ramer–Douglas–Peucker algorithm
 *
 * https://en.wikipedia.org/wiki/Ramer%E2%80%93Douglas%E2%80%93Peucker_algorithm
 */
function simplify(points: GPencilStrokePoint[], tolerance: number) {
  if (points.length <= 2) return points
  if (tolerance === 0) return points

  const closestPoint = new Vector3()
  const start = new Vector3()
  const end = new Vector3() // all the maths is done on these points to reduce allocations
  const p = new Vector3()

  const keep = new BitField(points.length)
  const segment = new Line3()

  let first = 0
  let last = points.length - 1

  // Index array
  const stack: number[] = []

  // Keep both ends
  keep.set(first, true)
  keep.set(last, true)

  while (true) {
    let distanceMax = -Infinity
    let indexMax = 0

    // Find the furthest away point
    for (let i = first + 1; i < last; i++) {
      start.set(points[first].co[0], points[first].co[1], points[first].co[2])
      end.set(points[last].co[0], points[last].co[1], points[last].co[2])
      p.set(points[i].co[0], points[i].co[1], points[i].co[2])

      // Calculate distance to line segment
      segment.set(start, end)

      segment.closestPointToPoint(p, true, closestPoint)

      const distance = closestPoint.distanceToSquared(p)

      if (distance > distanceMax) {
        indexMax = i
        distanceMax = distance
      }
    }

    // If it's bigger than the tolerance, keep it
    if (distanceMax >= tolerance) {
      keep.set(indexMax, true)
      stack.push(first, indexMax, indexMax, last)
    }

    // Prepare for next iteration
    const newLast = stack.pop()
    const newFirst = stack.pop()

    if (newLast === undefined || newFirst === undefined) {
      break
    }

    last = newLast
    first = newFirst
  }

  // Run through the array and pick out the points we want
  const newPoints: GPencilStrokePoint[] = []
  for (let index = 0; index < points.length; index++) {
    const point = points[index]
    if (keep.get(index)) {
      newPoints.push(point)
    }
  }
  return newPoints
}
