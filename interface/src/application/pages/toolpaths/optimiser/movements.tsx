import { CatmullRomCurve3, CubicBezierCurve3, MathUtils, Vector3 } from 'three'
import { defaultTransitionMaterial } from './material'
import { Material } from '../optimiser/materials/Base'
import {
  PlannerMovementMove,
  MovementMoveReference,
  MovementMoveType,
} from './hardware'
import { TriggerCall, TriggerType } from './triggers'
import { MOVEMENT_TYPE } from './movement_types'

import {
  fcmp,
  findHighestApproximateSpeedAndT,
  isMovementGroup,
  MILLISECONDS_IN_SECOND,
  predictSpeedAtT,
  predictVelocityAtT,
} from './movement_utilities'

import React from 'react'

export const TRANSITION_OBJECT_ID = '__transition'
export const GLOBAL_OVERRIDE_OBJECT_ID = '__all'

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
   * The object ID, unique within a scene.
   */
  abstract objectID: string

  /**
   * The override keys for propagating overrides, hidden state, etc. Highest specificity at the end of the array
   */
  abstract overrideKeys: string[]

  /**
   * The 'persistent' ID across frames to effectively cache optimisation
   */
  public interFrameID: string = ''

  /**
   * Whether the movement is flipped
   */
  public isFlipped: boolean = false

  /**
   * Take this movement in the opposite direction
   */
  abstract flip: () => void

  /**
   * Flatten this movement, returning a list of it and its children in order
   */
  abstract flatten: () => Movement[]

  /**
   * Hydrate this movement with cached data
   */
  abstract hydrate: (serialised: SerialisedTour) => void

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
   */
  abstract generateToolpath: () => PlannerMovementMove[]

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
   * Get any triggers this movement should trip
   */
  abstract getTriggers: () => TriggerCall[]

  public triggers: TriggerCall[] = []

  /**
   * Every movement needs a material to generate the light moves, and generate the ThreeJS representation.
   *
   * This is the current material, it may have been mutated by something like the sparseToDense function.
   */
  abstract material: Material

  /**
   * Keep the base material. This is used to reset the material in the resetOptimisationState function.
   */
  abstract baseMaterial: Material
}

export type XYZ = [x: number, y: number, z: number]
export type RGB = [r: number, g: number, b: number]
export type RGBA = [r: number, g: number, b: number, a: number]

export type AddLineCallback = (
  start: Vector3,
  end: Vector3,
  colorStart: RGBA,
  colorEnd: RGBA,
  movementIndex: number,
  objectID?: string,
) => void

export type AddComponentCallback = (component: React.ReactNode) => void

export interface SerialisedTour {
  [interFrameID: string]: {
    order: number
    flipped: boolean
  }
}

export function serialiseTour(movements: Movement[]): SerialisedTour {
  const ordering: {
    [interFrameID: string]: {
      order: number
      flipped: boolean
    }
  } = {}

  let index = 0
  for (let i = 0; i < movements.length; i++) {
    if (isMovementGroup(movements[i])) {
      ordering[movements[i].interFrameID] = {
        order: index, // So we know where to insert the movement group in the list
        flipped: movements[i].isFlipped,
      }
    }

    // Flatten movements
    const flattened = movements[i].flatten()

    for (let j = 0; j < flattened.length; j++) {
      const movement = flattened[j]
      ordering[movement.interFrameID] = {
        order: index++,
        flipped: movement.isFlipped,
      }
    }
  }

  return ordering
}

export function deserialiseTour(
  sparseBag: Movement[],
  serialised: SerialisedTour,
) {
  const movements = sparseBag.slice()
  // Hydration sorts movement groups recursively
  for (let index = 0; index < movements.length; index++) {
    const movement = movements[index]
    movement.hydrate(serialised)
  }

  // Sort the final ordering
  movements.sort((a, b) => {
    const aOrder = serialised[a.interFrameID]?.order ?? 0
    const bOrder = serialised[b.interFrameID]?.order ?? 0
    return aOrder - bOrder
  })
  return movements
}

/**
 * A group of movements that must be executed in order.
 */
export class MovementGroup extends Movement {
  readonly type = MOVEMENT_TYPE.GROUP
  public material = defaultTransitionMaterial
  public baseMaterial = defaultTransitionMaterial

  public objectID = '___movement_group' // This should never match

  public overrideKeys = [] // This is never matched on the actual MovementGroup keys, the children get matched on their own

  private movements: Movement[] = []

  // Whether the internal ordering is frozen or can be mutated. Also prevents flipping
  public frozen = false

  public addMovement = (movement: Movement) => {
    this.movements.push(movement)

    this.interFrameID = `[${this.movements
      .map(movement => movement.interFrameID)
      .join(',')}]`
  }

  public flip = () => {
    // Frozen movement groups can't be flipped
    if (this.frozen) {
      return
    }

    // Otherwise flip all internal movements
    for (let index = 0; index < this.movements.length; index++) {
      const movement = this.movements[index]
      movement.flip()
    }
    this.movements.reverse()
  }

  public getMovements = () => {
    return this.movements
  }

  public flatten = () => {
    const movements = []
    const thisMovements = this.getMovements()
    for (let index = 0; index < thisMovements.length; index++) {
      const movement = thisMovements[index]
      movements.push(...movement.flatten())
    }
    return movements
  }

  public hydrate = (serialised: SerialisedTour) => {
    // Hydrate children first
    for (let index = 0; index < this.getMovements().length; index++) {
      const movement = this.getMovements()[index]
      movement.hydrate(serialised)
    }

    // Sort these movements
    this.getMovements().sort((a, b) => {
      const aOrder = serialised[a.interFrameID]?.order ?? 0
      const bOrder = serialised[b.interFrameID]?.order ?? 0
      return aOrder - bOrder
    })
  }

  public getLength: () => number = () => {
    if (this.movements.length === 0) {
      return 0
    }

    return this.getMovements().reduce(
      (len, movement) => movement.getLength() + len,
      0,
    )
  }

  public getCost: () => number = () => {
    if (this.movements.length === 0) {
      return 0
    }

    return this.getMovements().reduce(
      (len, movement) => movement.getCost() + len,
      0,
    )
  }

  public setMaxSpeed = (speed: number) => {
    if (this.movements.length === 0) {
      throw new Error('MovementGroup is empty, but setSpeed was called')
    }

    for (let index = 0; index < this.getMovements().length; index++) {
      const movement = this.getMovements()[index]
      movement.setMaxSpeed(speed)
    }
  }

  public getDuration = () => {
    if (this.movements.length === 0) {
      return 0
    }

    return this.getMovements().reduce(
      (dur, movement) => movement.getDuration() + dur,
      0,
    )
  }

  public getStart = () => {
    if (this.movements.length === 0) {
      throw new Error('MovementGroup is empty, but getStart was called')
    }

    return this.getMovements()[0].getStart()
  }

  public getEnd = () => {
    if (this.movements.length === 0) {
      throw new Error('MovementGroup is empty, but getEnd was called')
    }

    return this.getMovements()[this.getMovements().length - 1].getEnd()
  }

  public getDesiredEntryVelocity = () => {
    if (this.movements.length === 0) {
      throw new Error(
        'MovementGroup is empty, but getDesiredEntryVelocity was called',
      )
    }

    return this.getMovements()[0].getDesiredEntryVelocity()
  }

  public getExpectedExitVelocity = () => {
    if (this.movements.length === 0) {
      throw new Error(
        'MovementGroup is empty, but getExpectedExitVelocity was called',
      )
    }

    return this.getMovements()[
      this.getMovements().length - 1
    ].getExpectedExitVelocity()
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

    for (let index = 0; index < this.getMovements().length; index++) {
      const movement = this.getMovements()[index]
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
    for (let index = 0; index < this.getMovements().length; index++) {
      const movement = this.getMovements()[index]
      movement.resetOptimisationState()
    }
  }

  public getTriggers = () => {
    const triggers = []
    const thisMovements = this.getMovements()
    for (let index = 0; index < thisMovements.length; index++) {
      const movement = thisMovements[index]
      triggers.push(...movement.getTriggers())
    }
    return triggers
  }
}

/**
 * A line is a linear move from one location to another, with the light on.
 */
export class Line extends Movement {
  readonly type = MOVEMENT_TYPE.LINE
  // Maximum speed in millimeters per second
  public maxSpeed: number = defaultSpeed
  public baseMaterial: Material

  // The maximum amount of shrink 0-1. If both are non-zero, meet at half way point
  private maxStartShrinkFactor = 0
  private maxEndShrinkFactor = 0

  // If we want to lock the speed regardless of the optimiser, set this to a non-zero number
  // If it's below the max speed, it will be used instead
  public lockSpeed: number = 0

  constructor(
    private from: Vector3,
    private to: Vector3,
    public material: Material,
    public objectID: string,
    public overrideKeys: string[],
  ) {
    super()
    this.baseMaterial = material
  }

  // Swap the ordering of these points
  public flip = () => {
    // return
    this.isFlipped = !this.isFlipped
  }

  public getTo = () => {
    return this.isFlipped ? this.from : this.to
  }

  public getFrom = () => {
    return this.isFlipped ? this.to : this.from
  }

  public flatten = () => {
    return [this]
  }

  public hydrate = (serialised: SerialisedTour) => {
    this.isFlipped = serialised[this.interFrameID]?.flipped
  }

  public getLength: () => number = () => {
    return this.getStart().distanceTo(this.getEnd())
  }

  public getCost: () => number = () => {
    return this.getLength()
  }

  public setMaxSpeed = (maxSpeed: number) => {
    this.maxSpeed = maxSpeed
  }

  private calculateSpeed = () => {
    let speedUsed = this.maxSpeed

    if (this.lockSpeed !== 0 && this.lockSpeed < this.maxSpeed) {
      speedUsed = this.lockSpeed
    }

    return speedUsed
  }

  public getDuration = () => {
    return Math.ceil(
      (this.getLength() / this.calculateSpeed()) * MILLISECONDS_IN_SECOND,
    )
  }

  private getDirection = () => {
    const direction = this.getTo().clone().sub(this.getFrom()).normalize()

    return direction
  }

  public getStart = () => {
    if (this.maxStartShrinkFactor) {
      const length = this.getFrom().distanceTo(this.getTo())

      let shrinkFactor = this.maxStartShrinkFactor

      if (this.maxEndShrinkFactor) {
        // If there's also a shrink factor on the end, clamp our start shrink factor to 0.5
        shrinkFactor = MathUtils.clamp(this.maxStartShrinkFactor, 0, 0.5)
      }

      return this.getDirection()
        .multiplyScalar(length * shrinkFactor)
        .add(this.getFrom())
    }

    return this.getFrom()
  }

  public getEnd = () => {
    if (this.maxEndShrinkFactor) {
      const length = this.getFrom().distanceTo(this.getTo())

      let shrinkFactor = this.maxEndShrinkFactor

      if (this.maxStartShrinkFactor) {
        // If there's also a shrink factor on the start, clamp our end shrink factor to 0.5
        shrinkFactor = MathUtils.clamp(this.maxEndShrinkFactor, 0, 0.5)
      }

      return this.getDirection()
        .multiplyScalar(length * (1 - shrinkFactor))
        .add(this.getFrom())
    }

    return this.getTo()
  }

  // Shrink the start by an amount in millimeters
  public shrinkStartByDistance = (distance: number) => {
    const length = this.getFrom().distanceTo(this.getTo())
    const clampedDistance = MathUtils.clamp(distance, 0, length)
    const shinkFactor = clampedDistance / length

    this.maxStartShrinkFactor = shinkFactor
  }

  // Shrink the end by an amount in millimeters
  public shrinkEndByDistance = (distance: number) => {
    const length = this.getFrom().distanceTo(this.getTo()) // raw without the shrinkage
    const clampedDistance = MathUtils.clamp(distance, 0, length)
    const shinkFactor = clampedDistance / length

    this.maxEndShrinkFactor = shinkFactor
  }

  public getDesiredEntryVelocity = () => {
    return this.getTo()
      .clone()
      .sub(this.getFrom())
      .normalize()
      .multiplyScalar(this.calculateSpeed())
  }

  public getExpectedExitVelocity = () => {
    return this.getTo()
      .clone()
      .sub(this.getFrom())
      .normalize()
      .multiplyScalar(this.calculateSpeed())
  }

  public generateToolpath = () => {
    const move: PlannerMovementMove = {
      duration: this.getDuration(),
      type: MovementMoveType.LINE,
      reference: MovementMoveReference.ABSOLUTE,
      points: [this.getStart().toArray(), this.getEnd().toArray()],
    }

    return [move]
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
    this.material = this.baseMaterial
  }

  public getTriggers = () => {
    return this.triggers
  }
}

const zeroVector = new Vector3(0, 0, 0)

/**
 * A `Point` is a fixed duration stay at a certain point.
 */
export class Point extends Movement {
  readonly type = MOVEMENT_TYPE.POINT
  maxSpeed: number = defaultSpeed

  public baseMaterial: Material

  // For a particle, approach in the velocity of its velocity
  public velocity: Vector3 = new Vector3(0, 0, 0)

  constructor(
    public pos: Vector3,
    public duration: number, // ms Can be 0, for a 'passthrough'
    public material: Material,
    public objectID: string,
    public overrideKeys: string[],
  ) {
    super()
    this.baseMaterial = material
  }

  // Flipping a Point does nothing
  public flip = () => {}

  public flatten = () => {
    return [this]
  }

  public hydrate = (serialised: SerialisedTour) => {
    this.isFlipped = serialised[this.interFrameID]?.flipped
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
    }

    return [move]
  }

  public getApproximateCentroid = () => {
    return getCentroid([this.getStart(), this.getEnd()])
  }

  public samplePoint = (t: number) => {
    // Points are always just points, even if we fudge it a bit in the movement
    return this.pos
  }

  public resetOptimisationState = () => {
    this.material = this.baseMaterial
  }

  public getTriggers = () => {
    return this.triggers
  }
}

/**
 * A transition is a move from one Movement to another.
 *
 * It's probably going to be a Cubic Bezier, with the velocity components used as control points.
 */
export class Transition extends Movement {
  readonly type = MOVEMENT_TYPE.TRANSITION
  maxSpeed: number = defaultSpeed // mm/s
  public baseMaterial: Material

  public objectID = TRANSITION_OBJECT_ID
  public overrideKeys = []

  constructor(
    public c0: Vector3,
    public c1: Vector3,
    public c2: Vector3,
    public c3: Vector3,
    public entryVelocity: Vector3,
    public exitVelocity: Vector3,
    public material: Material,
    private entrySpeedExact: boolean, // false is exact exit speed
  ) {
    super()
    this.baseMaterial = material
  }

  private curve: CubicBezierCurve3 | null = null

  /**
   * Normalises the speed so that the maximum is hit.
   */
  private normaliseSpeed = () => {
    const desiredEntrySpeed = Math.min(
      this.getDesiredEntryVelocity().length(),
      this.maxSpeed,
    )
    const desiredMaxSpeed = this.maxSpeed
    const desiredExitSpeed = Math.min(
      this.getExpectedExitVelocity().length(),
      this.maxSpeed,
    )

    let desiredSpeed = desiredMaxSpeed
    let tToPredictSpeedAt = 0.5

    if (this.getEntrySpeedExact() && !fcmp(desiredEntrySpeed, 0, 5)) {
      // if the entry speed is the one we need to be exact and it's not 0ish, try to hit that
      desiredSpeed = desiredEntrySpeed

      tToPredictSpeedAt = 0
    } else if (!this.getEntrySpeedExact() && !fcmp(desiredExitSpeed, 0, 5)) {
      // if the entry speed is the one we need to be exact and it's not 0ish, try to hit that
      desiredSpeed = desiredExitSpeed

      tToPredictSpeedAt = 1
    } else {
      // otherwise just try and hit the max speed at our fastest point
      const { maxSpeed, tOfHighestSpeed } =
        findHighestApproximateSpeedAndT(this)
      desiredSpeed = this.maxSpeed

      tToPredictSpeedAt = tOfHighestSpeed
    }

    this.maxSpeed = 100

    const speedAtT = predictSpeedAtT(this, tToPredictSpeedAt)

    const factor = desiredSpeed / speedAtT

    if (!isFinite(factor)) {
      console.log(
        `bad factor ${factor} actualSpeedT ${tToPredictSpeedAt} speedAtT ${speedAtT} desiredSpeedAtT ${desiredSpeed}`,
      )

      return this.curve
    }

    this.maxSpeed = this.maxSpeed * factor
  }

  private lazyGenerateCurve = () => {
    if (this.curve) return this.curve

    /**
     * v0 – The starting point.
     * v1 – The first control point.
     * v2 – The second control point.
     * v3 – The ending point.
     */
    this.curve = new CubicBezierCurve3(
      this.getC0(),
      this.getC1(),
      this.getC2(),
      this.getC3(),
    )

    this.curve.arcLengthDivisions = 20 // divide into 20 segments for length calculations

    return this.curve
  }

  // Swap the ordering of this transition movement
  public flip = () => {
    this.isFlipped = !this.isFlipped

    // Reset the curve
    this.curve = null
  }

  public getEntrySpeedExact = () => {
    // The notion of entry is stable over flipped and unflipped versions
    return this.entrySpeedExact
  }

  public flatten = () => {
    return [this]
  }

  private getC0 = () => {
    if (!this.isFlipped) {
      return this.c0
    } else {
      return this.c3
    }
  }

  private getC1 = () => {
    if (!this.isFlipped) {
      return this.c1
    } else {
      return this.c2
    }
  }

  private getC2 = () => {
    if (!this.isFlipped) {
      return this.c2
    } else {
      return this.c1
    }
  }

  private getC3 = () => {
    if (!this.isFlipped) {
      return this.c3
    } else {
      return this.c0
    }
  }

  public hydrate = (serialised: SerialisedTour) => {
    this.isFlipped = serialised[this.interFrameID]?.flipped
  }

  public getCost = () => {
    return this.getLength()
  }

  public getLength = () => {
    const curve = this.lazyGenerateCurve()

    const length = curve.getLength()

    return length
  }

  public setMaxSpeed = (maxSpeed: number) => {
    this.maxSpeed = maxSpeed

    // after this call this.maxSpeed will likely be different to what is set in maxSpeed
    this.normaliseSpeed()
  }

  public getDuration = () => {
    return Math.ceil(
      (this.getLength() / this.maxSpeed) * MILLISECONDS_IN_SECOND,
    )
  }

  public getStart = () => {
    return this.getC0()
  }

  public getEnd = () => {
    return this.getC3()
  }

  public getDesiredEntryVelocity = () => {
    return this.entryVelocity
  }

  public getExpectedExitVelocity = () => {
    return this.exitVelocity
  }

  public generateToolpath = () => {
    const move: PlannerMovementMove = {
      duration: this.getDuration(),
      type: MovementMoveType.BEZIER_CUBIC,
      reference: MovementMoveReference.ABSOLUTE,
      points: [
        this.getC0().toArray(),
        this.getC1().toArray(),
        this.getC2().toArray(),
        this.getC3().toArray(),
      ],
    }

    return [move]
  }

  public getApproximateCentroid = () => {
    return getCentroid([this.getC0(), this.getC1(), this.getC2(), this.getC3()])
  }

  public samplePoint = (t: number) => {
    const curve = this.lazyGenerateCurve()

    return curve.getPoint(t)
  }

  public resetOptimisationState = () => {
    this.material = this.baseMaterial
    this.curve = null
  }

  public getTriggers = () => {
    return this.triggers
  }
}

function getCentroid(points: Vector3[]) {
  const centroid = new Vector3()
  for (const point of points) {
    centroid.add(point) // this doesn't mutate point
  }
  centroid.divideScalar(points.length)

  return centroid
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
  public overrideKeys = []

  public baseMaterial: Material
  private numSegments = 20

  constructor(
    private prePointMovement: Movement,
    private pointFrom: Point,
    private pointTo: Point,
    private postPointMovement: Movement,
    public material: Material,
  ) {
    super()
    this.baseMaterial = material
  }

  private curvePoints: Vector3[] = []
  private curvelength: number | null = null

  private curve: CatmullRomCurve3 | null = null

  public getPrePointMovement = () => {
    return this.prePointMovement
  }

  public getPointFrom = () => {
    return this.pointFrom
  }

  public getPointTo = () => {
    return this.pointTo
  }

  public getPostPointMovement = () => {
    return this.postPointMovement
  }

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
        this.getPrePointMovement().getEnd(),
        this.getPointFrom().getEnd(),
        this.getPointTo().getStart(),
        this.getPostPointMovement().getStart(),
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
    this.isFlipped = !this.isFlipped

    // Reset the curve
    this.curve = null
    // TODO: Flip the material
  }

  public flatten = () => {
    return [this]
  }

  public hydrate = (serialised: SerialisedTour) => {
    this.isFlipped = serialised[this.interFrameID]?.flipped
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
    return this.getPointFrom().getEnd()
  }

  public getEnd = () => {
    return this.getPointTo().getStart()
  }

  public getDesiredEntryVelocity = () => {
    return this.getPointFrom().getExpectedExitVelocity()
  }

  public getExpectedExitVelocity = () => {
    return this.getPointTo().getDesiredEntryVelocity()
  }

  public generateToolpath = () => {
    const move: PlannerMovementMove = {
      duration: this.getDuration(),
      type: MovementMoveType.CATMULL_SPLINE_LINEARISED,
      reference: MovementMoveReference.ABSOLUTE,
      points: [
        this.getPrePointMovement().getEnd().toArray(),
        this.getPointFrom().getEnd().toArray(),
        this.getPointTo().getStart().toArray(),
        this.getPostPointMovement().getStart().toArray(),
      ],
    }

    return [move]
  }

  public getApproximateCentroid = () => {
    return getCentroid([
      this.getPrePointMovement().getEnd(),
      this.getPointFrom().getEnd(),
      this.getPointTo().getStart(),
      this.getPostPointMovement().getStart(),
    ])
  }

  public resetOptimisationState = () => {
    this.material = this.baseMaterial
  }

  public getTriggers = () => {
    return this.triggers
  }
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
  public baseMaterial: Material

  constructor(
    private from: Line,
    private to: Line,
    public objectID: string, // Takes the ObjectID of the first line
    public overrideKeys: string[], // Takes the overrideKeys of the first line?
    public material: Material,
  ) {
    super()
    this.baseMaterial = material
  }

  public getFrom = () => {
    return this.from
  }

  public getTo = () => {
    return this.to
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
      this.getFrom().getEnd(),
      this.getFrom().getTo(),
      this.getTo().getFrom(),
      this.getTo().getStart(),
    )

    this.curve.arcLengthDivisions = 20 // divide into 20 segments for length calculations

    return this.curve
  }

  // Swap the ordering of this transition movement
  public flip = () => {
    this.isFlipped = !this.isFlipped

    // TODO: Flip the material
  }

  public flatten = () => {
    return [this]
  }

  public hydrate = (serialised: SerialisedTour) => {
    this.isFlipped = serialised[this.interFrameID]?.flipped
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
    return this.getFrom().getEnd()
  }

  public getEnd = () => {
    return this.getTo().getStart()
  }

  public getDesiredEntryVelocity = () => {
    return this.getFrom().getExpectedExitVelocity()
  }

  public getExpectedExitVelocity = () => {
    return this.getTo().getDesiredEntryVelocity()
  }

  public generateToolpath = () => {
    const move: PlannerMovementMove = {
      duration: this.getDuration(),
      type: MovementMoveType.BEZIER_CUBIC_LINEARISED,
      reference: MovementMoveReference.ABSOLUTE,
      points: [
        this.getFrom().getEnd().toArray(),
        this.getFrom().getTo().toArray(),
        this.getTo().getFrom().toArray(),
        this.getTo().getStart().toArray(),
      ],
    }

    return [move]
  }

  public getApproximateCentroid = () => {
    return getCentroid([this.getStart(), this.getEnd()])
  }

  public samplePoint = (t: number) => {
    const curve = this.lazyGenerateCurve()

    return curve.getPoint(t)
  }

  public resetOptimisationState = () => {
    this.material = this.baseMaterial
  }

  public getTriggers = () => {
    return this.triggers
  }
}

/**
 * A `Transit` is an invisible move to a point without care for the path taken
 */
export class Transit extends Movement {
  readonly type = MOVEMENT_TYPE.TRANSIT

  public baseMaterial: Material

  constructor(
    public endPoint: Vector3,
    public duration: number, // Must be above 0ms
    public material: Material,
    public objectID: string,
    public overrideKeys: string[],
  ) {
    super()
    this.baseMaterial = material

    if (this.duration < 1) {
      throw new Error(`Transit durations must be at least 1ms`)
    }
  }

  // Flipping a Transit does nothing
  public flip = () => {}

  public flatten = () => {
    return [this]
  }

  public hydrate = (serialised: SerialisedTour) => {
    this.isFlipped = serialised[this.interFrameID]?.flipped
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
    }

    return [move]
  }

  public getApproximateCentroid = () => {
    return this.endPoint
  }

  public samplePoint = (t: number) => {
    // Transits exist at their endpoint
    return this.endPoint
  }

  public resetOptimisationState = () => {
    this.material = this.baseMaterial
  }

  public getTriggers = () => {
    return this.triggers
  }
}

function pointsToControlPoints(
  inputArray: Vector3[],
): [Vector3, Vector3, Vector3, Vector3][] {
  if (inputArray.length < 4) {
    throw new Error(`Need at least 4 points to produce control points`)
  }

  return Array.from(
    { length: inputArray.length - (4 - 1) }, // precompute length of array of windows, size 4
    (_, index) => inputArray.slice(index, index + 4), // map over that to produce the windows
  ) as [Vector3, Vector3, Vector3, Vector3][]
}

interface DeconstructedCatmullCurve {
  threeCurve: CatmullRomCurve3
  controlPoints: [Vector3, Vector3, Vector3, Vector3]
  duration: number
}

/**
 * A transition is a move from one Point to another.
 *
 * Uses a catmull spline to visit points
 */
export class CatmullChain extends Movement {
  readonly type = MOVEMENT_TYPE.CATMULL_CHAIN

  public baseMaterial: Material

  // Maximum speed in millimeters per second
  public maxSpeed: number = defaultSpeed

  private numSegments = 20

  constructor(
    public points: Vector3[],
    public material: Material,
    public objectID: string,
    public overrideKeys: string[],
  ) {
    super()
    this.baseMaterial = material
  }

  private curvelength: number | null = null

  private curve: DeconstructedCatmullCurve[] | null = null

  private lazyGenerateCurve = () => {
    if (this.curve) return this.curve

    const points = this.isFlipped
      ? this.points.slice().reverse()
      : this.points.slice()

    const controlPointsArr = pointsToControlPoints(points)

    const deconstructed: DeconstructedCatmullCurve[] = []

    for (const controlPoints of controlPointsArr) {
      const threeCurve = new CatmullRomCurve3(
        controlPoints,
        false,
        'catmullrom',
      )
      threeCurve.arcLengthDivisions = 20

      const length = threeCurve.getLength()
      const duration = Math.ceil(
        (length / this.maxSpeed) * MILLISECONDS_IN_SECOND,
      ) // duration in milliseconds

      const decon: DeconstructedCatmullCurve = {
        threeCurve,
        controlPoints,
        duration,
      }

      deconstructed.push(decon)
    }

    this.curve = deconstructed

    return this.curve
  }

  private lazyGenerateCurveLength = () => {
    if (this.curvelength) return this.curvelength

    const curve = this.lazyGenerateCurve()

    const length = curve.reduce(
      (prev, curr) => prev + curr.threeCurve.getLength(),
      0,
    )

    return length
  }

  public samplePoint = (t: number) => {
    const curve = this.lazyGenerateCurve()
    const duration = this.getDuration()

    const absoluteTimeAlongCurve = t * duration

    let durationAccumulator = 0

    for (let index = 0; index < curve.length; index++) {
      const deconstructed = curve[index]

      const movementStartTime = durationAccumulator
      const movementEndTime = durationAccumulator + deconstructed.duration

      if (
        absoluteTimeAlongCurve >= movementStartTime &&
        absoluteTimeAlongCurve <= movementEndTime
      ) {
        // the point in time T is in this part of the curve
        const relT = MathUtils.mapLinear(
          absoluteTimeAlongCurve,
          movementStartTime,
          movementEndTime,
          1 / 3,
          2 / 3,
        )

        let p
        try {
          p = deconstructed.threeCurve.getPoint(relT)
        } catch (e) {
          debugger
          p = new Vector3(0, 0, 0)
        }
        return p
      }

      durationAccumulator = movementEndTime
    }

    // This should be unreachable

    throw new Error('Unreachable')
  }

  // Swap the ordering of this transition movement
  public flip = () => {
    this.isFlipped = !this.isFlipped

    // Reset the curve
    this.curve = null
    // TODO: Flip the material
  }

  public flatten = () => {
    return [this]
  }

  public hydrate = (serialised: SerialisedTour) => {
    this.isFlipped = serialised[this.interFrameID]?.flipped
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
    const curve = this.lazyGenerateCurve()

    const duration = curve.reduce((prev, curr) => prev + curr.duration, 0)

    return duration
  }

  public getStart = () => {
    return this.isFlipped ? this.points[this.points.length - 1] : this.points[0]
  }

  public getEnd = () => {
    return this.isFlipped ? this.points[0] : this.points[this.points.length - 1]
  }

  public getDesiredEntryVelocity = () => {
    return this.samplePoint(0.05)
      .clone()
      .sub(this.samplePoint(0))
      .normalize()
      .multiplyScalar(this.maxSpeed)
  }

  public getExpectedExitVelocity = () => {
    return this.samplePoint(1)
      .clone()
      .sub(this.samplePoint(0.95))
      .normalize()
      .multiplyScalar(this.maxSpeed)
  }

  public generateToolpath = () => {
    const curve = this.lazyGenerateCurve()

    const moves = []

    for (const deconstructed of curve) {
      const move: PlannerMovementMove = {
        duration: deconstructed.duration,
        type: MovementMoveType.CATMULL_SPLINE_LINEARISED,
        reference: MovementMoveReference.ABSOLUTE,
        points: deconstructed.controlPoints.map(vecs => vecs.toArray()),
      }

      moves.push(move)
    }

    return moves
  }

  public getApproximateCentroid = () => {
    return getCentroid([
      this.samplePoint(0),
      this.samplePoint(0.2),
      this.samplePoint(0.4),
      this.samplePoint(0.6),
      this.samplePoint(0.8),
      this.samplePoint(1),
    ])
  }

  public resetOptimisationState = () => {
    this.material = this.baseMaterial
  }

  public getTriggers = () => {
    return this.triggers
  }
}
