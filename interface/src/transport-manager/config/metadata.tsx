import {
  CancellationToken,
  Device,
  DiscoveryMetadataProcessor,
  DiscoveryMetadataRequester,
  FoundHint,
  Hint,
  Message,
} from '@electricui/core'

import { MSGID } from '../../application/typedState'

// Request and Process a 'name' for every device

class RequestName extends DiscoveryMetadataRequester {
  name = 'request-name'

  canRequestMetadata(device: Device) {
    return true
  }

  requestMetadata(device: Device) {
    const nameRequest = new Message(MSGID.FIRMWARE_INFO, null)
    nameRequest.metadata.query = true
    nameRequest.metadata.internal = false

    const cancellationToken = new CancellationToken(
      'request firmware build info metadata',
    ).deadline(1_000)

    return device
      .write(nameRequest, cancellationToken)
      .then(res => {
        console.log('Requested fwb, response:', res)
      })
      .catch(err => {
        console.log("Couldn't request fwb err:", err)
      })
  }
}

class ProcessName extends DiscoveryMetadataProcessor {
  isRelevantMessage(message: Message, device: Device) {
    // if this is an ack packet, ignore it
    if (message.metadata.ackNum > 0 && message.payload === null) {
      return false
    }

    // if it's a firmware version packet, process it
    if (message.messageID === MSGID.FIRMWARE_INFO) {
      return true
    }

    return false
  }

  processMetadata(message: Message, device: Device, foundHint: FoundHint) {
    if (message.messageID === MSGID.FIRMWARE_INFO) {
      device.addMetadata({
        firmware_info: message.payload
      })
    }
  }
}

export { RequestName, ProcessName }