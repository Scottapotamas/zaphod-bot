import {
  CancellationToken,
  Device,
  DiscoveryMetadataProcessor,
  DiscoveryMetadataRequester,
  FoundHint,
  Hint,
  Message,
} from '@electricui/core'

// Request and Process a 'name' for every device

class RequestName extends DiscoveryMetadataRequester {
  name = 'request-name'

  canRequestMetadata(device: Device) {
    return true
  }

  requestMetadata(device: Device) {
    const nameRequest = new Message('name', null)
    nameRequest.metadata.query = true
    nameRequest.metadata.internal = false

    const cancellationToken = new CancellationToken(
      'request name metadata',
    ).deadline(1_000)

    return device
      .write(nameRequest, cancellationToken)
      .then(res => {
        console.log('Requested name, response:', res)
      })
      .catch(err => {
        console.log("Couldn't request name err:", err)
      })
  }
}

class ProcessName extends DiscoveryMetadataProcessor {
  isRelevantMessage(message: Message, device: Device) {
    // if this is an ack packet, ignore it
    if (message.metadata.ackNum > 0 && message.payload === null) {
      return false
    }

    // if it's a name packet, process it
    if (message.messageID === 'name') {
      return true
    }

    return false
  }

  processMetadata(message: Message, device: Device, foundHint: FoundHint) {
    if (message.messageID === 'name') {
      device.addMetadata({
        name: message.payload,
      })
    }
  }
}

export { RequestName, ProcessName }
