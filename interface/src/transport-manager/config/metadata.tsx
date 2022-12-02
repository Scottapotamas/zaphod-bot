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

// Request and process firmware from the delta, and the fixture ID from the DMX utility

class RequestFirmwareMetadata extends DiscoveryMetadataRequester {
  name = 'request-metadata'

  canRequestMetadata(device: Device) {
    return true
  }

  requestMetadata(device: Device) {
    const nameRequest = new Message(MSGID.FIRMWARE_INFO, null)
    nameRequest.metadata.query = true
    nameRequest.metadata.internal = false

    const cancellationToken = new CancellationToken('request firmware build info metadata').deadline(1_000)

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

class RequestDMXMetadata extends DiscoveryMetadataRequester {
  name = 'request-dmx-metadata'

  canRequestMetadata(device: Device) {
    return true
  }

  requestMetadata(device: Device) {
    const nameRequest = new Message("fixture", null)
    nameRequest.metadata.query = true
    nameRequest.metadata.internal = false

    const cancellationToken = new CancellationToken('request dmx data').deadline(1_000)

    return device
      .write(nameRequest, cancellationToken)
      .then(res => {
        console.log('Requested dmx fixture data, response:', res)
      })
      .catch(err => {
        console.log("Couldn't request dmx fixture data err:", err)
      })
  }
}

class ProcessFirmwareInfo extends DiscoveryMetadataProcessor {
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
        firmware_info: message.payload,
        device_type: 'delta',
      })
    }
  }
}

class ProcessDMXDeviceType extends DiscoveryMetadataProcessor {
  isRelevantMessage(message: Message, device: Device) {
    // if this is an ack packet, ignore it
    if (message.metadata.ackNum > 0 && message.payload === null) {
      return false
    }

    // if it's a fixtures packet, it must be the DMX thing
    if (message.messageID === "fixture") {
      return true
    }

    return false
  }

  processMetadata(message: Message, device: Device, foundHint: FoundHint) {
    if (message.messageID === "fixture") {
      device.addMetadata({
        device_type: 'dmx',
      })
    }
  }
}

export { RequestFirmwareMetadata, ProcessFirmwareInfo, ProcessDMXDeviceType, RequestDMXMetadata }
