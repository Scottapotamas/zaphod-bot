import { Vector3 } from 'three'

import { MOVEMENT_TYPE } from './movement_types'

import type {
  Movement,
  MovementGroup,
  Line,
  Point,
  Bezier,
  PointTransition,
  InterLineTransition,
  Transit,
  CatmullChain,
  ConstantSpeedBezier,
} from './movements'

export const MILLISECONDS_IN_SECOND = 1000

export type DenseMovements = Movement[] & { __dense: true }

export function declareDense(movements: Movement[]) {
  return movements as unknown as DenseMovements
}

export function isMovementGroup(movement: Movement): movement is MovementGroup {
  return movement.type === MOVEMENT_TYPE.GROUP
}

export function isLine(movement: Movement): movement is Line {
  return movement.type === MOVEMENT_TYPE.LINE
}

export function isPoint(movement: Movement): movement is Point {
  return movement.type === MOVEMENT_TYPE.POINT
}

export function isBezier(movement: Movement): movement is Bezier {
  return movement.type === MOVEMENT_TYPE.BEZIER
}

export function isConstantSpeedBezier(
  movement: Movement,
): movement is ConstantSpeedBezier {
  return movement.type === MOVEMENT_TYPE.BEZIER_CONSTANT_SPEED
}

export function isTransition(movement: Movement) {
  return movement.isTransition
}

export function isPointTransition(
  movement: Movement,
): movement is PointTransition {
  return movement.type === MOVEMENT_TYPE.POINT_TRANSITION
}

export function isInterLineTransition(
  movement: Movement,
): movement is InterLineTransition {
  return movement.type === MOVEMENT_TYPE.INTER_LINE_TRANSITION
}

export function isTransit(movement: Movement): movement is Transit {
  return movement.type === MOVEMENT_TYPE.TRANSIT
}

export function isCatmullChain(movement: Movement): movement is CatmullChain {
  return movement.type === MOVEMENT_TYPE.CATMULL_CHAIN
}

// For a given movement, predict the velocity at T
export function predictVelocityAtT(movement: Movement, t: number) {
  // 1% parts, should be accurate enough
  const tRange = 0.01
  const halfT = tRange / 2

  // Calculate T slightly earlier and slightly later
  const tLeft = Math.max(0, t - halfT)
  const tRight = Math.min(1, t + halfT)

  // Calculate the positions at those points
  const pLeft = movement.samplePoint(tLeft)
  const pRight = movement.samplePoint(tRight)

  // Calculate distance
  const distance = pLeft.distanceTo(pRight)

  // Calculate duration across that time
  const movementDuration = movement.getDuration()
  const tRangeActual = tRight - tLeft

  const timeDelta = tRangeActual * movementDuration

  const speed = (distance * MILLISECONDS_IN_SECOND) / timeDelta

  return pRight.clone().sub(pLeft).normalize().multiplyScalar(speed)
}

export function predictSpeedAtT(movement: Movement, t: number) {
  return predictVelocityAtT(movement, t).length()
}

export function findHighestApproximateSpeedAndT(movement: Movement) {
  let maxSpeed = 0
  const numSegments = 20
  let tOfHighestSpeed = 0

  for (let index = 0; index <= numSegments; index++) {
    const t = index / numSegments
    const speed = predictSpeedAtT(movement, t)

    if (speed > maxSpeed) {
      maxSpeed = speed
      tOfHighestSpeed = t
    }
  }

  return { maxSpeed, tOfHighestSpeed }
}

/**
 * Returns if the difference between a and b is less than the tolerance
 */
export function fcmp(a: number, b: number, tolerance: number) {
  return Math.abs(a - b) < tolerance
}
