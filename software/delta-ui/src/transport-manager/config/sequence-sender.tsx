import {
  DeviceManager,
  Device,
  Message,
  MANAGER_EVENTS,
} from '@electricui/core'
import { DeviceManagerProxyPlugin } from '@electricui/components-core'

export type QueueDepthRequester = (deviceManager: DeviceManager) => Promise<any>

export type DeviceManagerChunkWriter = (
  deviceManager: DeviceManager,
  chunk: any,
) => Promise<any>

export type IncomingQueueDepthMessageFilter = (
  deviceManager: DeviceManager,
  message: Message,
) => boolean

export type IncomingQueueDepthMessageTransform = (
  deviceManager: DeviceManager,
  message: Message,
) => number

export type QueueDepthChangeCallback = (
  deviceManager: DeviceManager,
  depth: number,
) => void

export interface SequenceSenderPluginOptions {
  /**
   * A function that writes a chunk to the device
   */
  deviceManagerChunkWriter: DeviceManagerChunkWriter

  /**
   * The statically defined maximum queue depth
   */
  maxQueueDepth: number

  /**
   * Returns if a message is the correct one to be transformed into our queue depth
   */
  incomingQueueDepthMessageFilter: IncomingQueueDepthMessageFilter

  /**
   * Transforms a message into the queue depth number
   */
  incomingQueueDepthMessageTransform: IncomingQueueDepthMessageTransform

  /**
   * Do something with the device manager when the queue depth changes
   */
  queueDepthChangeCallback: QueueDepthChangeCallback
}

export type SubscribeCallback = (depth: number) => void

export class SequenceSenderPlugin extends DeviceManagerProxyPlugin {
  queue: Array<any> = []
  maxQueueDepth: number
  currentQueueDepth: number = 0
  deviceManagerChunkWriter: DeviceManagerChunkWriter
  incomingQueueDepthMessageFilter: IncomingQueueDepthMessageFilter
  incomingQueueDepthMessageTransform: IncomingQueueDepthMessageTransform
  queueDepthChangeCallback: QueueDepthChangeCallback
  paused: boolean = true

  subscribers: Map<SubscribeCallback, boolean> = new Map()

  constructor(options: SequenceSenderPluginOptions) {
    super()

    this.maxQueueDepth = options.maxQueueDepth
    this.deviceManagerChunkWriter = options.deviceManagerChunkWriter
    this.incomingQueueDepthMessageFilter = options.incomingQueueDepthMessageFilter // prettier-ignore
    this.incomingQueueDepthMessageTransform = options.incomingQueueDepthMessageTransform // prettier-ignore
    this.queueDepthChangeCallback = options.queueDepthChangeCallback
  }

  onMessage = (device: Device, message: Message) => {
    if (this.incomingQueueDepthMessageFilter(this.deviceManager!, message)) {
      const currentQueueDepth = this.incomingQueueDepthMessageTransform(
        this.deviceManager!,
        message,
      )

      this.currentQueueDepth = currentQueueDepth

      this.writeSomethingIfWeCan()
    }
  }

  setupProxyHandlers() {
    this.deviceManager!.on(MANAGER_EVENTS.DATA, this.onMessage)
  }

  teardownProxyHandlers() {
    this.deviceManager!.removeListener(MANAGER_EVENTS.DATA, this.onMessage)
  }

  /**
   * Add something to the queue
   */
  public queueItem = (chunk: any) => {
    this.queue.push(chunk)

    this.setQueueRemaining(this.queue.length)

    this.writeSomethingIfWeCan()
  }

  /**
   * Checks if we can write anything
   */
  private writeSomethingIfWeCan = async () => {
    // If we have allowable queue depth and there's something in the queue, write it
    if (
      this.currentQueueDepth < this.maxQueueDepth &&
      this.queue.length > 0 &&
      !this.paused
    ) {
      const item = this.queue.shift()

      // optimistically increase the queue depth, it'll get reset quickly
      this.currentQueueDepth += 1

      // tell the UI how much is left in _our_ queue
      this.setQueueRemaining(this.queue.length)

      await this.deviceManagerChunkWriter(this.deviceManager!, item)

      this.writeSomethingIfWeCan()
    }
  }

  private setQueueRemaining = (depth: number) => {
    // Notify subscribers
    for (const cb of this.subscribers.keys()) {
      cb(depth)
    }

    // run our queue depth change callback
    this.queueDepthChangeCallback(this.deviceManager!, depth)
  }

  /**
   * Returns the current queue depth
   */
  public getQueueDepth = () => {
    return this.currentQueueDepth
  }

  /**
   * Subscribe to changes in queue depth, returns an unsubscribe function
   */
  public subscribeToQueueDepth = (callback: SubscribeCallback) => {
    this.subscribers.set(callback, true)

    // returns an unsubscribe function
    return () => {
      this.subscribers.delete(callback)
    }
  }

  /**
   * Pause and unpause the sequence sender
   */
  public setPaused = (paused: boolean) => {
    this.paused = paused
    if (!paused) {
      this.writeSomethingIfWeCan()
    }
  }
}
