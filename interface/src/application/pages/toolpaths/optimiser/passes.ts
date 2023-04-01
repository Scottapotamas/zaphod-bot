import {
  deserialiseTour,
  Line,
  Movement,
  ConstantSpeedCatmullRom,
  SerialisedTour,
  serialiseTour,
  Transit,
  Bezier,
  TRANSITION_OBJECT_ID,
  ConstantSpeedBezier,
} from './movements'

import {
  isLine,
  isMovementGroup,
  isPoint,
  isTransit,
  declareDense,
  DenseMovements,
  predictSpeedAtT,
  predictVelocityAtT,
  findHighestApproximateSpeedAndT,
  MILLISECONDS_IN_SECOND,
  isConstantSpeedBezier,
} from './movement_utilities'

import type { Settings } from './settings'
import { Line3, MathUtils, Vector3 } from 'three'
import { defaultTransitionMaterial } from './material'
import { MixMaterial } from './materials/Mix'
import xxhash, { XXHash } from 'xxhash-wasm'
import { Permutor } from './permutor'
import { RemapMaterial } from './materials/Remap'
import { ColorRampMaterial } from './materials/ColorRamp'
import { MATERIALS } from './materials/utilities'
import { Material } from './materials/Base'

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

const emptyOverrideKeys: string[] = []

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
    emptyOverrideKeys,
  )

  const denseMovements: DenseMovements = declareDense([previousMovement])

  // Reset optimisation states and max speeds of everything
  for (let index = 0; index < flattened.length; index++) {
    const movement = flattened[index]

    // Reset line lengths, the material etc
    movement.resetOptimisationState()

    // Set the max speed of the movement so velocities are scaled
    movement.setMaxSpeed(settings.optimisation.maxSpeed)
  }

  // Then do the optimisation pass
  for (let index = 0; index < flattened.length; index++) {
    const movement = flattened[index]

    // Clamp the max speed based on the length of the movement
    const clampedLength = MathUtils.clamp(
      movement.getLength(),
      1,
      settings.optimisation.rampToMaxSpeedDistance,
    )

    let maxSpeedForMovement = MathUtils.mapLinear(
      clampedLength,
      1,
      settings.optimisation.rampToMaxSpeedDistance,
      25,
      settings.optimisation.maxSpeed,
    )

    if (settings.optimisation.rampToMaxSpeedDistance === 0) {
      maxSpeedForMovement = settings.optimisation.maxSpeed
    }

    movement.setMaxSpeed(maxSpeedForMovement)

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
      settings.optimisation.mergeColinearLines &&
      isLine(previousMovement) &&
      isLine(movement)
    ) {
      const line = new Line3(previousMovement.getStart(), movement.getEnd())

      const closestPME = new Vector3()
      const closestMS = new Vector3()

      line.closestPointToPoint(previousMovement.getEnd(), true, closestPME)
      line.closestPointToPoint(movement.getStart(), true, closestMS)

      if (
        closestPME.distanceTo(previousMovement.getEnd()) === 0 &&
        closestMS.distanceTo(movement.getStart()) === 0
      ) {
        const colinearLineTransition = new Line(
          previousMovement.getEnd(),
          movement.getStart(),
          defaultTransitionMaterial,
          TRANSITION_OBJECT_ID, // Take the object ID of this movement, they're probably the same.
          emptyOverrideKeys,
        )
        colinearLineTransition.isTransition = true

        colinearLineTransition.setMaxSpeed(maxSpeedForMovement)

        // Dump <1mm movements
        if (colinearLineTransition.getLength() > 1) {
          // Add the transition to the dense bag
          denseMovements.push(colinearLineTransition)
        }

        // Add the movement to the dense bag
        denseMovements.push(movement)

        // Update the last movement
        previousMovement = movement
        continue
      }
    }

    // If the last movement and this movement have matching end and start points
    // and their velocity angles are _very_ similar
    // immediately snap between them without a transition
    if (
      settings.optimisation.smoothInterlineTransitions &&
      previousMovement.getEnd().distanceTo(movement.getStart()) < 1 &&
      previousMovement
      .getExpectedExitVelocity()
      .clone()
      .normalize()
      .angleTo(movement.getDesiredEntryVelocity().clone().normalize()) <
      MathUtils.degToRad(settings.optimisation.interLineTransitionLessAngle)
    ) {
      // Add the movement to the dense bag
      denseMovements.push(movement)

      // Update the last movement
      previousMovement = movement
      continue
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

      const previousLineMaterial = previousLine.material
      const currentLineMaterial = currentLine.material

      // Shrink the previous line by a distance, calculate the T values of the line now shown
      // Replace the material so over the new 0 -> 1 T value, it goes from 0 -> X

      // Calculate the shrink percentage
      const previousLineDistance = previousLine
        .getFrom()
        .distanceTo(previousLine.getTo())
      const previousLineShrinkPercentage = MathUtils.clamp(
        settings.optimisation.interLineTransitionShaveDistance /
          previousLineDistance,
        0,
        1,
      )

      // Do the shrink
      previousLine.shrinkEndByDistance(
        settings.optimisation.interLineTransitionShaveDistance,
      )

      // If shrinking that line makes its length 0, delete it from the denseMovements list
      if (previousLine.getLength() === 0) {
        denseMovements.pop()
      }

      // Shrink the next line by a distance, calculate the T values of the line now shown
      // Replace the material so over the new 0 -> 1 T value, it goes from Y -> 1

      const currentLineDistance = currentLine
        .getFrom()
        .distanceTo(currentLine.getTo())
      const currentLineShrinkPercentage = MathUtils.clamp(
        settings.optimisation.interLineTransitionShaveDistance /
          currentLineDistance,
        0,
        1,
      )

      // Do the shrink
      currentLine.shrinkStartByDistance(
        settings.optimisation.interLineTransitionShaveDistance,
      )

      // Split the materials up at the split points
      previousLine.material = new RemapMaterial(
        previousLineMaterial,
        0,
        1 - previousLineShrinkPercentage,
      )

      const remainderOfPreviousLineMaterial = new RemapMaterial(
        previousLineMaterial,
        1 - previousLineShrinkPercentage,
        1,
      )
      const remainderOfCurrentLineMaterial = new RemapMaterial(
        currentLineMaterial,
        0,
        currentLineShrinkPercentage,
      )

      currentLine.material = new RemapMaterial(
        currentLineMaterial,
        currentLineShrinkPercentage,
        1,
      )

      // Insert a transition line in between. Its material needs to go from (1-X) of the previous material to Y of the current material

      const interLineTransitions = generateInterLineTransition(
        previousLine,
        currentLine,
        new MixMaterial(
          remainderOfPreviousLineMaterial,
          remainderOfCurrentLineMaterial,
        ),
        maxSpeedForMovement,
        false,
      )

      // Add these transitions to the dense bag
      denseMovements.push(...interLineTransitions)
      previousMovement = interLineTransitions[interLineTransitions.length - 1]

      // Add the movement to the dense bag if it has length
      if (currentLine.getLength() > 0) {
        denseMovements.push(currentLine)
        previousMovement = currentLine
      }

      // Update the last movement
      continue
    }

    // If the last movement is a constant speed bezier and the next movement is a line,
    // and their end and start points match up
    // And their velocity angles aren't too dissimilar,
    // reduce the length of the next line and do a transition inline
    if (
      settings.optimisation.smoothInterlineTransitions &&
      isConstantSpeedBezier(previousMovement) &&
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

      const previousLine = previousMovement
      const currentLine = movement

      const previousLineMaterial = previousLine.material
      const currentLineMaterial = currentLine.material

      // Shrink the next line by a distance, calculate the T values of the line now shown
      // Replace the material so over the new 0 -> 1 T value, it goes from Y -> 1
      const previousLineDistance = previousLine.getLength()
      const previousLineShrinkPercentage = MathUtils.clamp(
        settings.optimisation.interLineTransitionShaveDistance /
          previousLineDistance,
        0,
        1,
      )

      const currentLineDistance = currentLine
        .getFrom()
        .distanceTo(currentLine.getTo())
      const currentLineShrinkPercentage = MathUtils.clamp(
        settings.optimisation.interLineTransitionShaveDistance /
          currentLineDistance,
        0,
        1,
      )

      // Do the shrink
      currentLine.shrinkStartByDistance(
        settings.optimisation.interLineTransitionShaveDistance,
      )

      // Split the materials up at the split points
      previousLine.material = new RemapMaterial(
        previousLineMaterial,
        0,
        1 - previousLineShrinkPercentage,
      )

      const remainderOfPreviousLineMaterial = new RemapMaterial(
        previousLineMaterial,
        1 - previousLineShrinkPercentage,
        1,
      )
      const remainderOfCurrentLineMaterial = new RemapMaterial(
        currentLineMaterial,
        0,
        currentLineShrinkPercentage,
      )

      currentLine.material = new RemapMaterial(
        currentLineMaterial,
        currentLineShrinkPercentage,
        1,
      )

      // Insert a transition line in between. Its material needs to go from (1-X) of the previous material to Y of the current material
      const { newPreviousMovementCSB, nextMovementCSB } =
        generateCSBezierToLineTransition(
          previousLine,
          currentLine,
          new MixMaterial(
            remainderOfPreviousLineMaterial,
            remainderOfCurrentLineMaterial,
          ),
          settings.optimisation.interLineTransitionShaveDistance,
          maxSpeedForMovement,
          false,
        )

      // Shrink the previous constant size bezier, we need to pop it off and add back our new one
      denseMovements.pop()

      // If newPreviousMovementCSB is null, it's shorter than our shave distance
      // and will just be removed
      if (newPreviousMovementCSB && newPreviousMovementCSB.getLength() > 0) {
        newPreviousMovementCSB.material = previousLine.material
        denseMovements.push(newPreviousMovementCSB)
      }

      // Add the inter-line transition to the dense bag
      denseMovements.push(nextMovementCSB)
      previousMovement = nextMovementCSB

      // Add the current line to the dense bag if it has length
      if (currentLine.getLength() > 0) {
        denseMovements.push(currentLine)
        previousMovement = currentLine
      }

      // Update the last movement
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
        emptyOverrideKeys,
      )
      transit.isTransition = true
      transit.setMaxSpeed(maxSpeedForMovement)

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
      const blinkAtEnd = new MixMaterial(
        defaultTransitionMaterial,
        movement.material,
        0.999,
      )

      const transition = new ConstantSpeedCatmullRom(
        movementPrevPrev.getEnd(),
        movementPrev.getEnd(),
        movementCurrent.getStart(),
        movementNext.getStart(),
        blinkAtEnd,
        movement.objectID,
      )
      transition.isTransition = true

      transition.setMaxSpeed(maxSpeedForMovement)

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
        [],
      )
      runUp.isTransition = true
      // Set the speed to the incoming line's speed, not the transition speed.
      runUp.setMaxSpeed(maxSpeedForMovement)

      // Build our transition movement from the old movement to the new
      denseMovements.push(
        ...generateBezierTransition(
          previousMovement,
          runUp,
          defaultTransitionMaterial,
          settings.optimisation.transitionSize,
          settings.optimisation.maxSpeed,
          true,
        ),
      )

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
        emptyOverrideKeys,
      )
      runOut.isTransition = true
      // Set the speed to the incoming line's speed, not the transition speed.
      runOut.setMaxSpeed(maxSpeedForMovement)

      denseMovements.push(runOut)

      // Update the last movement
      previousMovement = runOut

      continue
    }

    // Build our transition movement from the old movement to the new
    denseMovements.push(
      ...generateBezierTransition(
        previousMovement,
        movement,
        defaultTransitionMaterial,
        settings.optimisation.transitionSize,
        settings.optimisation.maxSpeed,
        true,
      ),
    )

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

export function* optimiseNoop(
  sparseBag: Movement[],
  createHasher: (seed?: number) => XXHash<number>,
): Generator<OptimiserResult> {
  const start = performance.now()

  const best = {
    tour: serialiseTour(sparseBag),
    hash: hashTour(sparseBag, createHasher),
    cost: sparseToCost(sparseBag),
  }

  yield {
    iterations: 0,
    completed: false,
    time: performance.now() - start,
    best,
  }

  return
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

  // Need at least 4 items to do our 2opt
  if (sparseBag.length < 4) {
    yield {
      iterations: 0,
      completed: true,
      time: 0,
      best,
    }
    return
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
            break
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

          if (!prev || !current) {
            debugger
          }

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

        if (!A || !B) {
          debugger
        }

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
  let currentOptimisationLevel: OptimiserResult = optimiseNoop(
    sparseBag,
    createHasher,
  ).next().value

  if (partialUpdate) {
    const stopAfter = { current: startedOptimisation + OPTIMISATION_TIME }

    if (settings.optimisation.passes.nearestNeighbour) {
      currentOptimisationLevel = optimiseBySearch(
        sparseBag,
        createHasher,
        stopAfter,
      ).next().value
    }

    const currentDense = sparseToDense(
      deserialiseTour(sparseBag, currentOptimisationLevel.best.tour),
      settings,
    )
    const curentDuration = getTotalDuration(currentDense)

    // Final status update
    await updateProgress({
      duration: getTotalDuration(currentDense),
      text: `Optimised to ${Math.round(curentDuration * 100) / 100}ms`,
      serialisedTour: currentOptimisationLevel.best.tour,
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

  for (const iteration of smartOptimiser(
    sparseBag,
    settings,
    createHasher,
    stopAfter,
  )) {
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
      text: `${hash.toString(16)}: ${
        Math.round(iteration.best.cost * 10) / 10
      } (${Math.round(calculatedCost * 10) / 10}): ${
        Math.round(currentDuration * 100) / 100
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

    currentOptimisationLevel = iteration

    // If we've reached a minima, or should stop, or we've taken longer than the length of the tour to optimise, exit
    if (done || !shouldContinue) {
      return
    }
  }

  const deserialised = deserialiseTour(
    sparseBag,
    currentOptimisationLevel.best.tour,
  )
  const currentDense = sparseToDense(deserialised, settings)
  const currentDuration = getTotalDuration(currentDense)

  const calculatedCost = sparseToCost(deserialised)
  const hash = hashTour(deserialised, createHasher)

  // final update
  await updateProgress({
    duration: currentDuration,
    text: `${hash.toString(16)}: ${
      Math.round(currentOptimisationLevel.best.cost * 10) / 10
    } (${Math.round(calculatedCost * 10) / 10}): ${
      Math.round(currentDuration * 100) / 100
    }ms`,
    serialisedTour: currentOptimisationLevel.best.tour,
    completed: true,
    minimaFound:
      currentOptimisationLevel.completed ||
      performance.now() - startedOptimisation > currentDuration,
    timeSpent: performance.now() - startedOptimisation,
    startingCost,
    currentCost: sparseToCost(sparseBag),
  })
}

export function* smartOptimiser(
  sparseBag: Movement[],
  settings: Settings,
  createHasher: (seed?: number) => XXHash<number>,
  stopAfter: { current: number },
): Generator<OptimiserResult> {
  // Todo have this be switchable

  // Generate initial NN optimisation

  let initialOptimisation: OptimiserResult = optimiseNoop(
    sparseBag,
    createHasher,
  ).next().value

  if (settings.optimisation.passes.nearestNeighbour) {
    initialOptimisation = optimiseBySearch(
      sparseBag,
      createHasher,
      stopAfter,
    ).next().value

    yield initialOptimisation
  }

  // Optimise all MovementGroups as sub-tours
  for (let index = 0; index < sparseBag.length; index++) {
    const movement = sparseBag[index]

    // Only optimise non-frozen movement groups
    if (isMovementGroup(movement) && !movement.frozen) {
      for (const subRes of smartOptimiser(
        movement.getMovements(),
        settings,
        createHasher,
        stopAfter,
      )) {
        if (subRes.completed) {
          movement.hydrate(subRes.best.tour)
        } else {
          // During hierarchial optimisation, yield the original NN tour until we have a solve on a sub-tour
          // Once a sub-tour is optimised, the MovementGroup will be hydrated and the overall Tour will update
          yield initialOptimisation
        }
      }
    }
  }

  // Any subtour with under 10 elements, perform a brute force solve
  if (sparseBag.length < 10 && settings.optimisation.passes.bruteForceSmall) {
    for (const res of optimiseBruteForce(sparseBag, createHasher, stopAfter)) {
      if (res.completed) {
        yield res
        return
      }
      yield res
    }
  }

  // Any tours more complicated than 10 moves utilise 2-opt, seeded with a nearest neighbour search
  if (settings.optimisation.passes.twoOpt) {
    for (const res of optimise2Opt(
      deserialiseTour(sparseBag, initialOptimisation.best.tour),
      createHasher,
      stopAfter,
    )) {
      if (res.completed) {
        yield res
        return
      }
      yield res
    }
  }

  // should be unreachable if we did any extra optimisations, otherwise just return the initial one
  initialOptimisation.completed = true
  yield initialOptimisation
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

function generateBezierTransition(
  previousMovement: Movement,
  nextMovement: Movement,
  material: Material,
  controlPointScalar: number,
  maxSpeed: number,
  markAsTransition: boolean,
) {
  const c0 = previousMovement.getEnd()
  const c1 = previousMovement
    .getEnd()
    .clone()
    .add(
      previousMovement
        .getExpectedExitVelocity()
        .clone()
        .multiplyScalar(controlPointScalar),
    )
  const c2 = nextMovement
    .getStart()
    .clone()
    .sub(
      nextMovement
        .getDesiredEntryVelocity()
        .clone()
        .multiplyScalar(controlPointScalar),
    )
  const c3 = nextMovement.getStart()

  const entryVelocity = previousMovement.getExpectedExitVelocity()
  const exitVelocity = nextMovement.getDesiredEntryVelocity()

  const singularTransition = new Bezier(
    c0,
    c1,
    c2,
    c3,
    entryVelocity,
    exitVelocity,
    material,
    TRANSITION_OBJECT_ID,
    true, // doesn't matter
  )
  singularTransition.isTransition = markAsTransition
  singularTransition.setMaxSpeed(maxSpeed)

  const midPointVelocity = predictVelocityAtT(singularTransition, 0.5)

  const midpointClampedSpeed = Math.min(midPointVelocity.length(), maxSpeed)

  const clampedMidPointVelocity = midPointVelocity
    .clone()
    .normalize()
    .multiplyScalar(midpointClampedSpeed)

  const {
    entryTransition,
    exitTransition,

    A,
    B,
    C,
    D,
    E,
    F,
    G,
  } = subdivideBezierAndClampSpeed(
    c0,
    c1,
    c2,
    c3,
    entryVelocity,
    clampedMidPointVelocity,
    exitVelocity,
    maxSpeed,
    material,
    markAsTransition,
    0.5,
  )

  const { maxSpeed: maxSpeedEntry } =
    findHighestApproximateSpeedAndT(entryTransition)

  const { maxSpeed: maxSpeedExit } =
    findHighestApproximateSpeedAndT(exitTransition)

  const transitions: Bezier[] = []

  if (maxSpeedEntry > maxSpeed) {
    // further subdivide the entry

    const {
      entryTransition: entryEntryTransition,
      exitTransition: entryExitTransition,
    } = subdivideBezierAndClampSpeed(
      A,
      B,
      C,
      D,
      entryVelocity,
      clampedMidPointVelocity,
      clampedMidPointVelocity,
      maxSpeed,
      material,
      markAsTransition,
      0.5,
    )

    transitions.push(entryEntryTransition, entryExitTransition)
  } else {
    transitions.push(entryTransition)
  }

  if (maxSpeedExit > maxSpeed) {
    // further subdivide the exit

    const {
      entryTransition: exitEntryTransition,
      exitTransition: exitExitTransition,
    } = subdivideBezierAndClampSpeed(
      D,
      E,
      F,
      G,
      clampedMidPointVelocity,
      clampedMidPointVelocity,
      exitVelocity,
      maxSpeed,
      material,
      markAsTransition,
      0.5,
    )

    transitions.push(exitEntryTransition, exitExitTransition)
  } else {
    transitions.push(exitTransition)
  }

  for (const [index, transition] of transitions.entries()) {
    checkTransitionSpeed(transition, `transition #${index}`)

    const { maxSpeed: maxSpeedOfTransitionPart } =
      findHighestApproximateSpeedAndT(transition)

    if (maxSpeedOfTransitionPart > maxSpeed) {
      const constantSpeedTransitionEntry = new ConstantSpeedBezier(
        A,
        B,
        C,
        D,
        material,
        TRANSITION_OBJECT_ID,
      )
      constantSpeedTransitionEntry.isTransition = markAsTransition
      constantSpeedTransitionEntry.setMaxSpeed(entryVelocity.length())

      const constantSpeedTransitionExit = new ConstantSpeedBezier(
        D,
        E,
        F,
        G,
        material,
        TRANSITION_OBJECT_ID,
      )
      constantSpeedTransitionExit.isTransition = markAsTransition
      constantSpeedTransitionExit.setMaxSpeed(exitVelocity.length())

      return [constantSpeedTransitionEntry, constantSpeedTransitionExit]
    }
  }

  return transitions
}

function generateInterLineTransition(
  previousMovement: Line,
  nextMovement: Line,
  material: Material,
  maxSpeed: number,
  markAsTransition: boolean,
) {
  // const trans = new Line(previousMovement.getEnd(), nextMovement.getStart(), material, previousMovement.objectID,[])

  // trans.isTransition = markAsTransition
  // trans.setMaxSpeed(maxSpeed)

  // return [trans]

  const trans = new ConstantSpeedBezier(
    previousMovement.getEnd(),
    previousMovement.getTo(),
    nextMovement.getFrom(),
    nextMovement.getStart(),
    material,
    nextMovement.objectID,
  )
  trans.isTransition = markAsTransition
  trans.setMaxSpeed(maxSpeed)

  return [trans]
}

function generateCSBezierToLineTransition(
  previousMovement: ConstantSpeedBezier,
  nextMovement: Line,
  material: Material,
  shaveDistance: number,
  maxSpeed: number,
  markAsTransition: boolean,
) {
  // Calculate the time T where we can split the previous bezier
  const bezier = new Bezier(
    previousMovement.getC0(),
    previousMovement.getC1(),
    previousMovement.getC2(),
    previousMovement.getC3(),
    previousMovement.getDesiredEntryVelocity(),
    previousMovement.getExpectedExitVelocity(),
    material,
    previousMovement.objectID,
    true,
  )

  let left = 0
  let right = 1
  let iters = 0

  let tVal = 0

  let keepPreviousMovement = bezier.getLength() > shaveDistance

  if (keepPreviousMovement) {
    while (true) {
      iters++

      tVal = (left + right) / 2

      // Calculate the distance from that point to the end of the bezier
      const distance = bezier
        .samplePoint(tVal)
        .distanceTo(previousMovement.getC3())

      if (distance < shaveDistance) {
        right = tVal
      } else {
        left = tVal
      }

      if (Math.abs(distance - shaveDistance) < 1) {
        break
      }

      if (iters > 100) {
        console.log(
          `exceeded 100 iters to generate csbezier to line transition`,
          tVal,
          shaveDistance,
          bezier.getLength(),
        )
        break
      }
    }

    // Split the previous bezier with control points A, B, C, D
    const { A, B, C, D, E, F, G } = subdivideBezierAndClampSpeed(
      previousMovement.getC0(),
      previousMovement.getC1(),
      previousMovement.getC2(),
      previousMovement.getC3(),
      previousMovement.getDesiredEntryVelocity(),
      previousMovement.getExpectedExitVelocity(),
      nextMovement.getDesiredEntryVelocity(),
      maxSpeed,
      material,
      markAsTransition,
      0.5,
    )

    const newPreviousMovementCSB = new ConstantSpeedBezier(
      A,
      B,
      C,
      D,
      material,
      previousMovement.objectID,
    )
    newPreviousMovementCSB.isTransition = markAsTransition
    newPreviousMovementCSB.setMaxSpeed(maxSpeed)

    const nextMovementCSB = new ConstantSpeedBezier(
      D,
      E,
      nextMovement.getStart(),
      nextMovement.getFrom(),
      material,
      nextMovement.objectID,
    )
    nextMovementCSB.isTransition = markAsTransition
    nextMovementCSB.setMaxSpeed(maxSpeed)

    return {
      newPreviousMovementCSB,
      nextMovementCSB,
    }
  } else {
    // Just return the next movement, the old movement will be removed

    const nextMovementCSB = new ConstantSpeedBezier(
      previousMovement.getC0(),
      previousMovement.getC1(),
      nextMovement.getFrom(),
      nextMovement.getStart(),
      material,
      nextMovement.objectID,
    )
    nextMovementCSB.isTransition = markAsTransition
    nextMovementCSB.setMaxSpeed(maxSpeed)

    return {
      newPreviousMovementCSB: null,
      nextMovementCSB,
    }
  }
}

function pointAlongLine(start: Vector3, end: Vector3, u: number) {
  const length = end.distanceTo(start)
  const direction = end.clone().sub(start).normalize()

  return direction.multiplyScalar(length * u).add(start)
}

// Use De Casteljau's algorithm
// https://math.stackexchange.com/questions/877725/retrieve-the-initial-cubic-b%C3%A9zier-curve-subdivided-in-two-b%C3%A9zier-curves
function subdivideBezier(
  c0: Vector3,
  c1: Vector3,
  c2: Vector3,
  c3: Vector3,
  transitionT: number,
): {
  A: Vector3
  B: Vector3
  C: Vector3
  D: Vector3
  E: Vector3
  F: Vector3
  G: Vector3
} {
  const u = transitionT
  // const v = 1 - u

  const A = c0
  const P = c1
  const Q = c2
  const G = c3

  const X = pointAlongLine(P, Q, u)

  const B = pointAlongLine(A, P, u)

  const C = pointAlongLine(B, X, u)
  const F = pointAlongLine(Q, G, u)
  const E = pointAlongLine(X, F, u)
  const D = pointAlongLine(C, E, u)

  return {
    A,
    B,
    C,
    D,
    E,
    F,
    G,
  }
}

function checkTransitionSpeed(transition: Bezier, name: string) {
  const { maxSpeed, tOfHighestSpeed } =
    findHighestApproximateSpeedAndT(transition)

  const probableMaxSpeed = transition.getEntrySpeedExact()
    ? transition.getDesiredEntryVelocity().length()
    : transition.getExpectedExitVelocity().length()

  // if it's more than 10 over, throw up a warning
  if (!isFinite(transition.maxSpeed) || probableMaxSpeed - maxSpeed > 10) {
    console.log(`desiredSpeed strange for ${name}
    entryVelocity:  [${transition
      .getDesiredEntryVelocity()
      .toArray()
      .join(', ')}] transition.getDesiredEntryVelocity().length() ${transition
      .getDesiredEntryVelocity()
      .length()}
    exitVelocity:  [${transition
      .getExpectedExitVelocity()
      .toArray()
      .join(', ')}] transition.getExpectedExitVelocity().length() ${transition
      .getExpectedExitVelocity()
      .length()}
    
    entrySpeedExact: ${transition.getEntrySpeedExact() ? 'entry' : 'exit'}

    transition.maxSpeed: ${transition.maxSpeed}
    predictedSpeedAt 0: ${predictSpeedAtT(transition, 0)}
    predictedSpeedAt 0.1: ${predictSpeedAtT(transition, 0.1)}
    predictedSpeedAt 0.3: ${predictSpeedAtT(transition, 0.3)}
    predictedSpeedAt 0.5: ${predictSpeedAtT(transition, 0.5)}
    predictedSpeedAt 0.7: ${predictSpeedAtT(transition, 0.7)}
    predictedSpeedAt 0.9: ${predictSpeedAtT(transition, 0.9)}
    predictedSpeedAt 1: ${predictSpeedAtT(transition, 1)}
    max speed sampled is at ${maxSpeed}, t ${tOfHighestSpeed}
    `)
  }
}

function subdivideBezierAndClampSpeed(
  c0: Vector3,
  c1: Vector3,
  c2: Vector3,
  c3: Vector3,

  entryVelocity: Vector3,
  midpointVelocity: Vector3,
  exitVelocity: Vector3,
  maxSpeed: number,
  material: Material,
  markAsTransition: boolean,
  tValue: number,
) {
  // Subdivide the bezier into two pieces
  const { A, B, C, D, E, F, G } = subdivideBezier(c0, c1, c2, c3, tValue)

  let cVal = 0 // can be tested from 0-1 to slow down if required
  let optimisedC = pointAlongLine(C, D, cVal)

  let eVal = 0 // can be tested from 0-1 to slow down if required
  let optimisedE = pointAlongLine(E, D, eVal)

  let entryTransition = new Bezier(
    A,
    B,
    optimisedC,
    D,
    entryVelocity,
    midpointVelocity,
    material,
    TRANSITION_OBJECT_ID,
    true,
  )
  entryTransition.isTransition = markAsTransition
  entryTransition.setMaxSpeed(maxSpeed)

  // If the end of the entry is too fast, mutate the control point to try and slow it down
  const endOfEntryTransitionSpeed = predictSpeedAtT(entryTransition, 1)

  if (endOfEntryTransitionSpeed > maxSpeed) {
    // We'll optimise this transition to slow down the midpoint

    let left = 0
    let right = 1

    // console.log(
    //   `optimising entry since ${endOfEntryTransitionSpeed} > ${maxSpeed}`,
    // )

    let slowestCVal = 0
    let slowestSpeed = Infinity

    let iters = 0
    optimiseEntry: while (true) {
      iters++

      cVal = (left + right) / 2
      optimisedC = pointAlongLine(C, D, cVal)

      entryTransition = new Bezier(
        A,
        B,
        optimisedC,
        D,
        entryVelocity,
        midpointVelocity,
        material,
        TRANSITION_OBJECT_ID,
        true,
      )
      entryTransition.isTransition = markAsTransition
      entryTransition.setMaxSpeed(maxSpeed)

      const transitionSpeed = predictSpeedAtT(entryTransition, 1)

      // console.log(
      //   `trying cVal ${cVal}, transitionSpeed: ${transitionSpeed} left: ${left} right: ${right}`,
      // )

      if (transitionSpeed < maxSpeed) {
        right = cVal
      } else {
        left = cVal
      }

      // And break when we're just under the speed limit
      if (Math.abs(maxSpeed - transitionSpeed) < 2) {
        // console.log(
        //   `solution found after ${iters} iters, cVal: ${cVal}, speed: ${transitionSpeed} near max: ${maxSpeed}`,
        // )
        break optimiseEntry
      }

      if (transitionSpeed < slowestSpeed) {
        slowestSpeed = transitionSpeed
        slowestCVal = cVal
      }

      if (iters > 25) {
        // Final iteration to use the lowest speed found
        optimisedC = pointAlongLine(C, D, slowestCVal)

        entryTransition = new Bezier(
          A,
          B,
          optimisedC,
          D,
          entryVelocity,
          midpointVelocity,
          material,
          TRANSITION_OBJECT_ID,
          true,
        )
        entryTransition.isTransition = markAsTransition
        entryTransition.setMaxSpeed(maxSpeed)

        console.log(
          `too many iterations entry ${iters}, slowestCVal: ${slowestCVal}, speed: ${slowestSpeed} over max: ${maxSpeed}`,
        )

        break optimiseEntry
      }
    }
  }

  let exitTransition = new Bezier(
    D,
    optimisedE,
    F,
    G,
    midpointVelocity,
    exitVelocity,
    material,
    TRANSITION_OBJECT_ID,
    false,
  )
  exitTransition.isTransition = markAsTransition
  exitTransition.setMaxSpeed(maxSpeed)

  // If the start of the exit is too fast, mutate the control point to try and slow it down
  const endOfExitTransitionSpeed = predictSpeedAtT(exitTransition, 0)

  if (endOfExitTransitionSpeed > maxSpeed) {
    // We'll optimise this transition to slow down the midpoint

    let left = 0
    let right = 1

    // console.log(
    //   `optimising exit since ${endOfExitTransitionSpeed} > ${maxSpeed}`,
    // )

    let slowestEVal = 0
    let slowestSpeed = Infinity

    let iters = 0
    optimiseExit: while (true) {
      iters++

      cVal = (left + right) / 2
      optimisedE = pointAlongLine(E, D, cVal)

      exitTransition = new Bezier(
        D,
        optimisedE,
        F,
        G,
        midpointVelocity,
        exitVelocity,
        material,
        TRANSITION_OBJECT_ID,
        false,
      )
      exitTransition.isTransition = markAsTransition
      exitTransition.setMaxSpeed(maxSpeed)

      const transitionSpeed = predictSpeedAtT(exitTransition, 0)

      if (transitionSpeed < maxSpeed) {
        right = cVal
      } else {
        left = cVal
      }

      // And break when we're just under the speed limit
      if (Math.abs(maxSpeed - transitionSpeed) < 2) {
        // console.log(
        //   `solution found after ${iters} iters, cVal: ${cVal}, speed: ${transitionSpeed} near max: ${maxSpeed}`,
        // )
        break optimiseExit
      }

      if (transitionSpeed < slowestSpeed) {
        slowestSpeed = transitionSpeed
        slowestEVal = cVal
      }

      if (iters > 25) {
        // Final iteration to use the lowest speed found
        optimisedE = pointAlongLine(E, D, slowestEVal)

        exitTransition = new Bezier(
          D,
          optimisedE,
          F,
          G,
          entryVelocity,
          midpointVelocity,
          material,
          TRANSITION_OBJECT_ID,
          true,
        )
        exitTransition.isTransition = markAsTransition
        exitTransition.setMaxSpeed(maxSpeed)

        console.log(
          `too many iterations exit ${iters}, slowestEVal: ${slowestEVal}, speed: ${slowestSpeed} over max: ${maxSpeed}`,
        )

        break optimiseExit
      }
    }
  }

  return {
    entryTransition,
    exitTransition,
    A: A,
    B: B,
    C: optimisedC,
    D: D,
    E: optimisedE,
    F: F,
    G: G,
  }
}
