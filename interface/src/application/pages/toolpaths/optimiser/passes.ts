import {
  declareDense,
  DenseMovements,
  isMovementGroup,
  isPoint,
  Movement,
  Point,
  Transition,
} from './movements'
import { LightMove, MovementMove, MovementMoveReference } from './hardware'

import { getTransitionMaterial, Settings } from './settings'

/**
 * Add transition movements between each move if the start and the end are at different places
 */
export function sparseToDense(
  sparseBag: Movement[],
  settings: Settings,
): DenseMovements {
  const denseMovements: DenseMovements = declareDense([])

  const transitionMaterial = getTransitionMaterial(settings)

  // Start
  let previousMovement: Movement = new Point(
    settings.optimisation.startingPoint,
    settings.optimisation.waitAtStartDuration,
    transitionMaterial,
  )

  // Middle
  for (let index = 0; index < sparseBag.length; index++) {
    const movement = sparseBag[index]

    // Set the max speed of the movement so velocities are scaled
    movement.setMaxSpeed(settings.optimisation.maxSpeed)

    // TODO: If we're in the middle of a run of Points, build a Catmull spline that goes between them

    // Build our transition movement from the old movement to the new
    const transition = new Transition(
      previousMovement,
      movement,
      transitionMaterial,
    )

    // Add the transition to the dense bag
    denseMovements.push(transition)

    // Add the movement to the dense bag
    denseMovements.push(movement)

    // Update the last movement
    previousMovement = movement
    continue
  }

  // End
  let lastMovement: Movement = new Point(
    settings.optimisation.endingPoint,
    1, // wait for 1ms at the end
    transitionMaterial,
  )

  // Transition to the end
  const transitionToEnd = new Transition(
    previousMovement,
    lastMovement,
    transitionMaterial,
  )

  // Add the transition to the dense bag
  denseMovements.push(transitionToEnd)

  // Add the movement to the dense bag
  denseMovements.push(lastMovement)

  return denseMovements
}

/**
 * Flatten any `OrderedMovements` groups into simple movements.
 */
export function flattenDense(denseBag: DenseMovements): DenseMovements {
  let denseFlatList: DenseMovements = declareDense([])

  for (let index = 0; index < denseBag.length; index++) {
    const movement = denseBag[index]

    denseFlatList.push(...movement.flatten())
  }

  return denseBag
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
  const flattened = flattenDense(dense)

  return getTotalCost(flattened)
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
  // method: string;
  toolpath: Toolpath
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

  let ordering: Movement[] = []

  const costRef = { cost: sparseToCost(sparseBag, settings) }
  const startingCost = costRef.cost

  let method = 'blender order'

  // If an orderingCache is provided, start with that, otherwise do a nearest neighbour run first.
  if (orderingCache) {
    // Copy the array, might not be necessary since this is on the other end of an IPC bridge and it's just been deserialised.
    // TODO: Bench removing this
    ordering = sparseBag.slice()

    // Sort the movements according to the movement cache
    ordering.sort((a, b) => {
      const aOrder = orderingCache[a.id] ?? 0
      const bOrder = orderingCache[b.id] ?? 0

      // Sort in ascending order
      return aOrder - bOrder
    })

    const interFrameCacheCost = sparseToCost(ordering, settings)

    // console.log(
    //   `randomCost ${
    //     costRef.cost
    //   } -> interFrameCacheCost ${interFrameCacheCost} (${
    //     interFrameCacheCost > costRef.cost ? "ditching cache" : "using cache"
    //   })`
    // );

    // Make sure this is an improvement
    if (interFrameCacheCost >= costRef.cost) {
      // not an improvement, reset
      ordering = sparseBag.slice()
    } else {
      costRef.cost = interFrameCacheCost
      method = 'cache'
    }
  } else {
    // First run, do a nearest neighbour search, the first movement is the starting point, so start there.

    // Run a nearest neighbour search and see how optimal it is
    const toOrder: Movement[] = sparseBag.slice() // Copy the array

    // Pick a random movement to start at, remove it from the array
    let previousMovement = toOrder.splice(
      Math.floor(Math.random() * toOrder.length),
      1, // grab one
    )[0] // splice returns an array of the deleted items, index 0 is our starting point
    ordering = [previousMovement]

    // Pick the closest
    while (toOrder.length > 0) {
      let closest: Movement = previousMovement
      let closestDistance = Infinity
      let closestIndex = 0

      for (let index = 0; index < toOrder.length; index++) {
        const movement = toOrder[index]

        const d = movement
          .getEnd()
          .distanceToSquared(previousMovement.getStart())

        if (d < closestDistance) {
          closestDistance = d
          closest = movement
          closestIndex = index
        }
      }

      // We've found the next closest, pop it off
      toOrder.splice(closestIndex, 1)

      // And add it to the nnOrdered array
      ordering.push(closest)

      // Update the previousMovement
      previousMovement = closest
    }

    const nnCost = sparseToCost(ordering, settings)

    // console.log(
    //   `randomCost ${costRef.cost} -> nnCost ${nnCost} (${
    //     nnCost > costRef.cost ? "ditching NN" : "using NN"
    //   })`
    // );

    // Make sure this is an improvement
    if (nnCost >= costRef.cost) {
      // not an improvement, reset
      ordering = sparseBag.slice()
    } else {
      costRef.cost = nnCost
      method = 'nearest neighbour'
    }
  }

  // Setup our ordering cache
  const nextOrderingCache: OrderingCache = {}

  const populateOrderingCache = () => {
    // Store the final order for passing to the next frame
    for (let index = 0; index < ordering.length; index++) {
      const movement = ordering[index]
      nextOrderingCache[movement.id] = index
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

    const currentDense = flattenDense(sparseToDense(ordering, settings))
    const curentDuration = getTotalDuration(currentDense)

    const shouldContinue = await updateProgress({
      duration: curentDuration,
      text: `Optimised to ${Math.round(curentDuration * 100) /
        100}ms via ${method}`,
      toolpath: toolpath(currentDense),
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

  const currentDense = flattenDense(sparseToDense(ordering, settings))
  const curentDuration = getTotalDuration(currentDense)

  // Final status update
  await updateProgress({
    duration: getTotalDuration(currentDense),
    text: `Optimised to ${Math.round(curentDuration * 100) / 100}ms`,
    toolpath: toolpath(flattenDense(sparseToDense(ordering, settings))),
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

export interface Toolpath {
  movementMoves: MovementMove[]
  lightMoves: LightMove[]
}

export function toolpath(denseMovements: DenseMovements): Toolpath {
  const movementMoves: MovementMove[] = []
  const lightMoves: LightMove[] = []

  let id = 0
  // each movement should have a generateToolpath method
  for (const movement of denseMovements) {
    // Increment the ID
    id++

    // Build the hardware moves
    movementMoves.push(...movement.generateToolpath(id))

    // Build the light moves
    lightMoves.push(...movement.generateLightpath(id))
  }

  return {
    movementMoves,
    lightMoves,
  }
}
