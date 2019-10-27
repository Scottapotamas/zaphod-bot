import { DeviceManagerProxyPlugin } from '@electricui/components-core'
import { Device, Message, MANAGER_EVENTS } from '@electricui/core'

const Victor = require('victor')
const tinycolor = require('tinycolor2')

export class XboxColours extends DeviceManagerProxyPlugin {
  xboxCache: Map<string, number> = new Map()
  shouldTick: boolean = false
  timer: NodeJS.Timeout

  constructor() {
    super()

    this.timer = setInterval(this.tick, 10)
  }

  tick = () => {
    if (!this.shouldTick) {
      return
    }

    const leftThumbHorizontal = this.xboxCache.get('leftThumbHorizontal') || 0
    const leftThumbVertical = this.xboxCache.get('leftThumbVertical') || 0
    const leftTrigger = this.xboxCache.get('leftTrigger') || 0
    const rightTrigger = this.xboxCache.get('rightTrigger') || 0

    const left = Victor(leftThumbHorizontal, leftThumbVertical)

    const leftAngle = left.angleDeg() + 180
    const leftMagnitude = left.magnitude()

    const color = tinycolor.fromRatio({
      h: leftAngle,
      s: Math.min(leftMagnitude, 1),
      v: 1 - Math.min(leftTrigger, 1),
    })

    const rgb = color.toRgb() // { r: 255, g: 0, b: 0, a: 1 }

    const msgColor = new Message('rgb', [rgb.r, rgb.g, rgb.b])
    msgColor.metadata.type = 8 // TYPES.UINT16
    msgColor.metadata.ack = true

    const msgBrightness = new Message(
      'bright',
      Math.floor((1 - rightTrigger) * 255),
    )
    msgBrightness.metadata.type = 6 // TYPES.UINT8
    msgBrightness.metadata.ack = true

    for (const device of this.deviceManager!.devices.values()) {
      if (device.getMetadata().type === 'Xbox One Controller') {
        continue
      }

      device.write(msgColor).catch(console.log)
      device.write(msgBrightness).catch(console.log)
    }
    this.shouldTick = false
  }

  onMessage = (device: Device, message: Message) => {
    if (device.getMetadata().type !== 'Xbox One Controller') {
      return
    }

    this.shouldTick = true
    this.xboxCache.set(message.messageID, message.payload)
  }

  setupProxyHandlers() {
    this.deviceManager!.on(MANAGER_EVENTS.DATA, this.onMessage)
  }

  teardownProxyHandlers() {
    this.deviceManager!.removeListener(MANAGER_EVENTS.DATA, this.onMessage)
  }
}
