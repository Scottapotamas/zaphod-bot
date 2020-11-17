import {
  BinaryPipeline,
  BinaryTypeCachePipeline,
  DeliverabilityManagerBinaryProtocol,
  QueryManagerBinaryProtocol,
  UndefinedMessageIDGuardPipeline,
} from '@electricui/protocol-binary'
import {
  CodecDuplexPipeline,
  ConnectionInterface,
  ConnectionStaticMetadataReporter,
  DiscoveryHintConsumer,
  Hint,
  TransportFactory,
  TypeCache,
} from '@electricui/core'
import {
  SerialPortHintProducer,
  SerialPortUSBHintTransformer,
  SerialTransport,
  SerialTransportOptions,
} from '@electricui/transport-node-serial'

import { BinaryLargePacketHandlerPipeline } from '@electricui/protocol-binary-large-packet-handler'
import { COBSPipeline } from '@electricui/protocol-binary-cobs'
import { HeartbeatConnectionMetadataReporter } from '@electricui/protocol-binary-heartbeats'
import SerialPort from 'serialport'
import USB from '@electricui/node-usb'
import { USBHintProducer } from '@electricui/transport-node-usb-discovery'
import { customCodecs } from './codecs'
import { defaultCodecList } from '@electricui/protocol-binary-codecs'

const typeCache = new TypeCache()

const serialProducer = new SerialPortHintProducer({
  SerialPort,
  baudRate: 500000,
})

const usbProducer = new USBHintProducer({
  USB,
})

// Serial Ports
const serialTransportFactory = new TransportFactory(
  (options: SerialTransportOptions) => {
    const connectionInterface = new ConnectionInterface()

    const transport = new SerialTransport(options)

    const deliverabilityManager = new DeliverabilityManagerBinaryProtocol({
      connectionInterface,
      timeout: 1000,
    })

    const queryManager = new QueryManagerBinaryProtocol({
      connectionInterface,
    })

    const cobsPipeline = new COBSPipeline()
    const binaryPipeline = new BinaryPipeline()
    const typeCachePipeline = new BinaryTypeCachePipeline(typeCache)

    // If you have runtime generated messageIDs, add them as an array as a second argument
    // `name` is added because it is requested by the metadata requester before handshake.
    const undefinedMessageIDGuard = new UndefinedMessageIDGuardPipeline(
      typeCache,
      ['name'],
    )

    const codecPipeline = new CodecDuplexPipeline()
    // Add the default codecs first so that queries are dealt with preferentially
    codecPipeline.addCodecs(defaultCodecList)
    // Add custom codecs after the default ones.
    codecPipeline.addCodecs(customCodecs)

    const largePacketPipeline = new BinaryLargePacketHandlerPipeline({
      connectionInterface,
      maxPayloadLength: 100,
    })

    const connectionStaticMetadata = new ConnectionStaticMetadataReporter({
      name: 'Serial',
      baudRate: options.baudRate,
    })

    const heartbeatMetadata = new HeartbeatConnectionMetadataReporter({
      interval: 500,
      timeout: 1000,
      // measurePipeline: true,
    })

    connectionInterface.setTransport(transport)
    connectionInterface.setQueryManager(queryManager)
    connectionInterface.setDeliverabilityManager(deliverabilityManager)
    connectionInterface.setPipelines([
      cobsPipeline,
      binaryPipeline,
      largePacketPipeline,
      codecPipeline,
      typeCachePipeline,
      undefinedMessageIDGuard,
    ])
    connectionInterface.addMetadataReporters([
      connectionStaticMetadata,
      heartbeatMetadata,
    ])

    return connectionInterface.finalise()
  },
)

const serialConsumer = new DiscoveryHintConsumer({
  factory: serialTransportFactory,
  canConsume: (hint: Hint) => {
    if (hint.getTransportKey() === 'serial') {
      // If you wanted to filter for specific serial devices, you would modify this section, removing the
      // return statement below and uncommenting the block below it, modifying it to your needs.

      const identification = hint.getIdentification()

      // Filter out any /dev/ttyS____ comPaths since they're almost certainly terminals
      if (identification.comPath.startsWith('/dev/ttyS')) {
        return false
      }

      // An example of filtering devices with Arduino or Silicon in the manufacturers
      /*
      return (
        identification.manufacturer && (
          identification.manufacturer.includes('Arduino') ||
          identification.manufacturer.includes('Silicon'))
      )
      */

      // Try any device that isn't filtered out by this stage
      return true
    }
    return false
  },
  configure: (hint: Hint) => {
    const identification = hint.getIdentification()
    const configuration = hint.getConfiguration()

    const options: SerialTransportOptions = {
      SerialPort,
      comPath: identification.comPath,
      baudRate: configuration.baudRate,
      // if you have an Arduino that resets on connection, uncomment this line to delay the connection
      // attachmentDelay: 2500,
    }

    return options
  },
})

const usbToSerialTransformer = new SerialPortUSBHintTransformer({
  producer: serialProducer,
})

export { serialConsumer, serialProducer, usbProducer, usbToSerialTransformer }
