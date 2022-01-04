import {
  CatmullRomCurve3,
  Color,
  CubicBezierCurve3,
  MathUtils,
  Vector3,
} from 'three'
import { isSimpleColorMaterial, Material } from './material'
import {
  LightMove,
  MovementMove,
  MovementMoveReference,
  MovementMoveType,
} from './hardware'
import { LRUCache } from 'typescript-lru-cache'

import { v4 as uuidv4 } from 'uuid'
import React from 'react'

import { Segments, Segment, Html } from '@react-three/drei'
import { VisualisationSettings } from '../interface/state'
import { Intent, Tag } from '@blueprintjs/core'
import { NodeID, useTreeStore } from '../interface/RenderableTree'

const defaultSpeed = 30 // mm/s

/**
 * The base level optimisable movement.
 */
export abstract class Movement {
  /**
   * What type of movement
   */
  abstract type: string

  /**
   * The object ID, for propagating which movements are hidden
   */
  abstract objectID: string

  /**
   * The 'persistent' ID across frames to effectively cache optimisation
   */
  public interFrameID: string = ''

  /**
   * Take this movement in the opposite direction
   */
  abstract flip: () => void

  /**
   * Flatten this movement, calling a callback with its children.
   *
   * By default just adds itself to the array
   */
  abstract flatten: () => Movement[]

  /**
   * Get the length of this movement in mm
   */
  abstract getLength: () => number

  /**
   * Sets the desired speed for this movement in mm/second
   */
  abstract setMaxSpeed: (maxSpeed: number) => void

  /**
   * Get the length of this movement in ms
   */
  abstract getDuration: () => number

  /**
   * Get the cost of the movement in an arbritrary unit
   */
  abstract getCost: () => number

  /**
   * Get the starting position of this movement
   */
  abstract getStart: () => Vector3
  /**
   * Get the ending position of this movement
   */
  abstract getEnd: () => Vector3

  /**
   * Get the desired entry direction of this movement
   */
  abstract getDesiredEntryVelocity: () => Vector3
  /**
   * Get the desired exit direction of this movement
   */
  abstract getExpectedExitVelocity: () => Vector3

  /**
   * Given a movement ID, generate the final movement(s) for sending to hardware
   */
  abstract generateToolpath: (id: number) => MovementMove[]

  /**
   * Given a movement ID, generate the light moves for this movement, (defer to materials).
   */
  abstract generateLightpath: (id: number) => LightMove[]

  abstract generateThreeLineSegments: (
    movementIndex: number,
    visualisationSettings: VisualisationSettings,
    addColouredLine: AddLineCallback,
    addTransitionLine: AddLineCallback,
    addReactComponent: AddComponentCallback,
  ) => void
}

export type XYZ = [x: number, y: number, z: number]
export type RGBA = [r: number, g: number, b: number, a: number]

type AddLineCallback = (
  start: Vector3,
  end: Vector3,
  colorStart: RGBA,
  colorEnd: RGBA,
) => void

type AddComponentCallback = (component: React.ReactNode) => void

export type DenseMovements = Movement[] & { __dense: true }

export function declareDense(movements: Movement[]) {
  return movements as unknown as DenseMovements
}

export function isMovementGroup(movement: Movement): movement is MovementGroup {
  return movement.type === 'movementgroup'
}

/**
 * A group of movements that must be executed in order.
 */
export class MovementGroup extends Movement {
  readonly type = 'movementgroup'

  public objectID = '___movement_group' // This should never match

  public movements: Movement[] = []

  public addMovement = (movement: Movement) => {
    this.movements.push(movement)
  }

  public flip = () => {
    for (const movement of this.movements) {
      movement.flip()
    }
    this.movements.reverse()
  }

  public flatten = () => {
    const movements = []
    for (const movement of this.movements) {
      movements.push(...movement.flatten())
    }
    return movements
  }

  public getLength: () => number = () => {
    if (this.movements.length === 0) {
      return 0
    }

    return this.movements.reduce(
      (len, movement) => movement.getLength() + len,
      0,
    )
  }

  public getCost: () => number = () => {
    if (this.movements.length === 0) {
      return 0
    }

    return this.movements.reduce((len, movement) => movement.getCost() + len, 0)
  }

  public setMaxSpeed = (speed: number) => {
    if (this.movements.length === 0) {
      throw new Error('MovementGroup is empty, but setSpeed was called')
    }

    for (const movement of this.movements) {
      movement.setMaxSpeed(speed)
    }
  }

  public getDuration = () => {
    if (this.movements.length === 0) {
      return 0
    }

    return this.movements.reduce(
      (dur, movement) => movement.getDuration() + dur,
      0,
    )
  }

  public getStart = () => {
    if (this.movements.length === 0) {
      throw new Error('MovementGroup is empty, but getStart was called')
    }

    return this.movements[0].getStart()
  }

  public getEnd = () => {
    if (this.movements.length === 0) {
      throw new Error('MovementGroup is empty, but getEnd was called')
    }

    return this.movements[this.movements.length - 1].getEnd()
  }

  public getDesiredEntryVelocity = () => {
    if (this.movements.length === 0) {
      throw new Error(
        'MovementGroup is empty, but getDesiredEntryVelocity was called',
      )
    }

    return this.movements[0].getDesiredEntryVelocity()
  }

  public getExpectedExitVelocity = () => {
    if (this.movements.length === 0) {
      throw new Error(
        'MovementGroup is empty, but getExpectedExitVelocity was called',
      )
    }

    return this.movements[this.movements.length - 1].getExpectedExitVelocity()
  }

  public generateToolpath = (id: number) => {
    throw new Error(
      'generateToolpath called on MovementGroup, the movement bag should have been flattened',
    )
  }

  public generateLightpath = (id: number) => {
    throw new Error(
      'generateLightpath called on MovementGroup, the movement bag should have been flattened',
    )
  }

  public generateThreeLineSegments = (
    movementIndex: number,
    visualisationSettings: VisualisationSettings,
    addColouredLine: AddLineCallback,
    addTransitionLine: AddLineCallback,
    addReactComponent: AddComponentCallback,
  ) => {
    throw new Error(
      'generateThreeLineSegments called on MovementGroup, the movement bag should have been flattened',
    )
  }
}

export function isLine(movement: Movement): movement is Line {
  return movement.type === 'line'
}

/**
 * A line is a linear move from one location to another, with the light on.
 */
export class Line extends Movement {
  readonly type = 'line'
  maxSpeed: number = defaultSpeed

  constructor(
    public from: Vector3,
    public to: Vector3,
    public material: Material,
    public objectID: string,
  ) {
    super()
  }

  // Swap the ordering of these points
  public flip = () => {
    const temp = this.to
    this.to = this.from
    this.from = temp

    // TODO: Flip the material
  }

  public flatten = () => {
    return [this]
  }

  public getLength: () => number = () => {
    return this.from.distanceTo(this.to)
  }

  public getCost: () => number = () => {
    return this.getLength()
  }

  public setMaxSpeed = (maxSpeed: number) => {
    this.maxSpeed = maxSpeed
  }

  public getDuration = () => {
    // Lines are taken at maximum speed
    return Math.ceil(this.getLength() / this.maxSpeed)
  }

  public getStart = () => {
    return this.from
  }

  public getEnd = () => {
    return this.to
  }

  public getDesiredEntryVelocity = () => {
    return this.to
      .clone()
      .sub(this.from)
      .normalize()
      .multiplyScalar(this.maxSpeed)
  }

  public getExpectedExitVelocity = () => {
    return this.to
      .clone()
      .sub(this.from)
      .normalize()
      .multiplyScalar(this.maxSpeed)
  }

  public generateToolpath = (id: number) => {
    const move: MovementMove = {
      id,
      duration: this.getDuration(),
      type: MovementMoveType.LINE,
      reference: MovementMoveReference.ABSOLUTE,
      points: [this.getStart().toArray(), this.getEnd().toArray()],
      num_points: 2,
    }

    return [move]
  }

  public generateLightpath = (id: number) => {
    return this.material.generateLightpath(id, this)
  }

  public generateThreeLineSegments = (
    movementIndex: number,
    visualisationSettings: VisualisationSettings,
    addColouredLine: AddLineCallback,
    addTransitionLine: AddLineCallback,
    addReactComponent: AddComponentCallback,
  ) => {
    if (isSimpleColorMaterial(this.material)) {
      const batched = true

      // If we're annotating draw order, create the react component
      if (visualisationSettings.annotateDrawOrder) {
        addReactComponent(
          generateHtmlTagFromAveragePosition(
            this.objectID,
            [this.getStart(), this.getEnd()],
            'none',
            `L #${movementIndex + 1}`,
          ),
        )
      }

      if (batched) {
        addColouredLine(
          this.getStart(),
          this.getEnd(),
          this.material.color,
          this.material.color,
        )
      } else {
        addReactComponent(
          <Segments limit={1} lineWidth={1.0}>
            <Segment
              start={this.getStart()}
              end={this.getEnd()}
              color={
                new Color(
                  this.material.color[0],
                  this.material.color[1],
                  this.material.color[2],
                )
              }
            />
          </Segments>,
        )
      }

      return
    }

    console.warn(
      'Created a line with a non-simple material, make a react component for it',
    )
  }
}

export function isPoint(movement: Movement): movement is Point {
  return movement.type === 'point'
}

const zeroVector = new Vector3(0, 0, 0)

/**
 * A `Point` is a fixed duration stay at a certain point.
 */
export class Point extends Movement {
  readonly type = 'point'
  maxSpeed: number = defaultSpeed

  // For a particle, approach in the velocity of its velocity
  public velocity: Vector3 = new Vector3(0, 0, 0)

  constructor(
    public pos: Vector3,
    public duration: number, // Can be 0, for a 'passthrough'
    public material: Material,
    public objectID: string,
  ) {
    super()
  }

  // Flipping a Point does nothing
  public flip = () => {}

  public flatten = () => {
    return [this]
  }

  public getLength: () => number = () => {
    return 0
  }

  public setMaxSpeed = (maxSpeed: number) => {
    this.maxSpeed = maxSpeed
  }

  // Points have no cost
  public getCost: () => number = () => {
    return this.getLength()
  }

  /**
   * The duration of a point is determined by its set duration, or if it's nothing,
   * the minimum unit of time, 1 millisecond.
   */
  public getDuration = () => {
    if (this.duration > 0) {
      return Math.ceil(this.duration)
    }

    // The minimum duration is 1ms
    return 1
  }

  public getStart = () => {
    if (this.duration > 0) {
      return this.pos
    }

    // Start half of one millisecond away from our desired point, at max speed this is 0.15mm away
    return this.pos.clone().sub(
      this.getDesiredEntryVelocity()
        .clone()
        .multiplyScalar(0.001 / 2),
    )
  }

  public getEnd = () => {
    if (this.duration > 0) {
      return this.pos
    }

    // End half of one millisecond away from our desired point, at max speed this is 0.15mm away
    return this.pos.clone().add(
      this.getDesiredEntryVelocity()
        .clone()
        .multiplyScalar(0.001 / 2),
    )
  }

  public getDesiredEntryVelocity = () => {
    if (this.duration > 0) {
      return zeroVector
    }

    return this.velocity.normalize().multiplyScalar(this.maxSpeed * 0.01)
  }

  public getExpectedExitVelocity = () => {
    if (this.duration > 0) {
      return zeroVector
    }

    return this.velocity.normalize().multiplyScalar(this.maxSpeed * 0.01)
  }

  public generateToolpath = (id: number) => {
    const move: MovementMove = {
      id,
      duration: this.getDuration(),
      type: MovementMoveType.LINE, // Despite being a point, draw a line
      reference: MovementMoveReference.ABSOLUTE,
      points: [this.getStart().toArray(), this.getEnd().toArray()],
      num_points: 2,
    }

    return [move]
  }

  public generateLightpath = (id: number) => {
    return this.material.generateLightpath(id, this)
  }

  public generateThreeLineSegments = (
    movementIndex: number,
    visualisationSettings: VisualisationSettings,
    addColouredLine: AddLineCallback,
    addTransitionLine: AddLineCallback,
    addReactComponent: AddComponentCallback,
  ) => {
    if (isSimpleColorMaterial(this.material)) {
      const batched = true

      // If we're annotating draw order, create the react component
      if (visualisationSettings.annotateDrawOrder) {
        addReactComponent(
          generateHtmlTagFromAveragePosition(
            this.objectID,
            [this.getStart(), this.getEnd()],
            'none',
            `P #${movementIndex + 1}`,
          ),
        )
      }

      if (batched) {
        addColouredLine(
          this.getStart(),
          this.getEnd(),
          this.material.color,
          this.material.color,
        )
      } else {
        addReactComponent(
          <Segments limit={1} lineWidth={1.0}>
            <Segment
              start={this.getStart()}
              end={this.getEnd()}
              color={
                new Color(
                  this.material.color[0],
                  this.material.color[1],
                  this.material.color[2],
                )
              }
            />
          </Segments>,
        )
      }

      return
    }

    console.warn(
      'Created a point with a non-simple material, make a react component for it',
    )
  }
}

export function isTransition(movement: Movement): movement is Transition {
  return movement.type === 'transition'
}

// const transitionCurveCache = new LRUCache<string, number>({
//   maxSize: 1000, // store 1000 lengths by default
// });

// function transitionKeygen(a: Vector3, b: Vector3, c: Vector3, d: Vector3) {
//   return `${a.x},${a.y},${a.z}-${b.x},${b.y},${b.z}-${c.x},${c.y},${c.z}-${d.x},${d.y},${d.z}`;
// }

// let cacheHits = 0;

/**
 * A transition is a move from one Movement to another.
 *
 * It's probably going to be a Cubic Bezier, with the velocity components used as control points.
 */
export class Transition extends Movement {
  readonly type = 'transition'
  maxSpeed: number = defaultSpeed

  public objectID = 'transition'

  constructor(
    public from: Movement,
    public to: Movement,
    public material: Material,
  ) {
    super()
  }

  private curve: CubicBezierCurve3 | null = null

  private lazyGenerateCurve = () => {
    if (this.curve) return this.curve

    /**
     * v0 – The starting point.
     * v1 – The first control point.
     * v2 – The second control point.
     * v3 – The ending point.
     */
    this.curve = new CubicBezierCurve3(
      this.getStart(),
      this.getStart().clone().add(this.getDesiredEntryVelocity()),
      this.getEnd().clone().sub(this.getExpectedExitVelocity()),
      this.getEnd(),
    )

    this.curve.arcLengthDivisions = 20 // divide into 20 segments for length calculations

    return this.curve
  }

  // Swap the ordering of this transition movement
  public flip = () => {
    const temp = this.to
    this.to = this.from
    this.from = temp

    // TODO: Flip the material
  }

  public flatten = () => {
    return [this]
  }

  public getCost = () => {
    return this.getLength()
  }

  public getLength = () => {
    // // Check if we have a cached length first since this is expensive.
    // const key = transitionKeygen(
    //   this.getStart(),
    //   this.getEnd(),
    //   this.getDesiredEntryVelocity(),
    //   this.getExpectedExitVelocity()
    // );

    // const cached = transitionCurveCache.get(key);

    // if (cached) return cached;

    // Otherwise generate the curve, get the length
    const curve = this.lazyGenerateCurve()

    const length = curve.getLength()

    // transitionCurveCache.set(key, length);

    return length
  }

  public setMaxSpeed = (maxSpeed: number) => {
    this.maxSpeed = maxSpeed
  }

  public getDuration = () => {
    return Math.ceil(this.getLength() / this.maxSpeed)
  }

  public getStart = () => {
    return this.from.getEnd()
  }

  public getEnd = () => {
    return this.to.getStart()
  }

  public getDesiredEntryVelocity = () => {
    return this.from.getExpectedExitVelocity().clone().multiplyScalar(0.5)
  }

  public getExpectedExitVelocity = () => {
    return this.to.getDesiredEntryVelocity().clone().multiplyScalar(0.5)
  }

  public generateToolpath = (id: number) => {
    const move: MovementMove = {
      id,
      duration: this.getDuration(),
      type: MovementMoveType.BEZIER_CUBIC, // Despite being a point, draw a line
      reference: MovementMoveReference.ABSOLUTE,
      points: [
        this.getStart().toArray(),
        this.getStart().clone().add(this.getDesiredEntryVelocity()).toArray(),
        this.getEnd().clone().sub(this.getExpectedExitVelocity()).toArray(),
        this.getEnd().toArray(),
      ],
      num_points: 4,
    }

    return [move]
  }

  public generateLightpath = (id: number) => {
    return this.material.generateLightpath(id, this)
  }

  public generateThreeLineSegments = (
    movementIndex: number,
    visualisationSettings: VisualisationSettings,
    addColouredLine: AddLineCallback,
    addTransitionLine: AddLineCallback,
    addReactComponent: AddComponentCallback,
  ) => {
    if (!isSimpleColorMaterial(this.material)) {
      console.warn(
        'Created a transition with a non-simple material, make a react component for it',
      )

      return
    }

    const batched = true

    const curve = this.lazyGenerateCurve()

    const points = curve.getPoints(20)

    // If we're annotating draw order, create the react component
    if (visualisationSettings.annotateDrawOrder) {
      addReactComponent(
        generateHtmlTagFromAveragePosition(
          this.objectID,
          points,
          'primary',
          `T #${movementIndex + 1}`,
        ),
      )
    }

    if (batched) {
      for (let index = 1; index < points.length; index++) {
        const start = points[index - 1]
        const end = points[index]

        addTransitionLine(start, end, this.material.color, this.material.color)
      }
    } else {
      const segments: { start: Vector3; end: Vector3 }[] = []

      for (let index = 1; index < points.length; index++) {
        const start = points[index - 1]
        const end = points[index]

        segments.push({ start, end })
      }

      const col = this.material.color

      addReactComponent(
        <Segments limit={20} lineWidth={1.0} {...{ dashed: true }}>
          {segments.map((segment, index) => (
            <Segment
              key={index}
              start={segment.start}
              end={segment.end}
              color={new Color(col[0], col[1], col[2])}
            />
          ))}
        </Segments>,
      )
    }
  }
}

function TagThatHidesWhenNotInList(props: {
  objectID: NodeID
  intent: Intent
  text: string
}) {
  const isHidden = useTreeStore(state => {
    // If nothing is hovered, display everything
    if (state.hoveredObjectIDs.length === 0) {
      return false
    }

    // If something is hovered, only display if it matches
    return !state.hoveredObjectIDs.includes(props.objectID)
  })

  return (
    <Tag
      intent={props.intent}
      minimal={isHidden}
      style={{ opacity: isHidden ? 0.5 : 1 }}
    >
      {props.text}
    </Tag>
  )
}

function generateHtmlTagFromAveragePosition(
  objectID: NodeID,
  points: Vector3[],
  intent: Intent,
  text: string,
) {
  const centroid = new Vector3()
  for (const point of points) {
    centroid.add(point)
  }
  centroid.divideScalar(points.length)

  // Convert the centroid from Blender units to ThreeJS units
  const threeCentroid = new Vector3(centroid.x, centroid.z, -centroid.y)

  const component = (
    <Html position={threeCentroid}>
      <TagThatHidesWhenNotInList
        objectID={objectID}
        intent={intent}
        text={text}
      />
    </Html>
  )

  return component
}

/**
 * A transition is a move from one Point to another.
 *
 * Uses a catmull spline to visit points
 */
export class PointTransition extends Movement {
  readonly type = 'point-transition'
  maxSpeed: number = defaultSpeed
  public objectID = 'transition'

  constructor(
    public prePointMovement: Movement,
    public pointFrom: Point,
    public pointTo: Point,
    public postPointMovement: Movement,
    public material: Material,
  ) {
    super()
  }

  private curvePoints: Vector3[] = []
  private curvelength: number | null = null

  private lazyGenerateCurveLength = () => {
    if (this.curvelength) return this.curvelength

    const curve = new CatmullRomCurve3(
      [
        this.prePointMovement.getEnd(),
        this.pointFrom.getEnd(),
        this.pointTo.getStart(),
        this.postPointMovement.getStart(),
      ],
      false,
      'catmullrom',
    )

    const segments = 20

    // By default CatmullRomCurve3.getPoints(segments) gives us points along the entire length of vectors,
    // including the control points, we need to only build points between the start and end.

    // The control points are 1/3 and 2/3 of the way along the curve.

    this.curvePoints = []

    for (let index = 0; index < segments; index++) {
      // remap 0 -> num segments to a float between 1/3 and 2/3
      const t = MathUtils.mapLinear(index, 0, segments, 1 / 3, 2 / 3)

      this.curvePoints.push(curve.getPoint(t))
    }

    let distance = 0

    let lastPoint = this.curvePoints[0]

    for (let index = 1; index < this.curvePoints.length; index++) {
      const point = this.curvePoints[index]

      distance += lastPoint.distanceTo(point)
      lastPoint = point
    }

    this.curvelength = distance

    return distance
  }

  // Swap the ordering of this transition movement
  public flip = () => {
    const temp = this.prePointMovement
    this.prePointMovement = this.postPointMovement
    this.postPointMovement = temp

    const temp2 = this.pointTo
    this.pointTo = this.pointFrom
    this.pointFrom = temp2

    // TODO: Flip the material
  }

  public flatten = () => {
    return [this]
  }

  public getCost = () => {
    return this.getLength()
  }

  public getLength = () => {
    const length = this.lazyGenerateCurveLength()

    return length
  }

  public setMaxSpeed = (maxSpeed: number) => {
    this.maxSpeed = maxSpeed
  }

  public getDuration = () => {
    return Math.ceil(this.getLength() / this.maxSpeed)
  }

  public getStart = () => {
    return this.pointFrom.getEnd()
  }

  public getEnd = () => {
    return this.pointTo.getStart()
  }

  public getDesiredEntryVelocity = () => {
    return this.pointFrom.getExpectedExitVelocity()
  }

  public getExpectedExitVelocity = () => {
    return this.pointTo.getDesiredEntryVelocity()
  }

  public generateToolpath = (id: number) => {
    const move: MovementMove = {
      id,
      duration: this.getDuration(),
      type: MovementMoveType.CATMULL_SPLINE, // Despite being a point, draw a line
      reference: MovementMoveReference.ABSOLUTE,
      points: [
        // this.prePointMovement.getEnd().toArray(),
        // this.pointFrom.getEnd().toArray(),
        // this.pointTo.getStart().toArray(),
        // this.postPointMovement.getStart().toArray(),

        this.prePointMovement.getEnd().toArray(),

        this.pointFrom.getEnd().toArray(),
        this.pointTo.getStart().toArray(),
        this.postPointMovement.getStart().toArray(),
      ],
      num_points: 4,
    }

    return [move]
  }

  public generateLightpath = (id: number) => {
    return this.material.generateLightpath(id, this)
  }

  public generateThreeLineSegments = (
    movementIndex: number,
    visualisationSettings: VisualisationSettings,
    addColouredLine: AddLineCallback,
    addTransitionLine: AddLineCallback,
    addReactComponent: AddComponentCallback,
  ) => {
    if (!isSimpleColorMaterial(this.material)) {
      console.warn(
        'Created a transition with a non-simple material, make a react component for it',
      )

      return
    }

    const batched = true

    this.lazyGenerateCurveLength()

    const points = this.curvePoints

    // If we're annotating draw order, create the react component
    if (visualisationSettings.annotateDrawOrder) {
      addReactComponent(
        generateHtmlTagFromAveragePosition(
          this.objectID,
          points,
          'primary',
          `C #${movementIndex + 1}`,
        ),
      )
    }

    if (batched) {
      for (let index = 1; index < points.length; index++) {
        const start = points[index - 1]
        const end = points[index]
        addTransitionLine(start, end, this.material.color, this.material.color)
      }
    } else {
      const segments: { start: Vector3; end: Vector3 }[] = []

      for (let index = 1; index < points.length; index++) {
        const start = points[index - 1]
        const end = points[index]

        segments.push({ start, end })
      }

      const col = this.material.color

      addReactComponent(
        <Segments limit={20} lineWidth={1.0} {...{ dashed: true }}>
          {segments.map((segment, index) => (
            <Segment
              key={index}
              start={segment.start}
              end={segment.end}
              color={new Color(col[0], col[1], col[2])}
            />
          ))}
        </Segments>,
      )
    }
  }
}
