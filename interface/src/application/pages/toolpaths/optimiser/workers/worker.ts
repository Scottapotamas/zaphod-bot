import { Observable, Subject } from 'threads/observable'
import { expose } from 'threads/worker'

import { importJson, MovementJSON } from '../import'
import { Movement, SerialisedTour } from '../movements'
import {
  Continue,
  getTotalDuration,
  optimise,
  Progress,
  sparseToDense,
} from '../passes'
import type { Settings } from '../settings'

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

  async optimise(
    sparseBagToImport: MovementJSON[],
    settings: Settings,
    partialUpdate: boolean,
    debugInfo: any,
    cache?: SerialisedTour,
  ) {
    try {
      const updateProgress = async (progress: Progress): Promise<Continue> => {
        progressUpdates.next(progress)

        // need to wait for a microtick for other calls to come in
        await new Promise((resolve, reject) => setTimeout(resolve, 0))

        // Partial updates stop after the first iteration
        if (partialUpdate) return false

        return shouldContinue
      }

      const movements: Movement[] = []

      // Process the raw objects into movements
      for (const json of sparseBagToImport) {
        const imported = importJson(json)
        for (const movement of imported.toMovements(settings)) {
          movements.push(movement)
        }
      }

      // Run the optimiser
      await optimise(movements, partialUpdate, settings, updateProgress, debugInfo, cache)
    } catch (e) {
      progressUpdates.error(e)
    }
  },
}

expose(OptimisationWorker)
