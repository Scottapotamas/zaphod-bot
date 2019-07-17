import { Device, DeviceHandshake, Message } from '@electricui/core'

export default class DeviceHandshakeCamera extends DeviceHandshake {
  constructor(device: Device) {
    super(device)
    this.device = device
  }

  async onSubscribe() {
    const handshakeMessage = new Message('__handshake', null)
    const handshakeResults = await this.device.write(handshakeMessage)

    console.log('doing handshake')

    this.complete()
  }
}
