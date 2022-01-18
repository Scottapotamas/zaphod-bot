import {
  declareDense,
  DenseMovements,
  InterLineTransition,
  isLine,
  isPoint,
  isTransit,
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

function swap(array: any[], a: number, b: number) {
  const temp = array[a]
  array[a] = array[b]
  array[b] = temp
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

function optimiseByCache(sparseBag: Movement[], orderingCache: OrderingCache) {
  const movements = sparseBag.slice() // Copy the ordering

  // Sort the movements according to the movement cache
  movements.sort((a, b) => {
    const aOrder = orderingCache[a.interFrameID] ?? 0
    const bOrder = orderingCache[b.interFrameID] ?? 0

    // Sort in ascending order
    return aOrder - bOrder
  })

  return movements
}

function optimiseBySearch(sparseBag: Movement[]) {
  if (sparseBag.length < 2) {
    return sparseBag
  }

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

  return nnOrdering
}

function optimise2Opt(sparseBag: Movement[], timeLimit = 0) {
  const ordering: Movement[] = sparseBag.slice()

  let improved = true // Start iterating
  let iteration = 0
  let cost = sparseToCost(ordering)
  const start = Date.now()

  while (improved) {
    improved = false
    iteration++

    if (timeLimit > 0 && Date.now() - start > timeLimit) {
      return { ordering, iteration, completed: false }
    }

    // Start and end points are fixed with this algorithm
    iteration: for (let i = 1; i < ordering.length - 2; i++) {
      for (let j = i + 1; j < ordering.length - 1; j++) {
        const A = ordering[i - 1]
        const B = ordering[i]
        const C = ordering[i + 1]
        const D = ordering[j - 1]
        const E = ordering[j]
        const F = ordering[j + 1]

        // Calculate the distances of the different segments, flipped or swapped
        const dABnC = A.getEnd().distanceTo(B.getStart()) + B.getEnd().distanceTo(C.getStart()) // prettier-ignore
        const dDEnF = D.getEnd().distanceTo(E.getStart()) + E.getEnd().distanceTo(F.getStart()) // prettier-ignore

        const dAEnC = A.getEnd().distanceTo(E.getStart()) + E.getEnd().distanceTo(C.getStart()) // prettier-ignore
        const dDBnF = D.getEnd().distanceTo(B.getStart()) + B.getEnd().distanceTo(F.getStart()) // prettier-ignore

        const dABfC = A.getEnd().distanceTo(B.getEnd()) + B.getStart().distanceTo(C.getStart()) // prettier-ignore
        const dDEfF = D.getEnd().distanceTo(E.getEnd()) + E.getStart().distanceTo(F.getStart()) // prettier-ignore

        const dAEfC = A.getEnd().distanceTo(E.getEnd()) + E.getStart().distanceTo(C.getStart()) // prettier-ignore
        const dDBfF = D.getEnd().distanceTo(B.getEnd()) + B.getStart().distanceTo(F.getStart()) // prettier-ignore

        // Calculate cost deltas
        const current = dABnC + dDEnF
        const flipI = dABfC + dDEnF
        const flipJ = dABnC + dDEfF
        const flipIJ = dABfC + dDEfF
        const swappedIJ = dAEnC + dDBnF
        const flipISwapIJ = dAEnC + dDBfF
        const flipJSwapIJ = dAEfC + dDBnF
        const flipIJSwapIJ = dAEfC + dDBfF

        // Find the winner
        const smallest = Math.min(
          current,
          flipI,
          flipJ,
          flipIJ,
          swappedIJ,
          flipISwapIJ,
          flipJSwapIJ,
          flipIJSwapIJ,
        )

        // Do the operations of the winner
        if (smallest === current) {
          // No improvement
          // continue // try the next one
        } else if (smallest === flipI) {
          ordering[i].flip()
          cost = cost - current + flipI
          improved = true
        } else if (smallest === flipJ) {
          ordering[j].flip()
          cost = cost - current + flipJ
          improved = true
        } else if (smallest === flipIJ) {
          ordering[i].flip()
          ordering[j].flip()
          cost = cost - current + flipIJ
          improved = true
        } else if (smallest === swappedIJ) {
          swap(ordering, i, j)
          cost = cost - current + swappedIJ
          improved = true
        } else if (smallest === flipISwapIJ) {
          ordering[i].flip()
          swap(ordering, i, j)
          cost = cost - current + flipISwapIJ
          improved = true
        } else if (smallest === flipJSwapIJ) {
          ordering[j].flip()
          swap(ordering, i, j)
          cost = cost - current + flipJSwapIJ
          improved = true
        } else if (smallest === flipIJSwapIJ) {
          ordering[i].flip()
          ordering[j].flip()
          swap(ordering, i, j)
          cost = cost - current + flipIJSwapIJ
          improved = true
        }

        // continue iteration
      }
    }
  }

  return { ordering, iteration, completed: true }
}

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
  orderingCache: OrderingCache | null = null,
) {
  const startedOptimisation = Date.now()

  // Setup our ordering cache
  const nextOrderingCache: OrderingCache = {}

  const populateOrderingCache = () => {
    // Store the final order for passing to the next frame
    for (let index = 0; index < sparseBag.length; index++) {
      const movement = sparseBag[index]
      nextOrderingCache[movement.interFrameID] = index
    }

    return nextOrderingCache
  }

  const startingCost = sparseToCost(sparseBag)

  // Partial updates just run a beam search

  if (partialUpdate) {
    const beamSearched = optimiseBySearch(sparseBag)

    const currentDense = sparseToDense(beamSearched, settings)
    const curentDuration = getTotalDuration(currentDense)

    // Final status update
    await updateProgress({
      duration: getTotalDuration(currentDense),
      text: `Optimised to ${Math.round(curentDuration * 100) / 100}ms`,
      orderingCache: populateOrderingCache(),
      completed: true,
      minimaFound: false,
      timeSpent: Date.now() - startedOptimisation,
      startingCost,
      currentCost: sparseToCost(sparseBag),
    })

    return
  }

  let ordering = sparseBag

  let iterations = 0

  // Otherwise do a 2opt
  while (true) {
    // Do one second of optimisation at a time, then check in to see if we should cancel
    const nextPass = optimise2Opt(ordering, 1000)

    iterations += nextPass.iteration

    console.log(
      `did ${nextPass.iteration} that second, total of ${iterations}, ${
        iterations / ((Date.now() - startedOptimisation) / 1000)
      } iter/s`,
    )

    const currentDense = sparseToDense(nextPass.ordering, settings)
    const curentDuration = getTotalDuration(currentDense)

    const shouldContinue = await updateProgress({
      duration: getTotalDuration(currentDense),
      text: `Optimised to ${Math.round(curentDuration * 100) / 100}ms`,
      orderingCache: populateOrderingCache(),
      completed: nextPass.completed,
      minimaFound: nextPass.completed,
      timeSpent: Date.now() - startedOptimisation,
      startingCost,
      currentCost: sparseToCost(sparseBag),
    })

    ordering = nextPass.ordering

    // If we've reached a minima, or should stop, exit
    if (nextPass.completed || !shouldContinue) {
      return
    }
  }
}
