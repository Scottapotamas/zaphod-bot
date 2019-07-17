import {
  Connection,
  ConnectionMetadataRatio,
  ConnectionMetadataRule,
  Device,
  DeviceManager,
  Hint,
  MessageRouterLogRatioMetadata,
} from '@electricui/core'

import { HintValidatorBinaryHandshake } from '@electricui/protocol-binary'
import { BinaryConnectionHandshake } from '@electricui/protocol-binary-connection-handshake'
import { MessageQueueBinaryFIFO } from '@electricui/protocol-binary-fifo-queue'
import { ActionsPlugin } from '@electricui/core-actions'

import {
  ProcessBatteryPercentage,
  ProcessName,
  ProcessWS,
  RequestName,
  RequestWS,
} from './metadata'

import {
  serialConsumer,
  serialProducer,
  usbProducer,
  usbToSerialTransformer,
} from './serial'
import { websocketConsumer } from './websocket'
import actions from './actions'

import { cameraConsumer, cameraProducer } from './camera'
import { HintValidatorCamera, DeviceHandshakeCamera } from '../../camera-device'

import { AutoConnectPlugin } from './autoconnect'

//import { bleConsumer, bleProducer } from './ble'

const deviceManager = new DeviceManager()

function createRouter(device: Device) {
  return new MessageRouterLogRatioMetadata({ device, reportRankings: true })
}
function createQueue(device: Device) {
  return new MessageQueueBinaryFIFO({
    device,
    interval: 10,
    concurrentMessages: 100,
  })
}

function hintValidators(hint: Hint, connection: Connection) {
  const identification = hint.getIdentification()

  // Serial
  if (hint.getTransportKey() === 'serial') {
    const validator = new HintValidatorBinaryHandshake(hint, connection, 2000) // 2 second timeout

    return [validator]
  }

  // Wifi
  if (hint.getTransportKey() === 'websockets') {
    const validator = new HintValidatorBinaryHandshake(hint, connection, 5000) // 5 second timeout

    return [validator]
  }

  // BLE
  if (hint.getTransportKey() === 'ble') {
    const validator = new HintValidatorBinaryHandshake(hint, connection, 10000) // 10 second timeout

    return [validator]
  }

  // Camera
  if (hint.getTransportKey() === 'gphoto') {
    const validator = new HintValidatorCamera(hint, connection) // 10 second timeout

    return [validator]
  }

  return []
}

function createHandshakes(device: Device) {
  const metadata = device.getMetadata()

  const deviceType = metadata.type

  if (deviceType === 'Camera') {
    return [new DeviceHandshakeCamera(device)]
  }

  // Otherwise its an eUI device, do the binary handshakes
  const connectionHandshakeReadWrite = new BinaryConnectionHandshake({
    device: device,
    preset: 'default',
  })

  return [connectionHandshakeReadWrite]
}

const requestName = new RequestName()
const processName = new ProcessName()
const requestWS = new RequestWS()
const processWS = new ProcessWS()
const processBatteryPercentage = new ProcessBatteryPercentage()

deviceManager.setCreateHintValidatorsCallback(hintValidators)
deviceManager.addHintProducers([serialProducer, usbProducer, cameraProducer])
deviceManager.addHintConsumers([
  serialConsumer,
  websocketConsumer,
  cameraConsumer,
])
deviceManager.addHintTransformers([usbToSerialTransformer])
deviceManager.addDeviceMetadataRequesters([requestName])
deviceManager.addDiscoveryMetadataProcessors([
  processName,
  processWS,
  processBatteryPercentage,
])
deviceManager.setCreateRouterCallback(createRouter)
deviceManager.setCreateQueueCallback(createQueue)
deviceManager.setCreateHandshakesCallback(createHandshakes)

deviceManager.addConnectionMetadataRatios([
  new ConnectionMetadataRatio('latency', false, 1),
  new ConnectionMetadataRatio('jitter', false, 0.1),
  new ConnectionMetadataRatio('packetLoss', false, 2),
])

deviceManager.addConnectionMetadataRules([
  new ConnectionMetadataRule(['latency'], ({ latency }) => latency < 400),
  new ConnectionMetadataRule(
    ['packetLoss', 'consecutiveHeartbeats'],
    ({ packetLoss, consecutiveHeartbeats }) => {
      // If there are more than three consecutive heartbeats, then for
      // packet loss reasons, the connection is acceptable
      if (consecutiveHeartbeats > 3) {
        return true
      }

      // Otherwise we require less than 20% packet loss
      return packetLoss <= 0.2
    },
  ),
])
const actionsPlugin = new ActionsPlugin()
actionsPlugin.addActions(actions)

const autoConnectPlugin = new AutoConnectPlugin([
  { type: 'Camera' },
  // { name: 'Zaphod Beeblebot' },
])

deviceManager.addPlugins([actionsPlugin, autoConnectPlugin])

// start polling immediately!
deviceManager.poll()

export default deviceManager
