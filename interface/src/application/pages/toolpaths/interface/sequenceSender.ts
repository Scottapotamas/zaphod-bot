import { CancellationToken, Deferred } from '@electricui/core'
import { LightMove, MovementMove } from '../../../../application/typedState'
import { Toolpath } from './../optimiser/toolpath'

export type SendMovement = (movement: MovementMove) => Promise<void>
export type SendLightMove = (lightMove: LightMove) => Promise<void>
export type SendClear = () => Promise<void>
export type RequestQueueUpdates = () => Promise<void>
export type UpdateQueueInProgress = (
  movementQueueDepth: number,
  lightQueueDepth: number,
) => void

export class SequenceSender {
  // The estimated movement queue depth, updated optimistically
  private hardwareMovementQueueDepth = 0
  private hardwareLightMoveQueueDepth = 0

  // TODO: refactor with the concept that the queue is expressed as usage percentage
  //       rather than a specific element count
  private movementQueueWatermark = 50
  private lightMoveQueueWatermark = 50

  private movementMoves: MovementMove[] = []
  private lightMoves: LightMove[] = []

  private interval: NodeJS.Timeout | null = null

  private isRendering = false
  private finalMovementTimestamp = 0
  private cancellationToken = new CancellationToken()

  private completionDeferred = new Deferred<void>()
  private initialBatchDeferred = new Deferred<void>()

  constructor(
    private sendMovement: SendMovement,
    private sendLightMove: SendLightMove,
    private sendClear: SendClear,
    private requestQueueUpdates: RequestQueueUpdates,
    private updateOptimisticQueueDepth: UpdateQueueInProgress,
  ) {
    this.clear = this.clear.bind(this)
    this.tick = this.tick.bind(this)
    this.ingest = this.ingest.bind(this)
    this.stopTimers = this.stopTimers.bind(this)
    this.notifyUIOfOptimisticQueues = this.notifyUIOfOptimisticQueues.bind(this)
    this.updateHardwareQueues = this.updateHardwareQueues.bind(this)
    this.updateHardwareProgress = this.updateHardwareProgress.bind(this)

    this.waitForFrameToComplete = this.waitForFrameToComplete.bind(this)
    this.waitForInitialBatch = this.waitForInitialBatch.bind(this)
  }

  stopTimers() {
    if (this.interval) {
      clearInterval(this.interval)
      this.interval = null
    }
  }

  startTimers() {
    if (this.interval) {
      clearInterval(this.interval)
      this.interval = null
    }

    this.interval = setInterval(this.tick, 50)
  }

  async clear() {
    // clear the queue, cancel anything we were sending out
    this.movementMoves = []
    this.lightMoves = []
    this.isRendering = false

    this.completionDeferred = new Deferred<void>()
    this.initialBatchDeferred = new Deferred<void>()

    this.notifyUIOfOptimisticQueues()
    this.stopTimers()
    await this.sendClear()
  }

  async ingest(toolpath: Toolpath, cancellationToken: CancellationToken) {
    this.cancellationToken = cancellationToken
    await this.clear()

    this.movementMoves = toolpath.movementMoves
    this.lightMoves = toolpath.lightMoves

    this.finalMovementTimestamp =
      this.movementMoves[this.movementMoves.length - 1].sync_offset

    this.startTimers()
  }

  notifyUIOfOptimisticQueues() {
    this.updateOptimisticQueueDepth(
      this.movementMoves.length,
      this.lightMoves.length,
    )
  }

  waitForFrameToComplete() {
    return this.completionDeferred.promise
  }

  waitForInitialBatch() {
    return this.initialBatchDeferred.promise
  }

  async tick() {
    if (this.isRendering) {
      // Request a queue update to start
      await this.requestQueueUpdates()
    }

    // If both queues are empty, don't do anything in the tick
    if (this.movementMoves.length === 0 && this.lightMoves.length === 0) {
      return
    }

    // While both queues are empty and there is at least one movement in the queue
    while (
      this.hardwareMovementQueueDepth < this.movementQueueWatermark &&
      this.hardwareLightMoveQueueDepth < this.lightMoveQueueWatermark &&
      (this.movementMoves.length > 0 || this.lightMoves.length > 0)
    ) {
      if (this.cancellationToken.isCancelled()) {
        this.completionDeferred.reject(this.cancellationToken.token)
        this.initialBatchDeferred.reject(this.cancellationToken.token)
        this.clear()
        return
      }
      // Peek the next movement and potentially the next light move
      const movement: MovementMove | undefined = this.movementMoves[0]
      const firstLightMove: LightMove | undefined = this.lightMoves[0]

      // If there are movements left, intersperse the light moves between them
      if (movement) {
        // If the light move should be sent before the next movement, send it first
        if (firstLightMove && firstLightMove.timestamp < movement.sync_offset) {
          this.hardwareLightMoveQueueDepth++
          const shifted = this.lightMoves.shift()!
          this.notifyUIOfOptimisticQueues()

          await this.sendLightMove(shifted)
          continue
        }

        // Shift the movement off the queue and send it now
        this.hardwareMovementQueueDepth++
        const shifted = this.movementMoves.shift()!
        this.notifyUIOfOptimisticQueues()

        await this.sendMovement(shifted)
        this.isRendering = true
        continue
      }

      // Otherwise there are only light moves left, send the next one
      this.hardwareLightMoveQueueDepth++
      const shifted = this.lightMoves.shift()!
      this.notifyUIOfOptimisticQueues()

      await this.sendLightMove(shifted)
    }

    // The initial batch has been sent
    this.initialBatchDeferred.resolve()

    // request a queue update when we're done
    await this.requestQueueUpdates()
  }

  updateHardwareQueues(motionDepth: number, fadeDepth: number) {
    this.hardwareMovementQueueDepth = motionDepth
    this.hardwareLightMoveQueueDepth = fadeDepth

    // console.log(
    //   `hardware update received ${motionDepth} (${this.movementMoves.length}), ${fadeDepth} (${this.lightMoves.length})`,
    // )
  }

  updateHardwareProgress(movementID: number) {
    if (!this.isRendering) return

    if (movementID === this.finalMovementTimestamp) {
      this.isRendering = false
      this.completionDeferred.resolve()
    }

    // console.log(
    //   `hardware update received ${motionDepth} (${this.movementMoves.length}), ${fadeDepth} (${this.lightMoves.length})`,
    // )
  }
}
