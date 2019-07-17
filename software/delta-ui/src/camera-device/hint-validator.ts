import {
  DeviceCandidate,
  DiscoveryHintValidator,
  Hint,
  Message,
} from '@electricui/core'

const deviceType = 'Camera'

const dValidator = require('debug')('electricui-device-camera:validator')

export default class HintValidatorCamera extends DiscoveryHintValidator {
  canValidate(hint: Hint): boolean {
    console.log(hint.getTransportKey())
    if (hint.getTransportKey() === 'gphoto') {
      return true
    }
    return false
  }

  async startValidation() {
    const identification = this.hint.getIdentification()

    const model = identification.model as string

    dValidator(`running validator for the ${model}`)

    // ask for the serialnumber
    const message = new Message('/status/serialnumber', null)
    message.metadata.query = true

    const serialNumber = await this.connection.write(message)

    console.log('serial number', serialNumber)

    const boardID = `camera-${serialNumber}`

    const candidate = new DeviceCandidate(boardID, this.connection)

    candidate.setMetadata({
      type: deviceType,
      model: model,
    })

    this.pushDeviceCandidate(candidate)

    this.complete()
  }
}
