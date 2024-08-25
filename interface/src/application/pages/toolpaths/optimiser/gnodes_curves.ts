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
import { CancellationToken } from '@electricui/async-utilities'
import path, { resolve } from 'path'
import { PNG } from 'pngjs'
import { readFile } from 'fs/promises'
import { TextureStripMaterial } from './materials/TextureStrip'

export type GNodesCurvesSpline =
  | {
      type: BlenderCurveType.CATMULL_ROM
      cyclic: boolean
      points: GNodesCatmullRomPoint[]
      texture_file?: string
    }
  | {
      type: BlenderCurveType.POLY
      cyclic: boolean
      points: GNodesPolyLinePoint[]
      texture_file?: string
    }
  | {
      type: BlenderCurveType.BEZIER
      cyclic: boolean
      points: GNodesBezierPoint[]
      texture_file?: string
    }
  | {
      type: BlenderCurveType.NURBS
      cyclic: boolean
      points: GNodesNurbsPoint[]
      texture_file?: string
    }

export type GNodesCatmullRomPoint = {
  co: [number, number, number] // position
  color: [number, number, number, number] // Vertex color
  uv?: [number, number] // UV texture coordinate
}

export type GNodesPolyLinePoint = {
  co: [number, number, number] // position
  color: [number, number, number, number] // Vertex color
  uv?: [number, number] // UV texture coordinate
}

export type GNodesBezierPoint = {
  co: [number, number, number] // position
  color: [number, number, number, number] // Vertex color

  handle_left: [number, number, number] // position
  handle_right: [number, number, number] // position
  handle_type_left: BlenderBezierHandleType
  handle_type_right: BlenderBezierHandleType
  uv?: [number, number] // UV texture coordinate
}

export type GNodesNurbsPoint = {
  co: [number, number, number] // position
  color: [number, number, number, number] // Vertex color
  uv?: [number, number] // UV texture coordinate
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

  constructor(
    private filepath: string,
    public name: string,
  ) {}

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

  public matchesObjectID = (objectID: string) => {
    return this.name === objectID
  }

  public toMovements = async (settings: Settings, buildMaterials: boolean) => {
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
      let textureReader: TextureReader | null = null

      // Load the texture file if it exists
      if (spline.texture_file && buildMaterials) {
        textureReader = new TextureReader(
          path.join(path.dirname(this.filepath), spline.texture_file),
        )
        await textureReader.initialize()
      }

      let orderedMovements: MovementGroup
      switch (spline.type) {
        case BlenderCurveType.POLY:
          orderedMovements = polySplineToMovementGroup(
            spline,
            objectID,
            overrideKeys,
            i,
            textureReader,
            settings,
            buildMaterials,
          )
          break
        case BlenderCurveType.BEZIER:
          orderedMovements = bezierSplineToMovementGroup(
            spline,
            objectID,
            overrideKeys,
            i,
            textureReader,
            settings,
            buildMaterials,
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

export function importGNodesCurves(filepath: string, json: GNodesCurvesJSON) {
  const gnodesCurves = new GNodesCurves(filepath, json.name)

  for (const spline of json.splines) {
    gnodesCurves.addSpline(spline)
  }

  return gnodesCurves
}

const materialDefault = new SimpleColorMaterial([1, 1, 1, 1])

function polySplineToMovementGroup(
  spline: GNodesCurvesSpline,
  objectID: string,
  overrideKeys: string[],
  splineIndex: number,
  textureReader: TextureReader | null,
  settings: Settings,
  buildMaterials: boolean,
) {
  let points = spline.points as GNodesPolyLinePoint[]

  const orderedMovements = new MovementGroup()
  orderedMovements.interFrameID = `${objectID}-${splineIndex}`
  orderedMovements.frozen = true // We're sure that these are connected, so no need to optimise within a group

  if (spline.cyclic && points.length > 0) {
    points = points.slice()
    points.push(points[0]) // add the first point to the end
  }

  for (let index = 1; index < points.length; index++) {
    const prev = points[index - 1]
    const curr = points[index]

    const prevPos = new Vector3(prev.co[0], prev.co[1], prev.co[2])
    const currPos = new Vector3(curr.co[0], curr.co[1], curr.co[2])

    // Create a line from the lastPoint to the currentPoint
    const line: Movement = new Line(
      prevPos,
      currPos,
      buildMaterials
        ? buildMaterial(prev, curr, textureReader)
        : materialDefault,
      objectID,
      overrideKeys,
    )
    line.interFrameID = `${objectID}-${splineIndex}-${index}`
    orderedMovements.addMovement(line)
  }

  return orderedMovements
}

function bezierSplineToMovementGroup(
  spline: GNodesCurvesSpline,
  objectID: string,
  overrideKeys: string[],
  splineIndex: number,
  textureReader: TextureReader | null,
  settings: Settings,
  buildMaterials: boolean,
) {
  let points = spline.points as GNodesBezierPoint[]

  const movementGroup = new MovementGroup()
  movementGroup.interFrameID = `${objectID}-${splineIndex}`
  movementGroup.frozen = true // We're sure that these are connected, so no need to optimise within a group

  if (spline.cyclic && points.length > 0) {
    points = points.slice()
    points.push(points[0]) // add the first point to the end
  }

  let i = 0
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
      buildMaterials
        ? buildMaterial(leftPoint, rightPoint, textureReader)
        : materialDefault,
      objectID,
    )
    bezier.overrideKeys = overrideKeys

    bezier.interFrameID = `${objectID}-${splineIndex}-${i}`
    movementGroup.addMovement(bezier)
    i++
  }

  return movementGroup
}

function* window<T>(inputArray: T[], size: number) {
  for (let index = 0; index + size <= inputArray.length; index++) {
    yield inputArray.slice(index, index + size)
  }
}

class TextureReader {
  private imageData: Buffer | null = null
  public width: number = 0
  public height: number = 0

  constructor(private filePath: string) {}

  async initialize(): Promise<void> {
    const fileData = await readFile(this.filePath)

    const metadata = PNG.sync.read(fileData)

    this.width = metadata.width
    this.height = metadata.height

    this.imageData = metadata.data
  }

  getPixelCoordinate(u: number, v: number): [x: number, y: number] {
    // Clamp u and v to [0, 1], flip y
    u = Math.max(0, Math.min(1, u))
    v = 1 - Math.max(0, Math.min(1, v))

    // Convert UV coordinates to pixel coordinates
    const x = Math.floor(u * (this.width - 1))
    const y = Math.floor(v * (this.height - 1))

    return [x, y]
  }

  readPixel(x: number, y: number): [number, number, number, number] {
    if (!this.imageData) {
      throw new Error('Image data not initialized. Call initialize() first.')
    }

    // Calculate the index in the image data array
    const index = (y * this.width + x) * 4

    // Read RGBA values and normalize to [0, 1]
    const r = this.imageData[index] / 255
    const g = this.imageData[index + 1] / 255
    const b = this.imageData[index + 2] / 255
    const a = this.imageData[index + 3] / 255

    return [r, g, b, a]
  }

  readStrip(x1: number, y1: number, x2: number, y2: number): RGBA[] {
    if (!this.imageData) {
      throw new Error('Image data not initialized. Call initialize() first.')
    }

    const startIndex = (y1 * this.width + x1) * 4
    const endIndex = (y2 * this.width + x2) * 4
    const strip: RGBA[] = []

    // Iterate forward or backwards
    if (endIndex >= startIndex) {
      for (let index = startIndex; index <= endIndex; index += 4) {
        const r = this.imageData[index] / 255
        const g = this.imageData[index + 1] / 255
        const b = this.imageData[index + 2] / 255
        // TODO: No alpha channel for now
        const a = 1 // this.imageData[index + 3] / 255
        strip.push([r, g, b, a])
      }
    } else {
      for (let index = endIndex; index <= startIndex; index += 4) {
        const r = this.imageData[index] / 255
        const g = this.imageData[index + 1] / 255
        const b = this.imageData[index + 2] / 255
        // TODO: No alpha channel for now
        const a = 1 //  this.imageData[index + 3] / 255
        strip.push([r, g, b, a])
      }
      // strip.reverse()
    }

    return strip
  }
}

function buildMaterial(
  prev: GNodesPolyLinePoint | GNodesBezierPoint,
  curr: GNodesPolyLinePoint | GNodesBezierPoint,
  textureReader: TextureReader | null,
) {
  // This method is only called if we're reading textures, in which case
  // it will have been initialised.
  const reader = textureReader!

  if (prev.uv && curr.uv) {
    // calculate texture pixel coordinates
    const start = reader.getPixelCoordinate(prev.uv[0], prev.uv[1])
    const end = reader.getPixelCoordinate(curr.uv[0], curr.uv[1])

    const strip = reader.readStrip(start[0], start[1], end[0], end[1])

    // Read a strip of texture information
    return new TextureStripMaterial(strip)
  }

  return new ColorRampMaterial(prev.color, curr.color)
}
