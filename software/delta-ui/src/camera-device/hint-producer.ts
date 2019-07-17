import { DiscoveryHintProducer, Hint } from '@electricui/core'

import * as GPhoto from '@typedproject/gphoto2-driver'

interface CameraHintProducerOptions {
  transportKey?: string
  GPhoto: typeof GPhoto
}

export default class CameraHintProducer extends DiscoveryHintProducer {
  transportKey: string
  options: CameraHintProducerOptions
  GPhoto: typeof GPhoto

  constructor(options: CameraHintProducerOptions) {
    super()

    this.transportKey = options.transportKey || 'gphoto'
    this.options = options

    this.GPhoto = options.GPhoto
  }

  async poll() {
    this.setPolling(true)

    const cameraList = new this.GPhoto.CameraList().load()

    if (!this.polling) {
      // if we were cancelled just don't send them up.
      return
    }

    for (const [index, device] of cameraList.toArray().entries()) {
      const port = device.port
      const model = device.model
      // const camera = cameraList.getCamera(index)

      // This is blocking and may take a while so we check at the top of the loop.
      // const serialNumber = camera.widgets.get('/status/serialnumber')

      const hint = new Hint(this.transportKey)

      hint.setAvailabilityHint()

      hint.setIdentification({
        port,
        model,
        //  serialNumber,
      })

      this.foundHint(hint)

      // camera.closeQuietly()
    }

    this.setPolling(false)
  }
}
