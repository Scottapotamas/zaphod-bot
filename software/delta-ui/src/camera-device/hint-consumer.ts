import { DiscoveryHintConsumer, Hint } from '@electricui/core'

import CameraTransportFactory from './transport-factory'

export default function CameraHintConsumerFactory(
  GPhoto: any,
  cameraType: string = 'EOS',
) {
  const consumer = new DiscoveryHintConsumer({
    factory: CameraTransportFactory,
    canConsume: (hint: Hint) => {
      if (hint.getTransportKey() === 'gphoto') {
        const identification = hint.getIdentification()

        return identification.model.includes(cameraType)
      }
      return false
    },
    configure: (hint: Hint) => {
      const identification = hint.getIdentification()

      return {
        GPhoto: GPhoto,
        port: identification.port,
      }
    },
  })

  return consumer
}
