import {
  CancellationToken,
  Connection,
  ConnectionMetadataRatio,
  ConnectionMetadataRule,
  Device,
  DeviceManager,
  Hint,
  MessageRouterLogRatioMetadata,
  AutoConnectPlugin,
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
import { SERIAL_TRANSPORT_KEY } from '@electricui/transport-node-serial'

import { movementQueueSequencer, lightQueueSequencer } from './sequence-senders'

/**
 * Create our device manager!
 */
export const deviceManager = new DeviceManager()

function createRouter(device: Device) {
  const router = new MessageRouterLogRatioMetadata({
    device,
    ratios: [
      new ConnectionMetadataRatio('latency', false, 1, (sum: number, latency: number) => sum + latency), // prettier-ignore
      new ConnectionMetadataRatio('jitter', false, 0.1, (sum: number, jitter: number) => sum + jitter), // prettier-ignore
      new ConnectionMetadataRatio('packetLoss', false, 2, (factor: number, packetLoss: number) => factor * packetLoss), // prettier-ignore
      new ConnectionMetadataRatio('consecutiveHeartbeats', true, 0.1, (minimum: number, consecutiveHeartbeats: number) => Math.min(minimum, consecutiveHeartbeats)), // prettier-ignore
    ],
    rules: [
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
    ],
  })

  return router
}

function createQueue(device: Device) {
  return new MessageQueueBinaryFIFO({
    device,
    interval: 10,
    concurrentMessages: 100,
  })
}

function hintValidators(
  hint: Hint,
  connection: Connection,
  cancellationToken: CancellationToken,
) {
  // Serial
  if (hint.getTransportKey() === SERIAL_TRANSPORT_KEY) {
    const validator = new HintValidatorBinaryHandshake(
      hint,
      connection,
      cancellationToken,
      {
        attemptTiming: [0, 10, 100, 1000, 2000, 5000],
      },
    ) // 2 second timeout

    return [validator]
  }

  return []
}

function createHandshakes(
  device: Device,
  cancellationToken: CancellationToken,
) {
  // Assume it's an eUI device, do the binary handshakes
  const connectionHandshakeReadWrite = new BinaryConnectionHandshake({
    device: device,
    preset: 'default',
    cancellationToken,
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

// Setup plugins
const autoConnectPlugin = new AutoConnectPlugin(deviceManager)
// TODO: movementQueueSequencer, lightQueueSequencer
deviceManager.addPlugins([autoConnectPlugin])

// start polling immediately, poll for 10 seconds
const cancellationToken = new CancellationToken('inital poll').deadline(10_000)
deviceManager.poll(cancellationToken).catch(err => {
  if (cancellationToken.caused(err)) {
    console.log("Didn't find any devices on initial poll")
  }
})

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
