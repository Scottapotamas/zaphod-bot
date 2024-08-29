import { FrameMovementJSON } from './import'
import { Progress } from './passes'
import { SerialisedTour } from './movements'
import type { Settings } from './settings'
import { spawn, Pool, Worker, ModuleThread } from 'threads'
import type { OptimisationWorker } from './workers/worker'

export enum FRAME_STATE {
  OPTIMISING_PARTIALLY = 0,
  OPTIMISING_FULLY = 1,
  UNOPTIMISED = 2,
  OPTIMISED_PARTIALLY = 3,
  OPTIMISED_FULLY = 4,
  ERRORED = 5,
}

export interface FrameProgressUpdate {
  frameNumber: number
  duration: number
  timeSpent: number
  currentCost: number
  serialisedTour: SerialisedTour

  frameState: FRAME_STATE
}

export class Deferred<T> {
  promise!: Promise<T>
  resolve!: (val: T) => void
  reject!: (err: any) => void
  constructor() {
    this.promise = new Promise<T>((resolve, reject) => {
      this.resolve = resolve
      this.reject = reject
    })
  }
}

/**
 * A persistent orchestrator of frame optimisation.
 */
export class ToolpathGenerator {
  private viewportFrame: number = 0
  private requestedFrame: number = 0

  private minFrameNumber: number = -Infinity
  private maxFrameNumber: number = Infinity

  /**
   * The list of frames that aren't finished
   */
  private unfinishedFrames: number[] = []
  private frameDuration: Map<number, number> = new Map()
  private frameState: Map<number, FRAME_STATE> = new Map()
  private frameCost: Map<number, FRAME_STATE> = new Map()

  private frameSubscriptions: Map<number, Deferred<void>> = new Map()
  private onCompleteDeferred = new Deferred<void>()

  private movementJSON: Map<number, FrameMovementJSON> = new Map()

  private pool: Pool<ModuleThread<typeof OptimisationWorker>>

  constructor(
    private settings: Settings,
    private numThreads = 4,
    private onUpdate: (progress: FrameProgressUpdate)=> void,
    private onFrameStateChange: (frameNumber: number, duration: number, frameState: FRAME_STATE) => void
  ) {
    this.reset = this.reset.bind(this)
    this.ingest = this.ingest.bind(this)
    this.currentWorkQueue = this.currentWorkQueue.bind(this)
    this.scheduleWork = this.scheduleWork.bind(this)
    this.setFrameState = this.setFrameState.bind(this)
    this.scheduleFrame = this.scheduleFrame.bind(this)
    this.setViewedFrame = this.setViewedFrame.bind(this)
    this.waitUntilFrameReady = this.waitUntilFrameReady.bind(this)
    this.onComplete = this.onComplete.bind(this)
    this.updateSettings = this.updateSettings.bind(this)
    this.teardown = this.teardown.bind(this)
    this.setFrameMinimum = this.setFrameMinimum.bind(this)
    this.setFrameMaximum = this.setFrameMaximum.bind(this)

    console.log(`Booting ${numThreads} workers`)

    this.pool = Pool(
      () =>
        spawn(
          // @ts-expect-error
          new Worker(new URL('./workers/worker.ts', import.meta.url)),
        ),
      numThreads /* Must be at least 2 */,
    )
  }

  reset() {
    this.unfinishedFrames = []
    this.frameDuration.clear()
    this.frameState.clear()
    this.frameCost.clear()
    this.movementJSON.clear()
    this.frameSubscriptions.clear()
    this.onCompleteDeferred = new Deferred<void>()
  }

  public ingest(
    movementJSONByFrame: {
      [frame: number]: FrameMovementJSON
    },
    settings: Settings,
  ) {
    this.reset()

    // Update the settings
    this.settings = settings

    for (const frameNumber of Object.keys(movementJSONByFrame)) {
      const num = Number(frameNumber)
      // Set all frames to unoptimised
      this.setFrameState(num, FRAME_STATE.UNOPTIMISED)

      // Set all frames to Infinite cost initially
      this.frameCost.set(num, Infinity)

      // Store the movement JSON
      this.movementJSON.set(num, movementJSONByFrame[num])

      // Add the frame to the unfinished frames list
      this.unfinishedFrames.push(num)
    }

    // Immediately schedule work
    this.scheduleWork()
  }

  setFrameMinimum(min: number) {
    this.minFrameNumber = min
    this.scheduleWork()
  }

  setFrameMaximum(max: number) {
    this.maxFrameNumber = max
    this.scheduleWork()
  }

  /**
   * Sort the unfinished frames so a partial
   */
  private sortWorkPriority() {
    this.unfinishedFrames.sort((a, b) => {
      // The requested frame and viewport frame have highest priority
      if (a === this.requestedFrame) return -1
      if (b === this.requestedFrame) return 1
      if (a === this.viewportFrame) return -1
      if (b === this.viewportFrame) return 1

      const frameAStatus = this.frameState.get(a) ?? FRAME_STATE.ERRORED
      const frameBStatus = this.frameState.get(b) ?? FRAME_STATE.ERRORED

      // If frames have different states
      if (frameAStatus !== frameBStatus) {
        // Give priority to lower optimisation level frames
        // UNOPTIMISED before PARTIALLY before FULLY
        return frameAStatus - frameBStatus
      }

      // Otherwise do it in frame order
      return a - b
    })
  }

  private currentWorkQueue() {
    this.sortWorkPriority()

    const workQueue: number[] = []
    let needed = this.numThreads

    for (let index = 0; index < this.unfinishedFrames.length; index++) {
      if (needed === 0) break

      const frameNumber = this.unfinishedFrames[index]

      if (frameNumber < this.minFrameNumber) continue
      if (frameNumber > this.maxFrameNumber) continue

      workQueue.push(frameNumber)
      needed--
    }

    return workQueue
  }

  /**
   * Diff the current pool state
   */
  scheduleWork() {
    // Schedule the current work queue
    for (const frameNumber of this.currentWorkQueue()) {
      switch (this.frameState.get(frameNumber)) {
        case FRAME_STATE.UNOPTIMISED:
          // Start a partial optimisation pass
          this.scheduleFrame(frameNumber, true)
          continue
        case FRAME_STATE.OPTIMISING_PARTIALLY:
          // Already optimising, don't allow for concurrent optimisation passes on the same frame
          continue
        case FRAME_STATE.OPTIMISING_FULLY:
          // Already optimising, don't allow for concurrent optimisation passes on the same frame
          continue
        case FRAME_STATE.OPTIMISED_PARTIALLY:
          // Start a full optimisation pass
          this.scheduleFrame(frameNumber, false)
          continue
        case FRAME_STATE.OPTIMISED_FULLY:
          // Already done
          continue

        default:
          continue
      }
    }
  }

  setFrameState(frameNumber: number, state: FRAME_STATE) {
    this.frameState.set(frameNumber, state)

    // Send an update 
    this.onFrameStateChange(frameNumber, this.frameDuration.get(frameNumber) ?? 0, state)

    if (state === FRAME_STATE.OPTIMISED_FULLY) {
      // Remove the frame from the unfinished list if it's fully optimised
      this.unfinishedFrames = this.unfinishedFrames.filter(
        fN => fN !== frameNumber,
      )

      // Notify any listeners
      if (this.frameSubscriptions.has(frameNumber)) {
        this.frameSubscriptions.get(frameNumber)!.resolve()

        // And delete the frameSub
        this.frameSubscriptions.delete(frameNumber)
      }

      // Check if there's nothing left
      if (this.unfinishedFrames.length === 0) {
        this.onCompleteDeferred.resolve()
      }
    }
  }

  scheduleFrame(frameNumber: number, partialOptimisation: boolean) {
    this.setFrameState(frameNumber, partialOptimisation ? FRAME_STATE.OPTIMISING_PARTIALLY : FRAME_STATE.OPTIMISING_FULLY)

    const captureSettingsReference = this.settings

    this.pool.queue(async worker => {
      let updates = 0

      const receiveUpdate = async (progress: Progress) => {
        // If the settings have changed, dump the current work queue
        if (captureSettingsReference !== this.settings) {
          await worker.finishEarly()
          return
        }

        updates++

        // Update our status if this is the last progress update
        if (!this.currentWorkQueue().includes(frameNumber)) {
          // If the current work queue doesn't include this frame, stop early
          await worker.finishEarly()
        }

        // Check if the update improves the cost, otherwise dump it and raise a warning
        const cachedCost = (this.frameCost.get(frameNumber) ?? Infinity)
        if (cachedCost < progress.currentCost) {
          console.log(`Update to frame ${frameNumber} was cost ${progress.currentCost} which is more than ${cachedCost}, why did this get propagated?`)
          return
        }

        const cachedDuration = (this.frameDuration.get(frameNumber) ?? Infinity)

        if (cachedDuration < progress.duration ) {
          console.log(`Update to frame ${frameNumber} was duration ${progress.duration} which is more than ${cachedDuration}, why did this get propagated? old cost was ${cachedCost}, new cost is ${progress.currentCost}`)
          return
        }

        this.frameCost.set(frameNumber, progress.currentCost)
        this.frameDuration.set(frameNumber, progress.duration)

        // Pass the updates up the chain
        this.onUpdate({
          frameNumber: frameNumber,
          duration: progress.duration,
          timeSpent: progress.timeSpent,
          currentCost: progress.currentCost,
          frameState: this.frameState.get(frameNumber) ?? FRAME_STATE.UNOPTIMISED,
          serialisedTour: progress.serialisedTour,
        })
      }

      // Reset the worker before we start
      await worker.reset()

      const sub = worker.progressUpdates().subscribe(
        receiveUpdate,
        error => {
          console.error(`Error in worker`, error)
          console.log(error, error.stack)
        },
        () => {
          console.log(`worker completed`)
        },
      )

      // Start the optimisation pass
      await worker.optimise(
        frameNumber,
        this.movementJSON.get(frameNumber)!,
        this.settings,
        partialOptimisation, // if this is a partial update, stop after the first iteration
      )

      this.setFrameState(frameNumber, partialOptimisation ? FRAME_STATE.OPTIMISED_PARTIALLY : FRAME_STATE.OPTIMISED_FULLY)

      // Schedule the next batch of work
      this.scheduleWork()

      sub.unsubscribe()
    })
  }

  public setViewedFrame(frameNumber: number) {
    this.viewportFrame = frameNumber

    this.scheduleWork()
  }

  public async waitUntilFrameReady(frameNumber: number) {
    // If this frame is complete, immediately resolve with the toolpath
    if (!this.unfinishedFrames.includes(frameNumber)) {
      return
    }

    this.requestedFrame = frameNumber

    this.scheduleWork()

    // If we already have a wait on this frame
    if (this.frameSubscriptions.has(frameNumber)) {
      return this.frameSubscriptions.get(frameNumber)!.promise
    }

    // Create a deferred for this frame's toolpath
    const deferred = new Deferred<void>()

    this.frameSubscriptions.set(frameNumber, deferred)

    return deferred.promise
  }

  public onComplete() {
    return this.onCompleteDeferred.promise
  }

  updateSettings(settings: Settings) {
    // Re-injest everything with new settings

    const movementJSON: {
      [frameNumber: number]: FrameMovementJSON
    } = {}

    for (const [frameNumber, json] of this.movementJSON) {
      movementJSON[frameNumber] = json
    }

    this.ingest(movementJSON, settings)
  }

  teardown() {
    this.reset()
    return this.pool.terminate(true)
  }
}
