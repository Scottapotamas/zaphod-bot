import { Vector3 } from 'three'

import { MOVEMENT_TYPE } from './movement_types'

import type {
  Movement,
  MovementGroup,
  Line,
  Point,
  Transition,
  PointTransition,
  InterLineTransition,
  Transit,
  CatmullChain,
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

export function isTransition(movement: Movement): movement is Transition {
  return movement.type === MOVEMENT_TYPE.TRANSITION
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

  return speed
}

export function bezierFromHermiteForm(
  start: Vector3,
  startVelocity: Vector3,
  end: Vector3,
  endVelocity: Vector3,
) {
  const controlPoint0 = start
  const controlPoint1 = start
    .clone()
    .add(startVelocity.clone().multiplyScalar(0.3))
  const controlPoint2 = end.clone().sub(endVelocity.clone().multiplyScalar(0.3))
  const controlPoint3 = end
}
