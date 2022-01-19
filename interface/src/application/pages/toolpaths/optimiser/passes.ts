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

export function swap(array: any[], a: number, b: number) {
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

export function optimiseBySearch(sparseBag: Movement[]) {
  if (sparseBag.length < 2) {
    return sparseBag
  }

  const toOrder: Movement[] = sparseBag.slice() // Copy the array

  // Pick a random movement to start at, remove it from the array
  let previousMovement = toOrder.splice(
    0, // start at 0
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

export function optimise2Opt(sparseBag: Movement[], timeLimit = 0) {
  let ordering: Movement[] = sparseBag.slice()

  let improved = true // Start iterating
  let iteration = 0
  let cost = sparseToCost(ordering)
  const start = Date.now()

  // Only support more than 4
  if (sparseBag.length < 4) {
    return { ordering, iteration, completed: true, time: Date.now() - start }
  }

  while (improved) {
    improved = false

    const n = false
    const f = true

    {
      // Test for all rotations, removing the longest link if possible
      let longestLinkIndex = 0
      let longestLinkDistance = 0

      for (let index = 0; index < ordering.length; index++) {
        const prev = ordering[index]
        const current = ordering[(index + 1) % ordering.length]
        const distance = prev.getEnd().distanceTo(current.getStart())

        if (distance > longestLinkDistance) {
          longestLinkIndex = index
          longestLinkDistance = distance
        }
      }

      if (longestLinkIndex !== ordering.length - 1) {
        const left = ordering.slice(0, longestLinkIndex + 1)
        const right = ordering.slice(longestLinkIndex + 1)

        // Reorder the arrays to remove the longest link
        ordering = right.concat(left)
        const newLink = right[right.length - 1]
          .getEnd()
          .distanceTo(left[0].getStart())
        cost = cost - longestLinkDistance + newLink

        // console.log(`rotated array, delta ${-longestLinkDistance + newLink}`)
      }
    }
    // The main iteration loop can't flip or swap the ends, so do that up here
    {
      // Test flipping the start
      const A = ordering[0]
      const B = ordering[1]

      const dAnB = A.getEnd().distanceTo(B.getStart())
      const dAfB = A.getStart().distanceTo(B.getStart())

      if (dAfB < dAnB) {
        A.flip()
        // console.log(`flipping start`)

        const delta = dAnB - dAfB

        cost = cost - delta
        improved = true
      }
    }
    {
      // Test flipping the end
      const E = ordering[ordering.length - 2]
      const F = ordering[ordering.length - 1]

      const dEFn = E.getEnd().distanceTo(F.getStart())
      const dEFf = E.getEnd().distanceTo(F.getEnd())

      if (dEFf < dEFn) {
        F.flip()
        // console.log(`flipping end`)

        const delta = dEFn - dEFf

        cost = cost - delta
        improved = true
      }
    }
    // {
    //   // Test a left rotation
    //   const A = ordering[0]
    //   const B = ordering[1]
    //   const C = ordering[ordering.length - 2]
    //   const D = ordering[ordering.length - 1]

    //   // A B ... C D
    //   // B ... C D A
    //   // destroys the AB link, adds the DA link

    //   const dAnBn = d(A, n, B, n)
    //   const dDnAn = d(D, n, A, n)

    //   if (dDnAn < dAnBn) {
    //     ordering.push(ordering.shift()!)
    //     // console.log(`rotating left`)

    //     const delta = dAnBn - dDnAn

    //     cost = cost - delta
    //     improved = true
    //   }
    // }
    // {
    //   // Test a right rotation
    //   const A = ordering[0]
    //   const B = ordering[1]
    //   const C = ordering[ordering.length - 2]
    //   const D = ordering[ordering.length - 1]

    //   // A B ... C D
    //   // D A B ... C
    //   // destroys the CD link, adds the DA link

    //   const dCnDn = d(C, n, D, n)
    //   const dDnAn = d(D, n, A, n)

    //   if (dDnAn < dCnDn) {
    //     ordering.unshift(ordering.pop()!)
    //     // console.log(`rotating right`)

    //     const delta = dCnDn - dDnAn

    //     cost = cost - delta
    //     improved = true
    //   }
    // }
    {
      // TODO: Test a flip
    }
    // Start and end points are fixed with this algorithm
    iteration: for (let b = 1; b < ordering.length - 2; b++) {
      for (let e = b + 1; e < ordering.length - 1; e++) {
        // If they're next to each other, it's a special case
        if (e === b + 1) {
          const A = ordering[b - 1]
          const B = ordering[b]
          const C = ordering[e]
          const D = ordering[e + 1]

          const current = d(A, n, B, n) + d(B, n, C, n) + d(C, n, D, n)
          const flipB = d(A, n, B, f) + d(B, f, C, n) + d(C, n, D, n)
          const flipC = d(A, n, B, n) + d(B, n, C, f) + d(C, f, D, n)
          const flipBC = d(A, n, B, f) + d(B, f, C, f) + d(C, f, D, n)
          const swapBC = d(A, n, C, n) + d(C, n, B, n) + d(B, n, D, n)
          const flipBSwapBC = d(A, n, C, n) + d(C, n, B, f) + d(B, f, D, n)
          const flipCSwapBC = d(A, n, C, f) + d(C, f, B, n) + d(B, n, D, n)
          const flipBCSwapBC = d(A, n, C, f) + d(C, f, B, f) + d(B, f, D, n)

          // Find the winner
          const smallest = Math.min(
            current,
            flipB,
            flipC,
            flipBC,
            swapBC,
            flipBSwapBC,
            flipCSwapBC,
            flipBCSwapBC,
          )

          // update the cost
          const delta = current - smallest
          cost = cost - delta

          // Do the operations of the winner

          if (smallest === current) {
            // No improvement
          } else if (smallest === flipB) {
            ordering[b].flip()
            improved = true
          } else if (smallest === flipC) {
            ordering[e].flip()
            improved = true
          } else if (smallest === flipBC) {
            ordering[b].flip()
            ordering[e].flip()
            improved = true
          } else if (smallest === swapBC) {
            swap(ordering, b, e)
            improved = true
          } else if (smallest === flipBSwapBC) {
            ordering[b].flip()
            swap(ordering, b, e)
            improved = true
          } else if (smallest === flipCSwapBC) {
            ordering[e].flip()
            swap(ordering, b, e)
            improved = true
          } else if (smallest === flipBCSwapBC) {
            ordering[b].flip()
            ordering[e].flip()
            swap(ordering, b, e)
            improved = true
          }

          continue
        }

        // The segments are not overlapping
        const A = ordering[b - 1]
        const B = ordering[b]
        const C = ordering[b + 1]
        const D = ordering[e - 1]
        const E = ordering[e]
        const F = ordering[e + 1]

        // Calculate the distances of the different segments, flipped or swapped
        const current = d(A, n, B, n) + d(B, n, C, n) + d(D, n, E, n) + d(E, n, F, n) // prettier-ignore
        const flipB = d(A, n, B, f) + d(B, f, C, n) + d(D, n, E, n) + d(E, n, F, n) // prettier-ignore
        const flipE = d(A, n, B, n) + d(B, n, C, n) + d(D, n, E, f) + d(E, f, F, n) // prettier-ignore
        const flipBE = d(A, n, B, f) + d(B, f, C, n) + d(D, n, E, f) + d(E, f, F, n) // prettier-ignore
        const swapBE = d(A, n, E, n) + d(E, n, C, n) + d(D, n, B, n) + d(B, n, F, n) // prettier-ignore
        const flipBSwapBE = d(A, n, E, n) + d(E, n, C, n) + d(D, n, B, f) + d(B, f, F, n) // prettier-ignore
        const flipESwapBE = d(A, n, E, f) + d(E, f, C, n) + d(D, n, B, n) + d(B, n, F, n) // prettier-ignore
        const flipBESwapBE = d(A, n, E, f) + d(E, f, C, n) + d(D, n, B, f) + d(B, f, F, n) // prettier-ignore

        // Find the winner
        const smallest = Math.min(
          current,
          flipB,
          flipE,
          flipBE,
          swapBE,
          flipBSwapBE,
          flipESwapBE,
          flipBESwapBE,
        )

        // update the cost
        const delta = current - smallest
        cost = cost - delta

        // Do the operations of the winner
        if (smallest === current) {
          // No improvement
        } else if (smallest === flipB) {
          ordering[b].flip()
          improved = true
        } else if (smallest === flipE) {
          ordering[e].flip()
          improved = true
        } else if (smallest === flipBE) {
          ordering[b].flip()
          ordering[e].flip()
          improved = true
        } else if (smallest === swapBE) {
          swap(ordering, b, e)
          improved = true
        } else if (smallest === flipBSwapBE) {
          ordering[b].flip()
          swap(ordering, b, e)
          improved = true
        } else if (smallest === flipESwapBE) {
          ordering[e].flip()
          swap(ordering, b, e)
          improved = true
        } else if (smallest === flipBESwapBE) {
          ordering[b].flip()
          ordering[e].flip()
          swap(ordering, b, e)
          improved = true
        }

        if (cost < 0) {
          throw new Error(`TSP score went < 0`)
        }
      }
    }
  }

  return { ordering, iteration, completed: true, time: Date.now() - start }
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

  const populateOrderingCache = (movements: Movement[]) => {
    // Store the final order for passing to the next frame
    for (let index = 0; index < movements.length; index++) {
      const movement = movements[index]
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
      orderingCache: populateOrderingCache(beamSearched),
      completed: true,
      minimaFound: false,
      timeSpent: Date.now() - startedOptimisation,
      startingCost,
      currentCost: sparseToCost(sparseBag),
    })

    return
  }

  let ordering = optimiseBySearch(sparseBag)

  let iterations = 0

  // Otherwise do a 2opt
  while (true) {
    // Do one second of optimisation at a time, then check in to see if we should cancel
    const nextPass = optimise2Opt(ordering, 1000)

    iterations += nextPass.iteration

    const currentDense = sparseToDense(nextPass.ordering, settings)
    const currentDuration = getTotalDuration(currentDense)

    // Finish within the time it takes to _do_ the frame no matter what
    let done =
      nextPass.completed || Date.now() - startedOptimisation > currentDuration

    const shouldContinue = await updateProgress({
      duration: getTotalDuration(currentDense),
      text: `Optimised to ${Math.round(currentDuration * 100) / 100}ms`,
      orderingCache: populateOrderingCache(nextPass.ordering),
      completed: done,
      minimaFound: done,
      timeSpent: Date.now() - startedOptimisation,
      startingCost,
      currentCost: sparseToCost(sparseBag),
    })

    console.log(
      `iterations: ${iterations}, currentDuration: ${currentDuration}, time: ${
        (Date.now() - startedOptimisation) / 1000
      }s`,
    )

    ordering = nextPass.ordering

    // If we've reached a minima, or should stop, or we've taken longer than the length of the tour to optimise, exit
    if (done || !shouldContinue) {
      return
    }
  }
}
