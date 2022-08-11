import {
  declareDense,
  DenseMovements,
  deserialiseTour,
  InterLineTransition,
  isLine,
  isMovementGroup,
  isPoint,
  isTransit,
  Line,
  MILLISECONDS_IN_SECOND,
  Movement,
  Point,
  PointTransition,
  SerialisedTour,
  serialiseTour,
  Transit,
  Transition,
  TRANSITION_OBJECT_ID,
} from './movements'

import { Settings } from './settings'
import { Line3, MathUtils, Vector3 } from 'three'
import { defaultTransitionMaterial } from './material'
import { MixMaterial } from './materials/Mix'
import xxhash, { XXHash } from 'xxhash-wasm'
import { Permutor } from './permutor'
import assert from 'assert'

/**
 * Flatten any grouped movements into simple movements
 */
function flattenSparseBag(sparseBag: Movement[]): DenseMovements {
  let denseFlatList: DenseMovements = declareDense([])

  for (let index = 0; index < sparseBag.length; index++) {
    const movement = sparseBag[index]

    const flattened = movement.flatten()

    for (let i = 0; i < flattened.length; i++) {
      const f = flattened[i]
      denseFlatList.push(f)
    }
  }

  return denseFlatList
}

/**
 * Add transition movements between each move
 */
export function sparseToDense(
  sparseBag: Movement[],
  settings: Settings,
): DenseMovements {
  if (sparseBag.length === 0) {
    // If there are no moves, return an empty dense bag
    return declareDense([])
  }

  const flattened = flattenSparseBag(sparseBag)

  // Start with a Transit move to the start of the first movement
  let previousMovement: Movement = new Transit(
    flattened[0].getStart(),
    settings.optimisation.waitAtStartDuration,
    defaultTransitionMaterial,
    TRANSITION_OBJECT_ID,
  )

  const denseMovements: DenseMovements = declareDense([previousMovement])

  for (let index = 0; index < flattened.length; index++) {
    const movement = flattened[index]

    // Reset line lengths, etc
    movement.resetOptimisationState()

    // Set the max speed of the movement so velocities are scaled
    movement.setMaxSpeed(settings.optimisation.maxSpeed)

    // If the previous movement was a transit to the correct place for this movement, don't do a second transition
    if (
      isTransit(previousMovement) &&
      previousMovement.getEnd().distanceTo(movement.getStart()) < 1
    ) {
      // Add the movement to the dense bag
      denseMovements.push(movement)

      // Update the last movement
      previousMovement = movement
      continue
    }

    // If the last movement and this movement are both lines, and the line from the start of the previous movement to the end of the next movement
    // contains the start and end points of both lines (they are collinear), then the transition should be a straight line between them
    if (
      settings.optimisation.smoothInterlineTransitions &&
      isLine(previousMovement) &&
      isLine(movement)
    ) {
      const line = new Line3(previousMovement.getStart(), movement.getEnd())

      const closestPME = new Vector3()
      const closestMS = new Vector3()

      line.closestPointToPoint(previousMovement.getEnd(), true, closestPME)
      line.closestPointToPoint(movement.getStart(), true, closestMS)

      if (
        closestPME.distanceTo(previousMovement.getEnd()) < 1 &&
        closestMS.distanceTo(movement.getStart()) < 1
      ) {
        const interLineTransition = new Line(
          previousMovement.getEnd(),
          movement.getStart(),
          defaultTransitionMaterial,
          TRANSITION_OBJECT_ID, // Take the object ID of this movement, they're probably the same.
        )

        // These should really be the same... TODO: unify speeds
        interLineTransition.setMaxSpeed(
          settings.optimisation.transitionMaxSpeed,
        )

        // Add the transition to the dense bag
        denseMovements.push(interLineTransition)

        // Add the movement to the dense bag
        denseMovements.push(movement)

        // Update the last movement
        previousMovement = movement
        continue
      }
    }

    // If the last movement and this movement are both lines, and their end and start points match up
    // And their velocity angles aren't too dissimilar, reduce the length of the lines and do a transition inline
    if (
      settings.optimisation.smoothInterlineTransitions &&
      isLine(previousMovement) &&
      isLine(movement) &&
      previousMovement.getEnd().distanceTo(movement.getStart()) < 1 &&
      previousMovement
        .getExpectedExitVelocity()
        .clone()
        .normalize()
        .angleTo(movement.getDesiredEntryVelocity().clone().normalize()) <
        MathUtils.degToRad(settings.optimisation.interLineTransitionAngle)
    ) {
      // Shrink the end of the previousLine
      // Shrink the start of the current line

      const previousLine = previousMovement as Line
      const currentLine = movement as Line

      previousLine.shrinkEndByDistance(
        settings.optimisation.interLineTransitionShaveDistance,
      )
      currentLine.shrinkStartByDistance(
        settings.optimisation.interLineTransitionShaveDistance,
      )

      const interLineTransition = new InterLineTransition(
        previousMovement,
        movement,
        movement.objectID, // Take the object ID of this movement, they're probably the same.
        new MixMaterial(previousMovement.material, movement.material),
      )

      interLineTransition.setMaxSpeed(settings.optimisation.transitionMaxSpeed)

      // Add the transition to the dense bag
      denseMovements.push(interLineTransition)

      // Add the movement to the dense bag
      denseMovements.push(movement)

      // Update the last movement
      previousMovement = movement
      continue
    }

    // Overrides any other behaviour
    if (settings.optimisation.disableShapedTransitions) {
      // Build our transition movement from the old movement to the new, just use a simple Line
      const transit = new Line(
        previousMovement.getEnd(),
        movement.getStart(),
        defaultTransitionMaterial,
        TRANSITION_OBJECT_ID,
      )
      transit.setMaxSpeed(settings.optimisation.transitionMaxSpeed)

      // Add the transition to the dense bag
      denseMovements.push(transit)

      // Add the movement to the dense bag
      denseMovements.push(movement)

      // Update the last movement
      previousMovement = movement
      continue
    }

    // Points are visited by catmulls if their duration is 0, and if we have available control points
    if (
      isPoint(previousMovement) &&
      isPoint(movement) &&
      movement.duration === 0 &&
      index >= 2 &&
      index < flattened.length - 1
    ) {
      const movementPrevPrev = flattened[index - 2]
      const movementPrev = previousMovement
      const movementCurrent = movement
      const movementNext = flattened[index + 1]

      // Build our transition movement from the old movement to the new
      // At the end of this move the end effector will be at the point to draw
      // Instead of stopping, we use a material to draw the pulse right at the last moment
      // and continue with the chain of catmulls.
      const blinkAtEnd = new MixMaterial(defaultTransitionMaterial, movement.material, 0.999)

      const transition = new PointTransition(
        movementPrevPrev,
        movementPrev,
        movementCurrent,
        movementNext,
        blinkAtEnd,
      )

      transition.setMaxSpeed(settings.optimisation.transitionMaxSpeed)

      // Add the transition to the dense bag, it handles the point.
      denseMovements.push(transition)

      // Update the last movement
      previousMovement = movement
      continue
    }

    if (settings.optimisation.lineRunUp > 0 && isLine(movement)) {
      const endPoint = movement.getStart()
      const backwardDirection = movement
        .getDesiredEntryVelocity()
        .normalize()
        .multiplyScalar(-1)
      const startPoint = backwardDirection
        .multiplyScalar(settings.optimisation.lineRunUp * movement.getLength())
        .add(endPoint)

      const runUp = new Line(
        startPoint,
        endPoint,
        defaultTransitionMaterial,
        TRANSITION_OBJECT_ID,
      )
      // Set the speed to the incoming line's speed, not the transition speed.
      runUp.setMaxSpeed(settings.optimisation.maxSpeed)

      // Build our transition movement from the old movement to the new
      const transition = new Transition(
        previousMovement,
        runUp,
        defaultTransitionMaterial,
        settings.optimisation.transitionSize,
      )
      transition.setMaxSpeed(settings.optimisation.transitionMaxSpeed)

      // Add the transition to the dense bag
      denseMovements.push(transition)

      // Add the run up
      denseMovements.push(runUp)

      // Add the movement to the dense bag
      denseMovements.push(movement)

      const startPointRunOut = movement.getEnd()
      const backwardDirectionRunOut = movement
        .getExpectedExitVelocity()
        .normalize()
      const endPointRunOut = backwardDirectionRunOut
        .multiplyScalar(settings.optimisation.lineRunUp * movement.getLength())
        .add(startPointRunOut)

      const runOut = new Line(
        startPointRunOut,
        endPointRunOut,
        defaultTransitionMaterial,
        TRANSITION_OBJECT_ID,
      )
      // Set the speed to the incoming line's speed, not the transition speed.
      runOut.setMaxSpeed(settings.optimisation.maxSpeed)

      denseMovements.push(runOut)

      // Update the last movement
      previousMovement = runOut

      continue
    }

    // Build our transition movement from the old movement to the new
    const transition = new Transition(
      previousMovement,
      movement,
      defaultTransitionMaterial,
      settings.optimisation.transitionSize,
    )
    transition.setMaxSpeed(settings.optimisation.transitionMaxSpeed)

    // Add the transition to the dense bag
    denseMovements.push(transition)

    // Add the movement to the dense bag
    denseMovements.push(movement)

    // Update the last movement
    previousMovement = movement
    continue
  }

  // Flattened the movements
  return denseMovements
}

export function getTotalDuration(denseMoves: DenseMovements) {
  // For now, use the total duration as the cost function
  let cost = 0

  for (let index = 0; index < denseMoves.length; index++) {
    const movement = denseMoves[index]
    cost += movement.getDuration()
  }

  return cost
}

/**
 * Take a sparse set of movements, calculate their total 'cost'
 */
export function sparseToCost(movements: Movement[]): number {
  let d2Total = 0

  for (let index = 1; index < movements.length; index++) {
    const movementP = movements[index - 1]
    const movement = movements[index]
    const d2 = movementP.getEnd().distanceTo(movement.getStart())

    d2Total += d2
  }

  // for (let index = 1; index < movements.length; index++) {
  //   const movementP = movements[index - 1]
  //   const movement = movements[index]
  //   const d2 = new Transition(movementP, movement, defaultTransitionMaterial)

  //   d2Total += d2.getLength()
  // }

  return d2Total
}

export function hashTour(
  movements: Movement[],
  createHasher: (seed?: number) => XXHash<number>,
) {
  const hasher = createHasher()

  for (let index = 0; index < movements.length; index++) {
    const movement = movements[index]
    hasher.update(movement.interFrameID)
    hasher.update(movement.isFlipped ? 'f' : 'n')
  }

  return hasher.digest()
}

function applyOperations(
  ordering: Movement[],
  copyToAlternates: { signal: boolean },
  queue: Map<number, SerialisedTour>,
  enqueued: Set<number>,
  leftIndex: number,
  rightIndex: number,
  flipLeft: boolean,
  flipRight: boolean,
  swap: boolean,
  createHasher: (seed?: number) => XXHash<number>,
) {
  // If doing it for real, apply to the mutable movements
  if (!copyToAlternates.signal) {
    if (flipLeft) {
      ordering[leftIndex].flip()
    }
    if (flipRight) {
      ordering[rightIndex].flip()
    }
    if (swap) {
      const temp = ordering[leftIndex]
      ordering[leftIndex] = ordering[rightIndex]
      ordering[rightIndex] = temp
    }

    // All subsequent operations should copy to alternates
    copyToAlternates.signal = true

    let leftFlippedAfter = ordering[leftIndex].isFlipped
    let rightFlippedAfter = ordering[rightIndex].isFlipped
  } else {
    // Create an alternate, only copy the array if we're swapping the ordering
    const mutableOrdering = swap ? ordering.slice() : ordering

    if (flipLeft) {
      mutableOrdering[leftIndex].flip()
    }
    if (flipRight) {
      mutableOrdering[rightIndex].flip()
    }
    if (swap) {
      // this is done on a copy
      const temp = mutableOrdering[leftIndex]
      mutableOrdering[leftIndex] = mutableOrdering[rightIndex]
      mutableOrdering[rightIndex] = temp
    }

    const hash = hashTour(mutableOrdering, createHasher)

    // Only add this tour possibility if we haven't seen it before
    if (!enqueued.has(hash)) {
      enqueued.add(hash)
      queue.set(hash, serialiseTour(mutableOrdering))
    }

    // Flip the movement in the original ordering back
    // If we swapped the ordering for this queue addition,
    // doesn't matter since we're operating on the original index in the original array
    if (flipLeft) {
      ordering[leftIndex].flip()
    }
    if (flipRight) {
      ordering[rightIndex].flip()
    }
  }
}

export function swap(array: any[], a: number, b: number) {
  const temp = array[a]
  array[a] = array[b]
  array[b] = temp
}

export interface Progress {
  duration: number
  text: string
  serialisedTour: SerialisedTour
  // Whether this is the final update of this run
  completed: boolean
  // Whether a minima was found
  minimaFound: boolean
  // How much wall time spent optimising
  timeSpent: number
  currentCost: number
  startingCost: number
}

export type Continue = boolean

function optimiseByCache(
  sparseBag: Movement[],
  serialisedTour: SerialisedTour,
) {
  return deserialiseTour(sparseBag, serialisedTour)
}

// Search happens so fast we don't bother having it be cancellable
export function* optimiseBySearch(
  sparseBag: Movement[],
  createHasher: (seed?: number) => XXHash<number>,
  stopAfter: { current: number },
): Generator<OptimiserResult> {
  const start = performance.now()

  const best = {
    tour: serialiseTour(sparseBag),
    hash: hashTour(sparseBag, createHasher),
    cost: sparseToCost(sparseBag),
  }

  if (sparseBag.length < 2) {
    yield {
      iterations: 0,
      completed: true,
      time: performance.now() - start,
      best,
    }
    return
  }

  const toOrder: Movement[] = sparseBag.slice() // Copy the array

  // Start from the first movement
  let previousMovement = toOrder[0] // splice returns an array of the deleted items, index 0 is our starting point
  const nnOrdering = [previousMovement]

  // Find the closest next movement, potentially flipping it
  while (toOrder.length > 0) {
    let closest: Movement = previousMovement
    let closestDistance = Infinity
    let closestIndex = 0

    for (let index = 0; index < toOrder.length; index++) {
      const movement = toOrder[index]

      // Check if the end of the next movement is closer
      let d = movement.getStart().distanceTo(previousMovement.getEnd())

      if (d < closestDistance) {
        closestDistance = d
        closest = movement
        closestIndex = index
      }

      // Try flipping it
      movement.flip()

      d = movement.getStart().distanceTo(previousMovement.getEnd())

      // If it is closer, leave it flipped
      if (d < closestDistance) {
        closestDistance = d
        closest = movement
        closestIndex = index
        continue
      }

      // Otherwise flip it back
      movement.flip()
    }

    // We've found the next closest, pop it off
    toOrder.splice(closestIndex, 1)

    // And add it to the nnOrdered array
    nnOrdering.push(closest)

    // Update the previousMovement
    previousMovement = closest
  }

  best.tour = serialiseTour(nnOrdering)
  best.hash = hashTour(nnOrdering, createHasher)
  best.cost = sparseToCost(nnOrdering)

  yield {
    iterations: toOrder.length,
    completed: true,
    time: performance.now() - start,
    best,
  }
  return
}

// Iterates over a tour and flips movements to reduce distance
export function orientTour(sparseBag: Movement[]) {
  for (let index = 0; index < sparseBag.length - 1; index++) {
    const movement = sparseBag[index]
    const movementNext = sparseBag[index + 1]

    const unflippedDistance = movement
      .getEnd()
      .distanceTo(movementNext.getStart())
    const flippedDistance = movement.getEnd().distanceTo(movementNext.getEnd())

    if (flippedDistance < unflippedDistance) {
      movementNext.flip()
    }
  }
}

export function optimalFlippingForTour(sparseBag: Movement[]) {
  if (sparseBag.length === 0) return

  // Orient the tour, calculate the cost
  orientTour(sparseBag)
  const unflippedFirstCost = sparseToCost(sparseBag)

  // Flip the first movement, reorient the tour, calculate the cost
  sparseBag[0].flip()
  orientTour(sparseBag)
  const flippedFirstCost = sparseToCost(sparseBag)

  // If flipped is better, just return, otherwise flip it all back
  if (flippedFirstCost < unflippedFirstCost) {
    return
  } else {
    sparseBag[0].flip()
    orientTour(sparseBag)
  }
}

export interface BestTour {
  tour: SerialisedTour
  hash: number
  cost: number
}
export interface OptimiserResult {
  completed: boolean
  iterations: number
  time: number
  best: BestTour
}

/**
 * Warning, has factorial time complexity.
 *
 * Takes 5s for 10 movements
 * Takes 15s for 11 movements
 * Takes 112s for 12 movements
 * Don't bother after 10.
 */

export function* optimiseBruteForce(
  sparseBag: Movement[],
  createHasher: (seed?: number) => XXHash<number>,
  stopAfter: { current: number },
): Generator<OptimiserResult> {
  const best = {
    tour: serialiseTour(sparseBag),
    hash: hashTour(sparseBag, createHasher),
    cost: sparseToCost(sparseBag),
  }

  const permutor = new Permutor(sparseBag)

  const start = performance.now()

  let tourIndex = 0

  while (permutor.hasNext()) {
    const time = performance.now() - start

    // Check if the time limit has been exceeded
    if (performance.now() > stopAfter.current) {
      yield { iterations: tourIndex + 1, completed: false, time: time, best }
    }

    tourIndex++

    const currentOrdering = permutor.next()
    optimalFlippingForTour(currentOrdering)
    const cost = sparseToCost(currentOrdering)

    // if (tourIndex % 10000) {
    //   console.log(permutor.getIterations() / permutor.getTotal(), best.cost)
    // }

    if (cost < best.cost) {
      const hash = hashTour(currentOrdering, createHasher)

      // If we have a new best tour, set it
      best.tour = serialiseTour(currentOrdering)
      best.hash = hash
      best.cost = cost
    }
  }

  yield {
    iterations: tourIndex + 1,
    completed: true,
    time: performance.now() - start,
    best,
  }
}

const n = false
const f = true

function d(
  first: Movement,
  firstFlipped: boolean,
  second: Movement,
  secondFlipped: boolean,
) {
  if (!firstFlipped && !secondFlipped) {
    return first.getEnd().distanceTo(second.getStart())
  } else if (firstFlipped && !secondFlipped) {
    return first.getStart().distanceTo(second.getStart())
  } else if (!firstFlipped && secondFlipped) {
    return first.getEnd().distanceTo(second.getEnd())
  } else if (firstFlipped && secondFlipped) {
    return first.getStart().distanceTo(second.getEnd())
  }

  throw new Error(`Unreachable`)
}

/**
 * Cannot be called with a tour below 4 moves!
 */
export function* optimise2Opt(
  sparseBag: Movement[],
  createHasher: (seed?: number) => XXHash<number>,
  stopAfter: { current: number },
): Generator<OptimiserResult> {
  const best = {
    tour: serialiseTour(sparseBag),
    hash: hashTour(sparseBag, createHasher),
    cost: sparseToCost(sparseBag),
  }

  // The alternate candidates list
  const queue: Map<number, SerialisedTour> = new Map()
  const enqueued: Set<number> = new Set()

  // Add every initial rotation of the initial tour to the queue
  for (let index = 0; index < sparseBag.length; index++) {
    // rotate it
    sparseBag.push(sparseBag.shift()!)

    const h = hashTour(sparseBag, createHasher)

    queue.set(h, serialiseTour(sparseBag))
    enqueued.add(h)
  }

  const initialHash = hashTour(sparseBag, createHasher)
  queue.set(initialHash, serialiseTour(sparseBag))
  enqueued.add(initialHash)

  const start = performance.now()

  let tourIndex = 0

  fetchQueue: while (queue.size > 0) {
    tourIndex++

    let [hash, serialised]: [number, SerialisedTour] = queue
      .entries()
      .next().value

    // Take a copy of the sparseBag so it doesn't get mutated underneath us
    // The movements might still be flipped, TODO: is this affecting us?
    let currentOrdering = deserialiseTour(sparseBag.slice(), serialised)

    // Remove the candidate from the queue
    queue.delete(hash)

    // Iteratively improve upon the candidate tour

    let cost = sparseToCost(currentOrdering)

    let improved = true

    while (improved) {
      improved = false

      const n = false
      const f = true

      for (let b = 1; b < currentOrdering.length - 2; b++) {
        const time = performance.now() - start

        let shouldYield = performance.now() > stopAfter.current

        // Check if the time limit has been exceeded, and yield mid computation
        if (shouldYield) {
          const serialisePreYield = serialiseTour(currentOrdering)

          const costPreYield = sparseToCost(currentOrdering)

          yield {
            iterations: tourIndex + 1,
            completed: false,
            time: time,
            best,
          }

          currentOrdering = deserialiseTour(currentOrdering, serialisePreYield)

          const costPostYield = sparseToCost(currentOrdering)

          const maintainedCost = costPreYield === costPostYield

          if (!maintainedCost) {
            // debugger
            console.warn(
              `Solver cost post yield not maintained, note this file and frame`,
            )
            cost = costPostYield
          }
        }

        for (let e = b + 1; e < currentOrdering.length - 1; e++) {
          let current = 0
          let flipL = 0
          let flipR = 0
          let flipLR = 0
          let swapLR = 0
          let flipLSwapLR = 0
          let flipRSwapLR = 0
          let flipLRSwapLR = 0

          let isLPoint = isPoint(currentOrdering[b])
          let isRPoint = isPoint(currentOrdering[e])

          // If they're next to each other, it's a special case
          if (e === b + 1) {
            const A = currentOrdering[b - 1]
            const B = currentOrdering[b]
            const C = currentOrdering[e]
            const D = currentOrdering[e + 1]

            current = d(A, n, B, n) + d(B, n, C, n) + d(C, n, D, n)
            swapLR = d(A, n, C, n) + d(C, n, B, n) + d(B, n, D, n)

            flipL = d(A, n, B, f) + d(B, f, C, n) + d(C, n, D, n)
            flipR = d(A, n, B, n) + d(B, n, C, f) + d(C, f, D, n)
            flipLR = d(A, n, B, f) + d(B, f, C, f) + d(C, f, D, n)
            flipLSwapLR = d(A, n, C, n) + d(C, n, B, f) + d(B, f, D, n)
            flipRSwapLR = d(A, n, C, f) + d(C, f, B, n) + d(B, n, D, n)
            flipLRSwapLR = d(A, n, C, f) + d(C, f, B, f) + d(B, f, D, n)
          } else {
            // The segments are not overlapping
            const A = currentOrdering[b - 1]
            const B = currentOrdering[b]
            const C = currentOrdering[b + 1]
            const D = currentOrdering[e - 1]
            const E = currentOrdering[e]
            const F = currentOrdering[e + 1]

            // Calculate the distances of the different segments, flipped or swapped
            current = d(A, n, B, n) + d(B, n, C, n) + d(D, n, E, n) + d(E, n, F, n) // prettier-ignore
            swapLR = d(A, n, E, n) + d(E, n, C, n) + d(D, n, B, n) + d(B, n, F, n) // prettier-ignore
            flipL = d(A, n, B, f) + d(B, f, C, n) + d(D, n, E, n) + d(E, n, F, n) // prettier-ignore
            flipR = d(A, n, B, n) + d(B, n, C, n) + d(D, n, E, f) + d(E, f, F, n) // prettier-ignore
            flipLR = d(A, n, B, f) + d(B, f, C, n) + d(D, n, E, f) + d(E, f, F, n) // prettier-ignore
            flipLSwapLR = d(A, n, E, n) + d(E, n, C, n) + d(D, n, B, f) + d(B, f, F, n) // prettier-ignore
            flipRSwapLR = d(A, n, E, f) + d(E, f, C, n) + d(D, n, B, n) + d(B, n, F, n) // prettier-ignore
            flipLRSwapLR = d(A, n, E, f) + d(E, f, C, n) + d(D, n, B, f) + d(B, f, F, n) // prettier-ignore
          }

          // Find the winner
          const smallest = Math.min(
            current,
            flipL,
            flipR,
            flipLR,
            swapLR,
            flipLSwapLR,
            flipRSwapLR,
            flipLRSwapLR,
          )

          // update the cost
          if (smallest !== current) {
            const copyToAlternates = { signal: false }

            const small = 0.1 // changes under a millimeter are fine, try all possibilities there

            if (!isLPoint && flipL - smallest < small) {
              applyOperations(currentOrdering, copyToAlternates, queue, enqueued, b, e, true, false, false, createHasher) // prettier-ignore
            }

            if (!isRPoint && flipR - smallest < small) {
              applyOperations(currentOrdering, copyToAlternates, queue, enqueued, b, e, false, true, false, createHasher) // prettier-ignore
            }

            if (!isLPoint && !isRPoint && flipLR - smallest < small) {
              applyOperations(currentOrdering, copyToAlternates, queue, enqueued, b, e, true, true, false, createHasher) // prettier-ignore
            }

            if (swapLR - smallest < small) {
              applyOperations(currentOrdering, copyToAlternates, queue, enqueued, b, e, false, false, true, createHasher) // prettier-ignore
            }

            if (!isLPoint && flipLSwapLR - smallest < small) {
              applyOperations(currentOrdering, copyToAlternates, queue, enqueued, b, e, true, false, true, createHasher) // prettier-ignore
            }

            if (!isRPoint && flipRSwapLR - smallest < small) {
              applyOperations(currentOrdering, copyToAlternates, queue, enqueued, b, e, false, true, true, createHasher) // prettier-ignore
            }

            if (!isLPoint && !isRPoint && flipLRSwapLR - smallest < small) {
              applyOperations(currentOrdering, copyToAlternates, queue, enqueued, b, e, true, true, true, createHasher) // prettier-ignore
            }
            const delta = smallest - current

            improved = true
            cost = cost + delta
          }
        }
      }

      // Final optimisation passes to rotate the tour
      {
        // Test for all rotations, removing the longest link if possible
        let longestLinkIndex = 0
        let longestLinkDistance = 0

        for (let index = 0; index < currentOrdering.length; index++) {
          const prev = currentOrdering[index]
          const current = currentOrdering[(index + 1) % currentOrdering.length]
          const distance = prev.getEnd().distanceTo(current.getStart())

          if (distance > longestLinkDistance) {
            longestLinkIndex = index
            longestLinkDistance = distance
          }
        }

        if (longestLinkIndex !== currentOrdering.length - 1) {
          const left = currentOrdering.slice(0, longestLinkIndex + 1)
          const right = currentOrdering.slice(longestLinkIndex + 1)

          // Reorder the arrays to remove the longest link
          currentOrdering = right.concat(left)
          const newLink = right[right.length - 1]
            .getEnd()
            .distanceTo(left[0].getStart())
          cost = cost - longestLinkDistance + newLink
        }
      }
      // The main iteration loop can't flip or swap the ends, so do that up here
      {
        // Test flipping the start
        const A = currentOrdering[0]
        const B = currentOrdering[1]

        const dAnB = A.getEnd().distanceTo(B.getStart())
        const dAfB = A.getStart().distanceTo(B.getStart())

        if (dAfB < dAnB) {
          A.flip()

          const delta = dAnB - dAfB

          cost = cost - delta
          improved = true
        }
      }
      {
        // Test flipping the end
        const E = currentOrdering[currentOrdering.length - 2]
        const F = currentOrdering[currentOrdering.length - 1]

        const dEFn = E.getEnd().distanceTo(F.getStart())
        const dEFf = E.getEnd().distanceTo(F.getEnd())

        if (dEFf < dEFn) {
          F.flip()

          const delta = dEFn - dEFf

          cost = cost - delta
          improved = true
        }
      }
      {
        // TODO: Test a flip
      }
    }

    if (cost < best.cost) {
      // If we have a new best tour, set it
      best.tour = serialiseTour(currentOrdering)
      best.hash = hash
      best.cost = cost
    }
  }

  yield {
    iterations: tourIndex + 1,
    completed: true,
    time: performance.now() - start,
    best,
  }
}

// 4hz updates will look good enough
const OPTIMISATION_TIME = 250

/**
 * Reorders and flips the members of a sparse bag of movements, optimising for the fastest tour.
 *
 * Async so we can interrupt the event queue to check for pausing, otherwise it'll just run.
 */
export async function optimise(
  sparseBag: Movement[],
  partialUpdate: boolean,
  settings: Settings,
  updateProgress: (progress: Progress) => Promise<Continue>,
  debugInfo: any,
  cache?: SerialisedTour,
) {
  const { create32: createHasher } = await xxhash()

  const startedOptimisation = performance.now()

  const startingCost = sparseToCost(sparseBag)

  // Partial updates just run a beam search

  if (partialUpdate) {
    const stopAfter = { current: startedOptimisation + OPTIMISATION_TIME }

    const beamSearched = optimiseBySearch(
      sparseBag,
      createHasher,
      stopAfter,
    ).next().value

    const currentDense = sparseToDense(
      deserialiseTour(sparseBag, beamSearched.best.tour),
      settings,
    )
    const curentDuration = getTotalDuration(currentDense)

    // Final status update
    await updateProgress({
      duration: getTotalDuration(currentDense),
      text: `Optimised to ${Math.round(curentDuration * 100) / 100}ms`,
      serialisedTour: beamSearched.best.tour,
      completed: true,
      minimaFound: false,
      timeSpent: performance.now() - startedOptimisation,
      startingCost,
      currentCost: sparseToCost(sparseBag),
    })

    return
  }

  let iterations = 0

  const stopAfter = { current: performance.now() + OPTIMISATION_TIME }

  for (const iteration of smartOptimiser(sparseBag, createHasher, stopAfter)) {
    iterations += iteration.iterations

    const deserialised = deserialiseTour(sparseBag, iteration.best.tour)

    const currentDense = sparseToDense(deserialised, settings)
    const currentDuration = getTotalDuration(currentDense)

    // Finish within the time it takes to render the frame no matter what
    const done =
      iteration.completed ||
      performance.now() - startedOptimisation > currentDuration

    const calculatedCost = sparseToCost(deserialised)
    const hash = hashTour(deserialised, createHasher)

    const shouldContinue = await updateProgress({
      duration: currentDuration,
      text: `${hash.toString(16)}: ${Math.round(iteration.best.cost * 10) / 10
        } (${Math.round(calculatedCost * 10) / 10}): ${Math.round(currentDuration * 100) / 100
        }ms`,
      serialisedTour: iteration.best.tour,
      completed: done,
      minimaFound: done,
      timeSpent: performance.now() - startedOptimisation,
      startingCost,
      currentCost: sparseToCost(sparseBag),
    })

    // Update the allowed time to iterate
    stopAfter.current = performance.now() + OPTIMISATION_TIME

    // If we've reached a minima, or should stop, or we've taken longer than the length of the tour to optimise, exit
    if (done || !shouldContinue) {
      return
    }
  }
}

export function* smartOptimiser(
  sparseBag: Movement[],
  createHasher: (seed?: number) => XXHash<number>,
  stopAfter: { current: number },
): Generator<OptimiserResult> {
  // Generate initial NN optimisation
  const nnRes: OptimiserResult = optimiseBySearch(
    sparseBag,
    createHasher,
    stopAfter,
  ).next().value

  // Optimise all MovementGroups as sub-tours
  for (let index = 0; index < sparseBag.length; index++) {
    const movement = sparseBag[index]

    if (isMovementGroup(movement)) {
      for (const subRes of smartOptimiser(
        movement.getMovements(),
        createHasher,
        stopAfter,
      )) {
        if (subRes.completed) {
          movement.hydrate(subRes.best.tour)
        } else {
          // During hierarchial optimisation, yield the original NN tour until we have a solve on a sub-tour
          // Once a sub-tour is optimised, the MovementGroup will be hydrated and the overall Tour will update
          yield nnRes
        }
      }
    }
  }

  // Any subtour with under 10 elements, perform a brute force solve
  if (sparseBag.length < 10) {
    for (const res of optimiseBruteForce(sparseBag, createHasher, stopAfter)) {
      if (res.completed) {
        yield res
        return
      }
      yield res
    }
  }

  // Any tours more complicated than 10 moves utilise 2-opt, seeded with a nearest neighbour search
  for (const res of optimise2Opt(
    deserialiseTour(sparseBag, nnRes.best.tour),
    createHasher,
    stopAfter,
  )) {
    if (res.completed) {
      yield res
      return
    }
    yield res
  }

  throw new Error(`unreachable`)
}

function debuggerIfCostOut(currentOrdering: Movement[], cost: number) {
  const calculatedCost = sparseToCost(currentOrdering)

  if (Math.abs(calculatedCost - cost) > 1) {
    debugger
    return calculatedCost
  }

  if (cost < 0) {
    debugger
    return calculatedCost
  }

  return calculatedCost
}
