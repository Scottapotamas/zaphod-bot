import {
  Connection,
  ConnectionMetadataRatio,
  ConnectionMetadataRule,
  Device,
  DeviceManager,
  Hint,
  MessageRouterLogRatioMetadata,
} from '@electricui/core'
import { ProcessName, RequestName } from './metadata'
import {
  serialConsumer,
  serialProducer,
  usbProducer,
  usbToSerialTransformer,
} from './serial'

import { BinaryConnectionHandshake } from '@electricui/protocol-binary-connection-handshake'
import { HintValidatorBinaryHandshake } from '@electricui/protocol-binary'
import { MessageQueueBinaryFIFO } from '@electricui/protocol-binary-fifo-queue'

import { ActionsPlugin } from '@electricui/core-actions'
import { AutoConnectPlugin } from './autoconnect'
import { movementQueueSequencer, lightQueueSequencer } from './sequence-senders'

import actions from './actions'

/**
 * Create our device manager!
 */
export const deviceManager = new DeviceManager()

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
    const validator = new HintValidatorBinaryHandshake(hint, connection, {
      timeout: 2000, // 2 second timeout
    })

    return [validator]
  }

  return []
}

function createHandshakes(device: Device) {
  const metadata = device.getMetadata()

  // Assume it's an eUI device, do the binary handshakes
  const connectionHandshakeReadWrite = new BinaryConnectionHandshake({
    device: device,
    preset: 'default',
  })

  return [connectionHandshakeReadWrite]
}

const requestName = new RequestName()
const processName = new ProcessName()

deviceManager.setCreateHintValidatorsCallback(hintValidators)
deviceManager.addHintProducers([serialProducer, usbProducer])
deviceManager.addHintConsumers([serialConsumer])
deviceManager.addHintTransformers([usbToSerialTransformer])
deviceManager.addDeviceMetadataRequesters([requestName])
deviceManager.addDiscoveryMetadataProcessors([processName])
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
      // If there are more than three consecutive heartbeats, the connection
      // is considered acceptable despite potential previous packet loss.
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
  // { type: 'Camera' },
  { name: 'Zaphod Beeblebot' },
])

deviceManager.addPlugins([
  actionsPlugin,
  autoConnectPlugin,
  movementQueueSequencer,
  lightQueueSequencer,
])

// start polling immediately.
deviceManager.poll()

if (module.hot) {
  /**
   * This handler accepts disposal events in the hot reload system.
   * It will tear down the device manager and record device and connection information,
   * passing it to the next module so connections don't go away.
   */
  module.hot.dispose((data: any) => {
    // record the devices we're currently connected to
    const devices = deviceManager.getDevicesMap()

    let deviceIDsWithUsageRequests = Array.from(devices.values())
      .filter(device => device.getUsageRequests().includes('ui'))
      .map(device => device.getDeviceID())

    data['deviceIDsToConnectTo'] = deviceIDsWithUsageRequests
  })

  /**
   * This handler accepts information from the old device manager and loads it in
   */
  if (module.hot.data && module.hot.data['deviceIDsToConnectTo']) {
    let deviceIDsWithUsageRequests = module.hot.data['deviceIDsToConnectTo']

    deviceManager.setDeviceIDsToAutoConnectForHotReloading(
      deviceIDsWithUsageRequests,
    )
  }
}
