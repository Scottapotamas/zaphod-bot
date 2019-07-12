import {
  Device,
  DiscoveryMetadataProcessor,
  DiscoveryMetadataRequester,
  FoundHint,
  Hint,
  Message,
} from '@electricui/core'

// Request and Process 'ws' alternative connection hints

class RequestWS extends DiscoveryMetadataRequester {
  canRequestMetadata(device: Device) {
    const metadata = device.getMetadata()

    // Xbox controllers don't operate over WS
    if (metadata.type && metadata.type === 'Xbox Wireless Controller') {
      return false
    }

    // Mock devices don't operate over WS
    if (metadata.type && metadata.type === 'Mock Device') {
      return false
    }

    return true // always ask
  }

  requestMetadata(device: Device) {
    const wsPathRequest = new Message('ws', null)
    wsPathRequest.metadata.query = true
    wsPathRequest.metadata.internal = false
    wsPathRequest.metadata.ackNum = 0

    return device.write(wsPathRequest)
  }
}

class ProcessWS extends DiscoveryMetadataProcessor {
  isRelevantMessage(message: Message, device: Device) {
    // if this is an ack packet, ignore it
    if (message.metadata.ackNum > 0 && message.payload === null) {
      return false
    }

    // if it's a ws packet, process it
    if (message.messageID !== 'ws') {
      return false
    }

    // if it's a blank payload, don't process it
    if (message.payload === '') {
      return false
    }

    return true
  }

  processMetadata(message: Message, device: Device, foundHint: FoundHint) {
    // build the hint
    const hint = new Hint('websockets')

    // it has no identification
    hint.setConfiguration({
      uri: message.payload,
    })

    // push the hint up
    foundHint(hint)
  }
}

// Request and Process a 'name' for every device

class RequestName extends DiscoveryMetadataRequester {
  canRequestMetadata(device: Device) {
    return true
  }

  requestMetadata(device: Device) {
    const nameRequest = new Message('name', null)
    nameRequest.metadata.query = true
    nameRequest.metadata.internal = false
    nameRequest.metadata.ackNum = 0

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

class ProcessBatteryPercentage extends DiscoveryMetadataProcessor {
  isRelevantMessage(message: Message, device: Device) {
    // if this is an ack packet, ignore it
    if (message.metadata.ackNum > 0 && message.payload === null) {
      return false
    }

    // if it's a bat_percent packet, process it
    if (message.messageID === 'bat_percent') {
      return true
    }

    return false
  }

  processMetadata(message: Message, device: Device, foundHint: FoundHint) {
    if (message.messageID === 'bat_percent') {
      device.addMetadata({
        bat_percent: message.payload,
      })
    }
  }
}

export {
  RequestWS,
  ProcessWS,
  RequestName,
  ProcessName,
  ProcessBatteryPercentage,
}
