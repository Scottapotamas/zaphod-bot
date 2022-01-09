import {
  declareDense,
  DenseMovements,
  InterLineTransition,
  isLine,
  isPoint,
  Line,
  Movement,
  Point,
  PointTransition,
  Transit,
  Transition,
  TRANSITION_OBJECT_ID,
} from './movements'

import { Settings } from './settings'
import { MathUtils, Vector3 } from 'three'
import { defaultTransitionMaterial } from './material'
import { MixMaterial } from './materials/MixMaterial'

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
      const transition = new PointTransition(
        movementPrevPrev,
        movementPrev,
        movementCurrent,
        movementNext,
        defaultTransitionMaterial,
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

    // If the last movement and this movement are both lines, and their end and start points match up
    // And their velocity angles aren't too dissimilar, reduce the length of the lines and do a transition inline
    if (
      settings.optimisation.smoothInterlineTransitions &&
      isLine(previousMovement) &&
      isLine(movement) &&
      previousMovement.getEnd().distanceToSquared(movement.getStart()) < 1 &&
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
        new MixMaterial(previousMovement, movement),
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

    // Build our transition movement from the old movement to the new
    const transition = new Transition(
      previousMovement,
      movement,
      defaultTransitionMaterial,
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

export function getTotalCost(denseMoves: DenseMovements) {
  // For now, use the total duration as the cost function
  let cost = 0

  for (let index = 0; index < denseMoves.length; index++) {
    const movement = denseMoves[index]
    const newCost = movement.getCost()
    cost += movement.getCost()
  }

  return cost
}

/**
 * Take a sparse set of movements, join them, flatten them, calculate the total duration
 */
export function sparseToCost(
  sparseBag: Movement[],
  settings: Settings,
): number {
  const dense = sparseToDense(sparseBag, settings)

  return getTotalCost(dense)
}

function swap(array: any[], a: number, b: number) {
  const temp = array[a]
  array[a] = array[b]
  array[b] = temp
}

/**
 * Swaps two line segments if the cost is better, returns if the swap was better
 */
function swapIsBetter(
  movements: Movement[],
  costRef: { cost: number },
  settings: Settings,
  i: number,
  j: number,
): boolean {
  // Do the swap
  swap(movements, i, j)

  // Calculate the new cost
  const tourCostWithSwap = sparseToCost(movements, settings)

  // console.log(
  //   `trying to swap ${i} and ${j}, cost flipped is ${tourCostWithSwap}, without is ${costRef.cost}.`
  // );

  // if it's worse or equal, swap it back
  if (tourCostWithSwap >= costRef.cost) {
    swap(movements, i, j)
    return false
  }

  // Otherwise it's better, update the cost
  costRef.cost = tourCostWithSwap
  return true
}

/**
 * Flips a singular movement if the cost is better, returns if it was better.
 */
function flipIsBetter(
  movements: Movement[],
  costRef: { cost: number },
  settings: Settings,
  i: number,
): boolean {
  // If the movement is a Point, we can't flip, bail early
  if (isPoint(movements[i])) {
    return false
  }

  // Do the flip
  movements[i].flip()

  // Calculate the new cost
  const tourCostWithFlip = sparseToCost(movements, settings)

  // console.log(
  //   `trying to flip ${i}, cost flipped is ${tourCostWithFlip}, without is ${costRef.cost}.`
  // );

  // if it's worse or equal, swap it back
  if (tourCostWithFlip >= costRef.cost) {
    movements[i].flip()
    return false
  }

  // Otherwise it's better, update the cost
  costRef.cost = tourCostWithFlip
  return true
}

export interface OrderingCache {
  [id: string]: number
}

export interface Progress {
  duration: number
  text: string
  orderingCache: OrderingCache
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

/**
 * Reorders and flips the members of a sparse bag of movements, optimising for the fastest tour.
 *
 * Async so we can interrupt the event queue to check for pausing, otherwise it'll just run.
 */
export async function optimise(
  sparseBag: Movement[],
  settings: Settings,
  updateProgress: (progress: Progress) => Promise<Continue>,
  orderingCache: OrderingCache | null = null,
) {
  const sparseLength = sparseBag.length

  let ordering: Movement[] = sparseBag

  const costRef = { cost: sparseToCost(sparseBag, settings) }
  const startingCost = costRef.cost

  let method = 'blender order'

  // If an ordering cache is provided, test it
  if (orderingCache) {
    // Copy the array, might not be necessary since this is on the other end of an IPC bridge and it's just been deserialised.
    // TODO: Bench removing this
    const cachedOrdering = sparseBag.slice()

    // Sort the movements according to the movement cache
    cachedOrdering.sort((a, b) => {
      const aOrder = orderingCache[a.interFrameID] ?? 0
      const bOrder = orderingCache[b.interFrameID] ?? 0

      // Sort in ascending order
      return aOrder - bOrder
    })

    const interFrameCacheCost = sparseToCost(cachedOrdering, settings)

    // console.log(
    //   `randomCost ${
    //     costRef.cost
    //   } -> interFrameCacheCost ${interFrameCacheCost} (${
    //     interFrameCacheCost > costRef.cost ? "ditching cache" : "using cache"
    //   })`
    // );

    if (interFrameCacheCost < costRef.cost) {
      // If the inter frame cache is better than the naive ordering, use it
      ordering = cachedOrdering
      costRef.cost = interFrameCacheCost
      method = 'cache'
    }
  }

  // Try a nearest neighbour search, use it if it's better (even if we have a cache)
  {
    const toOrder: Movement[] = sparseBag.slice() // Copy the array

    // Pick a random movement to start at, remove it from the array
    let previousMovement = toOrder.splice(
      Math.floor(Math.random() * toOrder.length),
      1, // grab one
    )[0] // splice returns an array of the deleted items, index 0 is our starting point
    const nnOrdering = [previousMovement]

    // Find the closest next movement, potentially flipping it
    while (toOrder.length > 0) {
      let closest: Movement = previousMovement
      let closestDistance = Infinity
      let closestIndex = 0

      for (let index = 0; index < toOrder.length; index++) {
        const movement = toOrder[index]

        // Check if the end of the next movement is closer
        let d = movement.getStart().distanceToSquared(previousMovement.getEnd())

        if (d < closestDistance) {
          closestDistance = d
          closest = movement
          closestIndex = index
        }

        // Try flipping it
        movement.flip()

        d = movement.getStart().distanceToSquared(previousMovement.getEnd())

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

    const nnCost = sparseToCost(nnOrdering, settings)

    // console.log(
    //   `randomCost ${costRef.cost} -> nnCost ${nnCost} (${
    //     nnCost > costRef.cost ? "ditching NN" : "using NN"
    //   })`
    // );

    if (nnCost < costRef.cost) {
      // If NN is an improvement, use it as the starting point
      costRef.cost = nnCost
      ordering = nnOrdering
      method = 'nearest neighbour'
    }
  }

  // Setup our ordering cache
  const nextOrderingCache: OrderingCache = {}

  const populateOrderingCache = () => {
    // Store the final order for passing to the next frame
    for (let index = 0; index < ordering.length; index++) {
      const movement = ordering[index]
      nextOrderingCache[movement.interFrameID] = index
    }

    return nextOrderingCache
  }

  // Establish base cost to compare against

  let improved = true // Start iterating
  let iteration = 0
  let stoppedEarly = false
  let startedTSP = Date.now()

  while (improved) {
    improved = false
    iteration++

    const currentDense = sparseToDense(ordering, settings)
    const curentDuration = getTotalDuration(currentDense)

    const shouldContinue = await updateProgress({
      duration: curentDuration,
      text: `Optimised to ${
        Math.round(curentDuration * 100) / 100
      }ms via ${method}`,
      orderingCache: populateOrderingCache(),
      completed: false,
      minimaFound: false,
      timeSpent: Date.now() - startedTSP,
      startingCost,
      currentCost: costRef.cost,
    })

    if (!shouldContinue) {
      stoppedEarly = true
      break
    }

    method = '2-opt'

    iteration: for (let i = 0; i < sparseLength - 1; i++) {
      for (let j = i + 1; j < sparseLength; j++) {
        // Try flipping each member first
        if (flipIsBetter(ordering, costRef, settings, i)) {
          improved = true
          continue iteration
        }
        if (flipIsBetter(ordering, costRef, settings, j)) {
          improved = true
          continue iteration
        }

        // Try swapping the two movements
        if (swapIsBetter(ordering, costRef, settings, i, j)) {
          improved = true
          continue iteration
        }

        // Try flipping each member again
        if (flipIsBetter(ordering, costRef, settings, i)) {
          improved = true
          continue iteration
        }
        if (flipIsBetter(ordering, costRef, settings, j)) {
          improved = true
          continue iteration
        }
      }
    }
  }

  const currentDense = sparseToDense(ordering, settings)
  const curentDuration = getTotalDuration(currentDense)

  // Final status update
  await updateProgress({
    duration: getTotalDuration(currentDense),
    text: `Optimised to ${Math.round(curentDuration * 100) / 100}ms`,
    orderingCache: populateOrderingCache(),
    completed: true,
    minimaFound: !stoppedEarly,
    timeSpent: Date.now() - startedTSP,
    startingCost,
    currentCost: costRef.cost,
  })

  return {
    orderedMovements: ordering,
    cost: costRef.cost,
    iterations: iteration,
    orderingCache: populateOrderingCache(),
    stoppedEarly,
  }
}
