import { CatmullRomCurve3, CubicBezierCurve3, MathUtils, Vector3 } from 'three'
import { defaultTransitionMaterial } from './material'
import { Material } from '../optimiser/materials/Base'
import {
  PlannerMovementMove,
  MovementMoveReference,
  MovementMoveType,
} from './hardware'

import React from 'react'

export const TRANSITION_OBJECT_ID = '__transition'
export const GLOBAL_OVERRIDE_OBJECT_ID = '__all'

export enum MOVEMENT_TYPE {
  GROUP = 'movementgroup',
  LINE = 'line',
  POINT = 'point',
  TRANSITION = 'transition',
  POINT_TRANSITION = 'point-transition',
  INTER_LINE_TRANSITION = 'inter-line-transition',
  TRANSIT = 'transit',
}

const defaultSpeed = 30 // mm/s
const MILLISECONDS_IN_SECOND = 1000

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
   * Get the length of this movement in milliseconds
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
   * Given a movement ID, generate the final movement for sending to hardware.
   *
   * At this stage it's one MovementMove per Movement, if more are required, have flatten produce more movements.
   */
  abstract generateToolpath: () => PlannerMovementMove

  /**
   * Sample a point along this movement at time fraction t (0-1)
   */
  abstract samplePoint: (t: number) => Vector3

  /**
   * Get the approximate centroid of the movement
   */
  abstract getApproximateCentroid: () => Vector3

  /**
   * Reset any optimisation specific state, such as fudging line lengths to fit transitions in the gaps
   */
  abstract resetOptimisationState: () => void

  /**
   * Every movement needs a material to generate the light moves, and generate the ThreeJS representation
   */
  abstract material: Material
}

export type XYZ = [x: number, y: number, z: number]
export type RGB = [r: number, g: number, b: number]

export type AddLineCallback = (
  start: Vector3,
  end: Vector3,
  colorStart: RGB,
  colorEnd: RGB,
  objectID: string,
) => void

export type AddComponentCallback = (component: React.ReactNode) => void

export type DenseMovements = Movement[] & { __dense: true }

export function declareDense(movements: Movement[]) {
  return movements as unknown as DenseMovements
}

export function isMovementGroup(movement: Movement): movement is MovementGroup {
  return movement.type === MOVEMENT_TYPE.GROUP
}

/**
 * A group of movements that must be executed in order.
 */
export class MovementGroup extends Movement {
  readonly type = MOVEMENT_TYPE.GROUP
  public material = defaultTransitionMaterial

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

  public generateToolpath = () => {
    throw new Error(
      'generateToolpath called on MovementGroup, the movement bag should have been flattened',
    )
  }

  public getApproximateCentroid = () => {
    if (this.movements.length === 0) {
      throw new Error(
        'MovementGroup is empty, but getApproximateCentroid was called',
      )
    }

    const centers: Vector3[] = []

    for (const movement of this.movements) {
      centers.push(movement.getApproximateCentroid())
    }

    return getCentroid(centers)
  }

  public samplePoint = (t: number) => {
    throw new Error(
      'samplePoint called on MovementGroup, the movement bag should have been flattened',
    )
  }

  public resetOptimisationState = () => {
    // noop
  }
}

export function isLine(movement: Movement): movement is Line {
  return movement.type === MOVEMENT_TYPE.LINE
}

/**
 * A line is a linear move from one location to another, with the light on.
 */
export class Line extends Movement {
  readonly type = MOVEMENT_TYPE.LINE
  // Maximum speed in millimeters per second
  public maxSpeed: number = defaultSpeed

  // The maximum amount of shrink 0-1. If both are non-zero, meet at half way point
  private maxStartShrinkFactor = 0
  private maxEndShrinkFactor = 0

  // If we want to lock the speed regardless of the optimiser
  public lockSpeed = false

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
    return this.getStart().distanceTo(this.getEnd())
  }

  public getCost: () => number = () => {
    return this.getLength()
  }

  public setMaxSpeed = (maxSpeed: number) => {
    if (this.lockSpeed) return
    this.maxSpeed = maxSpeed
  }

  public getDuration = () => {
    return Math.ceil(
      (this.getLength() / this.maxSpeed) * MILLISECONDS_IN_SECOND,
    )
  }

  private getDirection = () => {
    const direction = this.to.clone().sub(this.from).normalize()

    return direction
  }

  public getStart = () => {
    if (this.maxStartShrinkFactor) {
      const length = this.from.distanceTo(this.to)

      let shrinkFactor = this.maxStartShrinkFactor

      if (this.maxEndShrinkFactor) {
        // If there's also a shrink factor on the end, clamp our start shrink factor to 0.5
        shrinkFactor = MathUtils.clamp(this.maxStartShrinkFactor, 0, 0.5)
      }

      return this.getDirection()
        .multiplyScalar(length * shrinkFactor)
        .add(this.from)
    }

    return this.from
  }

  public getEnd = () => {
    if (this.maxEndShrinkFactor) {
      const length = this.from.distanceTo(this.to)

      let shrinkFactor = this.maxEndShrinkFactor

      if (this.maxStartShrinkFactor) {
        // If there's also a shrink factor on the start, clamp our end shrink factor to 0.5
        shrinkFactor = MathUtils.clamp(this.maxEndShrinkFactor, 0, 0.5)
      }

      return this.getDirection()
        .multiplyScalar(length * (1 - shrinkFactor))
        .add(this.from)
    }

    return this.to
  }

  // Shrink the start by an amount in millimeters
  public shrinkStartByDistance = (distance: number) => {
    const length = this.from.distanceTo(this.to)
    const clampedDistance = MathUtils.clamp(distance, 0, length)
    const shinkFactor = clampedDistance / length

    this.maxStartShrinkFactor = shinkFactor
  }

  // Shrink the end by an amount in millimeters
  public shrinkEndByDistance = (distance: number) => {
    const length = this.from.distanceTo(this.to)
    const clampedDistance = MathUtils.clamp(distance, 0, length)
    const shinkFactor = clampedDistance / length

    this.maxEndShrinkFactor = shinkFactor
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

  public generateToolpath = () => {
    const move: PlannerMovementMove = {
      duration: this.getDuration(),
      type: MovementMoveType.LINE,
      reference: MovementMoveReference.ABSOLUTE,
      points: [this.getStart().toArray(), this.getEnd().toArray()],
      num_points: 2,
    }

    return move
  }

  public getApproximateCentroid = () => {
    return getCentroid([this.getStart(), this.getEnd()])
  }

  public samplePoint = (t: number) => {
    const start = this.getStart()
    const length = this.getLength()
    const direction = this.getEnd().clone().sub(this.getStart()).normalize()

    return direction.multiplyScalar(length * t).add(start)
  }

  public resetOptimisationState = () => {
    // line distance should be 100%
    this.maxStartShrinkFactor = 0
    this.maxEndShrinkFactor = 0
  }
}

export function isPoint(movement: Movement): movement is Point {
  return movement.type === MOVEMENT_TYPE.POINT
}

const zeroVector = new Vector3(0, 0, 0)

/**
 * A `Point` is a fixed duration stay at a certain point.
 */
export class Point extends Movement {
  readonly type = MOVEMENT_TYPE.POINT
  maxSpeed: number = defaultSpeed

  // For a particle, approach in the velocity of its velocity
  public velocity: Vector3 = new Vector3(0, 0, 0)

  constructor(
    public pos: Vector3,
    public duration: number, // ms Can be 0, for a 'passthrough'
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

    // The minimum duration
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

    return this.velocity.normalize().multiplyScalar(this.maxSpeed)
  }

  public getExpectedExitVelocity = () => {
    if (this.duration > 0) {
      return zeroVector
    }

    return this.velocity.normalize().multiplyScalar(this.maxSpeed)
  }

  public generateToolpath = () => {
    const move: PlannerMovementMove = {
      duration: this.getDuration(),
      type: MovementMoveType.LINE, // Despite being a point, draw a line
      reference: MovementMoveReference.ABSOLUTE,
      points: [this.getStart().toArray(), this.getEnd().toArray()],
      num_points: 2,
    }

    return move
  }

  public getApproximateCentroid = () => {
    return getCentroid([this.getStart(), this.getEnd()])
  }

  public samplePoint = (t: number) => {
    // Points are always just points, even if we fudge it a bit in the movement
    return this.pos
  }

  public resetOptimisationState = () => {
    // noop
  }
}

export function isTransition(movement: Movement): movement is Transition {
  return movement.type === MOVEMENT_TYPE.TRANSITION
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
  readonly type = MOVEMENT_TYPE.TRANSITION
  maxSpeed: number = defaultSpeed // mm/s
  private numSegments = 20

  public objectID = TRANSITION_OBJECT_ID

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
      this.getStart()
        .clone()
        .add(this.getDesiredEntryVelocity().clone().multiplyScalar(0.1)),
      this.getEnd()
        .clone()
        .sub(this.getExpectedExitVelocity().clone().multiplyScalar(0.1)),
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

    // Reset the curve
    this.curve = null

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
    return Math.ceil(
      (this.getLength() / this.maxSpeed) * MILLISECONDS_IN_SECOND,
    )
  }

  public getStart = () => {
    return this.from.getEnd()
  }

  public getEnd = () => {
    return this.to.getStart()
  }

  public getDesiredEntryVelocity = () => {
    return this.from.getExpectedExitVelocity()
  }

  public getExpectedExitVelocity = () => {
    return this.to.getDesiredEntryVelocity()
  }

  public generateToolpath = () => {
    const move: PlannerMovementMove = {
      duration: this.getDuration(),
      type: MovementMoveType.BEZIER_CUBIC, // Despite being a point, draw a line
      reference: MovementMoveReference.ABSOLUTE,
      points: [
        this.getStart().toArray(),
        this.getStart()
          .clone()
          .add(this.getDesiredEntryVelocity().clone().multiplyScalar(0.1))
          .toArray(),
        this.getEnd()
          .clone()
          .sub(this.getExpectedExitVelocity().clone().multiplyScalar(0.1))
          .toArray(),
        this.getEnd().toArray(),
      ],
      num_points: 4,
    }

    return move
  }

  public getApproximateCentroid = () => {
    return getCentroid([
      this.getStart(),
      this.getStart()
        .clone()
        .add(this.getDesiredEntryVelocity().clone().multiplyScalar(0.1)),
      this.getEnd()
        .clone()
        .sub(this.getExpectedExitVelocity().clone().multiplyScalar(0.1)),
      this.getEnd(),
    ])
  }

  public samplePoint = (t: number) => {
    const curve = this.lazyGenerateCurve()

    return curve.getPoint(t)
  }

  public resetOptimisationState = () => {
    // noop
  }
}

function getCentroid(points: Vector3[]) {
  const centroid = new Vector3()
  for (const point of points) {
    centroid.add(point)
  }
  centroid.divideScalar(points.length)

  return centroid
}

export function isPointTransition(
  movement: Movement,
): movement is PointTransition {
  return movement.type === MOVEMENT_TYPE.POINT_TRANSITION
}
/**
 * A transition is a move from one Point to another.
 *
 * Uses a catmull spline to visit points
 */
export class PointTransition extends Movement {
  readonly type = MOVEMENT_TYPE.POINT_TRANSITION
  maxSpeed: number = defaultSpeed
  public objectID = TRANSITION_OBJECT_ID

  private numSegments = 20

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

  private curve: CatmullRomCurve3 | null = null

  private lazyGenerateCurve = () => {
    if (this.curve) return this.curve

    /**
     * v0 – The starting point.
     * v1 – The first control point.
     * v2 – The second control point.
     * v3 – The ending point.
     */
    this.curve = new CatmullRomCurve3(
      [
        this.prePointMovement.getEnd(),
        this.pointFrom.getEnd(),
        this.pointTo.getStart(),
        this.postPointMovement.getStart(),
      ],
      false,
      'catmullrom',
    )

    this.curve.arcLengthDivisions = 20 // divide into 20 segments for length calculations

    return this.curve
  }

  private lazyGenerateCurveLength = () => {
    if (this.curvelength) return this.curvelength

    const curve = this.lazyGenerateCurve()

    // By default CatmullRomCurve3.getPoints(segments) gives us points along the entire length of vectors,
    // including the control points, we need to only build points between the start and end.

    // The control points are 1/3 and 2/3 of the way along the curve.

    this.curvePoints = []

    for (let index = 0; index < this.numSegments; index++) {
      // remap 0 -> num segments to a float between 1/3 and 2/3
      const t = MathUtils.mapLinear(index, 0, this.numSegments, 1 / 3, 2 / 3)

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

  public samplePoint = (t: number) => {
    const curve = this.lazyGenerateCurve()

    // The control points are 1/3 and 2/3 of the way along the curve, we only want to sample points within that region,
    // so remap the request, from 0-1 to 1/3-2/3
    const tR = MathUtils.mapLinear(t, 0, 1, 1 / 3, 2 / 3)

    return curve.getPoint(tR)
  }

  // Swap the ordering of this transition movement
  public flip = () => {
    const temp = this.prePointMovement
    this.prePointMovement = this.postPointMovement
    this.postPointMovement = temp

    const temp2 = this.pointTo
    this.pointTo = this.pointFrom
    this.pointFrom = temp2

    // Reset the curve
    this.curve = null
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
    return Math.ceil(
      (this.getLength() / this.maxSpeed) * MILLISECONDS_IN_SECOND,
    )
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

  public generateToolpath = () => {
    const move: PlannerMovementMove = {
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

    return move
  }

  public getApproximateCentroid = () => {
    return getCentroid([
      this.prePointMovement.getEnd(),
      this.pointFrom.getEnd(),
      this.pointTo.getStart(),
      this.postPointMovement.getStart(),
    ])
  }

  public resetOptimisationState = () => {
    // noop
  }
}

export function isInterLineTransition(
  movement: Movement,
): movement is InterLineTransition {
  return movement.type === MOVEMENT_TYPE.INTER_LINE_TRANSITION
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
export class InterLineTransition extends Movement {
  readonly type = MOVEMENT_TYPE.INTER_LINE_TRANSITION
  maxSpeed: number = defaultSpeed // mm/s
  private numSegments = 20

  constructor(
    public from: Line,
    public to: Line,
    public objectID: string, // Takes the ObjectID of the first line
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
      this.from.getEnd(),
      this.from.to,
      this.to.from,
      this.to.getStart(),
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
    return Math.ceil(
      (this.getLength() / this.maxSpeed) * MILLISECONDS_IN_SECOND,
    )
  }

  public getStart = () => {
    return this.from.getEnd()
  }

  public getEnd = () => {
    return this.to.getStart()
  }

  public getDesiredEntryVelocity = () => {
    return this.from.getExpectedExitVelocity()
  }

  public getExpectedExitVelocity = () => {
    return this.to.getDesiredEntryVelocity()
  }

  public generateToolpath = () => {
    const move: PlannerMovementMove = {
      duration: this.getDuration(),
      type: MovementMoveType.BEZIER_CUBIC, // Despite being a point, draw a line
      reference: MovementMoveReference.ABSOLUTE,
      points: [
        this.from.getEnd().toArray(),
        this.from.to.toArray(),
        this.to.from.toArray(),
        this.to.getStart().toArray(),
      ],
      num_points: 4,
    }

    return move
  }

  public getApproximateCentroid = () => {
    return getCentroid([this.getStart(), this.getEnd()])
  }

  public samplePoint = (t: number) => {
    const curve = this.lazyGenerateCurve()

    return curve.getPoint(t)
  }

  public resetOptimisationState = () => {
    // noop
  }
}

export function isTransit(movement: Movement): movement is Transit {
  return movement.type === MOVEMENT_TYPE.TRANSIT
}

/**
 * A `Transit` is an invisible move to a point without care for the path taken
 */
export class Transit extends Movement {
  readonly type = MOVEMENT_TYPE.TRANSIT

  constructor(
    public endPoint: Vector3,
    public duration: number, // Must be above 0ms
    public material: Material,
    public objectID: string,
  ) {
    super()

    if (this.duration < 1) {
      throw new Error(`Transit durations must be at least 1ms`)
    }
  }

  // Flipping a Transit does nothing
  public flip = () => {}

  public flatten = () => {
    return [this]
  }

  // Transits have an unknown length
  public getLength: () => number = () => {
    return 0
  }

  // Transits have no speed, only a duration
  public setMaxSpeed = (maxSpeed: number) => {
    // noop
  }

  // Transits have no cost
  public getCost: () => number = () => {
    return 0
  }

  /**
   * The duration of a transit is determined by its set duration, it cannot be shorter than 1ms
   */
  public getDuration = () => {
    return Math.ceil(this.duration)
  }

  public getStart = () => {
    return this.endPoint
  }

  public getEnd = () => {
    return this.endPoint
  }

  public getDesiredEntryVelocity = () => {
    // Assume the entry velocity is 0
    return zeroVector
  }

  public getExpectedExitVelocity = () => {
    // Assume the exit velocity is 0
    return zeroVector
  }

  public generateToolpath = () => {
    const move: PlannerMovementMove = {
      duration: this.getDuration(),
      type: MovementMoveType.POINT_TRANSIT,
      reference: MovementMoveReference.ABSOLUTE,
      points: [this.endPoint.toArray()],
      num_points: 1,
    }

    return move
  }

  public getApproximateCentroid = () => {
    return this.endPoint
  }

  public samplePoint = (t: number) => {
    // Transits exist at their endpoint
    return this.endPoint
  }

  public resetOptimisationState = () => {
    // noop
  }
}
