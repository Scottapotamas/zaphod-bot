import { Observable, Subject } from 'threads/observable'
import { expose } from 'threads/worker'

import { importJson, MovementJSON, FrameMovementJSON } from '../import'
import { Movement, SerialisedTour } from '../movements'
import { Continue, optimise, Progress } from '../passes'
import { preprocess } from '../preprocess'
import type { Settings } from '../settings'
import { CancellationToken } from '@electricui/async-utilities'

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
    sparseBagToImport: FrameMovementJSON,
    settings: Settings,
    partialUpdate: boolean,
    debugInfo: any,
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

      const filepath = sparseBagToImport.filepath

      // Process the raw objects into movements
      for (const json of sparseBagToImport.movementJSON) {
        const imported = importJson(filepath, json)
        for (const movement of await imported.toMovements(settings, false)) {
          movements.push(movement)
        }
      }

      // Run the optimiser
      await optimise(
        preprocess(movements, settings),
        partialUpdate,
        settings,
        updateProgress,
        debugInfo,
      )
    } catch (e) {
      progressUpdates.error(e)
    }
  },
}

expose(OptimisationWorker)
