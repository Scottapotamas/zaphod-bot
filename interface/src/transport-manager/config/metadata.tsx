import {
  Device,
  DiscoveryMetadataProcessor,
  DiscoveryMetadataRequester,
  FoundHint,
  Hint,
  Message,
} from '@electricui/core'

// Request and Process a 'name' for every device

class RequestName extends DiscoveryMetadataRequester {
  name: 'request-name'

  canRequestMetadata(device: Device) {
    return true
  }

  requestMetadata(device: Device) {
    const nameRequest = new Message('name', null)
    nameRequest.metadata.query = true
    nameRequest.metadata.internal = false

    return device
      .write(nameRequest)
      .then(res => {
        console.log('requested name, response:', res)
      })
      .catch(err => {
        console.log('Couldnt request name err:', err)
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
