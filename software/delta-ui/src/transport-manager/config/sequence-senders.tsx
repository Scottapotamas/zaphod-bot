import {
  Device,
  DeviceManager,
  MANAGER_EVENTS,
  Message,
} from '@electricui/core'

import { DeviceManagerProxyPlugin } from '@electricui/components-core'
import { SequenceSenderPlugin } from './sequence-sender'
import { getDelta } from './actions/utils'

export const movementQueueSequencer = new SequenceSenderPlugin({
  maxQueueDepth: 75,
  name: 'mv',
  deviceManagerChunkWriter: async (
    deviceManager: DeviceManager,
    chunk: any,
  ) => {
    // chunk is a momement
    const delta = getDelta(deviceManager)

    const message = new Message('inmv', chunk)
    message.metadata.ack = true

    return delta.write(message)
  },
  incomingQueueDepthMessageFilter: (
    deviceManager: DeviceManager,
    message: Message,
  ) => {
    let delta = null

    try {
      delta = getDelta(deviceManager)
    } catch (e) {
      return false
    }

    return message.deviceID === delta.deviceID && message.messageID === 'queue'
  },
  incomingQueueDepthMessageTransform: (
    deviceManager: DeviceManager,
    message: Message,
  ) => {
    return message.payload.movements
  },
  queueDepthChangeCallback: (deviceManager: DeviceManager, depth: number) => {
    const delta = getDelta(deviceManager)

    if (depth % 10 === 0) {
      delta.addMetadata({
        uiSideMovementQueueDepth: depth,
      })
    }
  },
})

export const lightQueueSequencer = new SequenceSenderPlugin({
  maxQueueDepth: 125,
  name: 'li',
  deviceManagerChunkWriter: async (
    deviceManager: DeviceManager,
    chunk: any,
  ) => {
    // chunk is a light move
    const delta = getDelta(deviceManager)

    const message = new Message('inlt', chunk)
    message.metadata.ack = true

    return delta.write(message)
  },
  incomingQueueDepthMessageFilter: (
    deviceManager: DeviceManager,
    message: Message,
  ) => {
    let delta = null

    try {
      delta = getDelta(deviceManager)
    } catch (e) {
      return false
    }

    return message.deviceID === delta.deviceID && message.messageID === 'queue'
  },
  incomingQueueDepthMessageTransform: (
    deviceManager: DeviceManager,
    message: Message,
  ) => {
    return message.payload.lighting
  },
  queueDepthChangeCallback: (deviceManager: DeviceManager, depth: number) => {
    const delta = getDelta(deviceManager)

    if (depth % 10 === 0) {
      delta.addMetadata({
        uiSideLightQueueDepth: depth,
      })
    }
  },
})
