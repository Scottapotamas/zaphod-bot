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

// TODO: Have the SequenceSender be aware of the connection acceptability and not retry if the connection isn't acceptable

export class SequenceSender {
  // The estimated movement queue depth, updated optimistically
  private hardwareMovementQueueDepth = 0
  private hardwareLightMoveQueueDepth = 0

  // How far should we fill the buffers
  private movementQueueWatermark = 50
  private lightMoveQueueWatermark = 50

  // The queues of movemenst we need to send
  private movementMoves: MovementMove[] = []
  private lightMoves: LightMove[] = []

  // When are we done
  private finalMovementSyncOffset = 0
  private completionDeferred = new Deferred<void>()
  private cancellationToken = new CancellationToken()

  constructor(
    private sendMovement: SendMovement,
    private sendLightMove: SendLightMove,
    private sendClear: SendClear,
    private requestQueueUpdates: RequestQueueUpdates,
    private updateOptimisticQueueDepth: UpdateQueueInProgress,
  ) {
    this.clear = this.clear.bind(this)
    this.ingest = this.ingest.bind(this)
    this.notifyUIOfOptimisticQueues = this.notifyUIOfOptimisticQueues.bind(this)
    this.updateHardwareQueuesAndProgress = this.updateHardwareQueuesAndProgress.bind(this)
    this.waitForFrameToComplete = this.waitForFrameToComplete.bind(this)
    this.fill = this.fill.bind(this)
  }

  public async clear() {
    // clear the queue, cancel anything we were sending out
    this.movementMoves = []
    this.lightMoves = []

    this.completionDeferred = new Deferred<void>()

    this.notifyUIOfOptimisticQueues()
    await this.sendClear()
  }

  public async ingest(toolpath: Toolpath, cancellationToken: CancellationToken) {
    this.cancellationToken = cancellationToken

    // wipe the delta
    await this.clear()

    // Reset our filling
    this.isFilling = false

    // request queue depth
    await this.requestQueueUpdates()

    // setup our new queues
    this.movementMoves = toolpath.movementMoves
    this.lightMoves = toolpath.lightMoves

    this.finalMovementSyncOffset = this.movementMoves[this.movementMoves.length - 1].sync_offset

    // run an iteration of the fill
    return this.fill()
  }

  public notifyUIOfOptimisticQueues() {
    this.updateOptimisticQueueDepth(
      this.movementMoves.length,
      this.lightMoves.length,
    )
  }

  public waitForFrameToComplete() {
    return this.completionDeferred.promise
  }

  private isFilling = false

  private async fill() {
    if (this.isFilling) {
      // this instance is already filling
      return
    }

    this.isFilling = true

    // If both queues are empty, don't do anything in the tick
    if (this.movementMoves.length === 0 && this.lightMoves.length === 0) {
      this.isFilling = false
      return
    }

    const attemptsPerMove = 10

    // While both queues are empty and there is at least one movement in the queue
    while (
      this.hardwareMovementQueueDepth < this.movementQueueWatermark &&
      this.hardwareLightMoveQueueDepth < this.lightMoveQueueWatermark &&
      (this.movementMoves.length > 0 || this.lightMoves.length > 0)
    ) {

      // make sure we haven't cancelled
      if (this.cancellationToken.isCancelled()) {
        this.completionDeferred.reject(this.cancellationToken.token)
        this.clear()

        this.isFilling = false
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

          let attempts = 0

          // Try attemptsPerMove times
          while (!this.cancellationToken.isCancelled() && attempts < attemptsPerMove) {
            try {
              await this.sendLightMove(shifted)
              break // out of the infinite loop
            } catch (err) {
              console.error(`Failed to send light fade at timestamp ${shifted.timestamp}, attempt ${attempts}/${attemptsPerMove}, err:`, err)
            }
          }

          continue
        }

        // Shift the movement off the queue and send it now
        this.hardwareMovementQueueDepth++
        const shifted = this.movementMoves.shift()!

        let attempts = 0

        // Try attemptsPerMove times
        while (!this.cancellationToken.isCancelled() && attempts < attemptsPerMove) {
          attempts++
          try {
            await this.sendMovement(shifted)
            break // out of the infinite loop
          } catch (err) {
            console.error(`Failed to send light fade at timestamp ${shifted.sync_offset}, attempt ${attempts}/${attemptsPerMove}, err:`, err)
          }
        }

        continue
      }

      // Otherwise there are only light moves left, send the next one
      this.hardwareLightMoveQueueDepth++
      const shifted = this.lightMoves.shift()!

      let attempts = 0

      // Try attemptsPerMove times
      while (!this.cancellationToken.isCancelled() && attempts < attemptsPerMove) {
        try {
          await this.sendLightMove(shifted)
          break // out of the infinite loop
        } catch (err) {
          console.error(`Failed to send light fade at timestamp ${shifted.timestamp}, attempt ${attempts}/${attemptsPerMove}, err:`, err)
        }
      }
    }

    // Tell the UI about our new queues
    this.notifyUIOfOptimisticQueues()
    // we've sent either all the moves, or up to the watermark
    this.isFilling = false
  }

  public updateHardwareQueuesAndProgress(motionDepth: number, fadeDepth: number, syncOffset: number) {
    this.hardwareMovementQueueDepth = motionDepth
    this.hardwareLightMoveQueueDepth = fadeDepth

    // new hardware queues, 

    // console.log(
    //   `hardware update received ${motionDepth} (${this.movementMoves.length}), ${fadeDepth} (${this.lightMoves.length})`,
    // )

    if (syncOffset === this.finalMovementSyncOffset) {
      this.completionDeferred.resolve()
    } else if (!this.cancellationToken.isCancelled()) {
      // potentially run the queue again, if we haven't cancelled
      this.fill().catch(err => {
        if (this.cancellationToken.caused(err)) {
          // we have now cancelled
        } else {
          console.error(`Something went wrong trying to fill the queue after a hw update:`, err)
        }
      })
    }

    // console.log(
    //   `hardware update received ${motionDepth} (${this.movementMoves.length}), ${fadeDepth} (${this.lightMoves.length})`,
    // )
  }
}
