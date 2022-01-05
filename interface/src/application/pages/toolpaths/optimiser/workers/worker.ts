import { Observable, Subject } from 'threads/observable'
import { expose } from 'threads/worker'

import { importJson, MovementJSON } from '../import'
import { Movement } from '../movements'
import {
  Continue,
  flattenDense,
  getTotalDuration,
  optimise,
  OrderingCache,
  Progress,
  sparseToDense,
} from '../passes'
import { Settings } from '../settings'

// This will be called across an IPC bridge, all arguments must be serialisable
export async function optimiseFrameBag(
  sparseBagToImport: MovementJSON[],
  settings: Settings,
  updateProgress: (progress: Progress) => Promise<Continue>,
  orderingCache?: OrderingCache,
) {
  const movements: Movement[] = []

  // Process the raw objects into movements
  for (const json of sparseBagToImport) {
    const imported = importJson(json)
    for (const movement of imported.toMovements(settings)) {
      movements.push(movement)
    }
  }

  // Run the optimiser
  const {
    orderedMovements,
    cost,
    orderingCache: nextOrderingCache,
    iterations,
  } = await optimise(movements, settings, updateProgress, orderingCache)

  const dense = sparseToDense(orderedMovements, settings)
  const flattened = flattenDense(dense)
  const duration = getTotalDuration(flattened)

  return {
    duration,
    orderingCache: nextOrderingCache,
  }
}

let progressUpdates = new Subject<Progress>()

let shouldContinue = true

// Export the type so we can strictly type the other side
export const OptimisationWorker = {
  finishEarly() {
    shouldContinue = false
  },
  reset() {
    shouldContinue = true
    progressUpdates.complete()
    progressUpdates = new Subject()
  },
  progressUpdates() {
    return Observable.from(progressUpdates)
  },

  optimise(
    sparseBagToImport: MovementJSON[],
    settings: Settings,
    partialUpdate: boolean,
    orderingCache?: OrderingCache,
  ) {
    const updateProgress = async (progress: Progress): Promise<Continue> => {
      progressUpdates.next(progress)

      // Partial updates stop after the first iteration
      if (partialUpdate) return false

      // need to wait for a microtick for other calls to come in
      await new Promise((resolve, reject) => setTimeout(resolve))

      return shouldContinue
    }

    return optimiseFrameBag(
      sparseBagToImport,
      settings,
      updateProgress,
      orderingCache,
    )
  },
}

expose(OptimisationWorker)
