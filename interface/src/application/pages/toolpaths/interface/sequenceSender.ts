import { LightMove, MovementMove } from './../optimiser/hardware'
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

  private movementQueueWatermark = 30
  private lightMoveQueueWatermark = 30

  private movementMoves: MovementMove[] = []
  private lightMoves: LightMove[] = []

  private interval: NodeJS.Timeout | null = null

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

    this.interval = setInterval(this.tick, 200)
  }

  async clear() {
    // clear the queue, cancel anything we were sending out
    this.movementMoves = []
    this.lightMoves = []

    this.notifyUIOfOptimisticQueues()
    this.stopTimers()
    await this.sendClear()
  }

  async ingest(toolpath: Toolpath) {
    await this.clear()

    this.movementMoves = toolpath.movementMoves
    this.lightMoves = toolpath.lightMoves

    this.startTimers()
  }

  notifyUIOfOptimisticQueues() {
    this.updateOptimisticQueueDepth(
      this.movementMoves.length,
      this.lightMoves.length,
    )
  }

  async tick() {
    // If both queues are empty, don't do anything in the tick
    if (this.movementMoves.length === 0 && this.lightMoves.length === 0) {
      return
    }

    // Request a queue update to start
    await this.requestQueueUpdates()

    // While both queues are empty and there is at least one movement in the queue
    while (
      this.hardwareMovementQueueDepth < this.movementQueueWatermark &&
      this.hardwareLightMoveQueueDepth < this.lightMoveQueueWatermark &&
      (this.movementMoves.length > 0 || this.lightMoves.length > 0)
    ) {
      // Peek the next movement and potentially the next light move
      const movement: MovementMove | undefined = this.movementMoves[0]
      const firstLightMove: LightMove | undefined = this.lightMoves[0]

      // If there are movements left, intersperse the light moves between them
      if (movement) {
        // If the light move should be sent before the next movement, send it first
        if (firstLightMove && firstLightMove.id < movement.id) {
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
        continue
      }

      // Otherwise there are only light moves left, send the next one
      this.hardwareLightMoveQueueDepth++
      const shifted = this.lightMoves.shift()!
      this.notifyUIOfOptimisticQueues()

      await this.sendLightMove(shifted)
    }

    // request a queue update when we're done
    await this.requestQueueUpdates()
  }

  updateHardwareQueues(motionDepth: number, fadeDepth: number) {
    this.hardwareMovementQueueDepth = motionDepth
    this.hardwareLightMoveQueueDepth = fadeDepth

    this.hardwareLightMoveQueueDepth = depth
  }
}
