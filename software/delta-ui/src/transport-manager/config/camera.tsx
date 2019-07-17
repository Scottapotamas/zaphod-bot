import {
  CameraHintConsumerFactory,
  CameraHintProducer,
  HintValidatorCamera,
  CameraTransportFactory,
  CameraTransport,
} from '../../camera-device'

import * as GPhoto from '@typedproject/gphoto2-driver'

const cameraConsumer = CameraHintConsumerFactory(GPhoto, 'EOS')
const cameraProducer = new CameraHintProducer({
  GPhoto,
})

export { cameraConsumer, cameraProducer }
