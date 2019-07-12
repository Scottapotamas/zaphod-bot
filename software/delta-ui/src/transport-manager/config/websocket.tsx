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
  BinaryPipeline,
  BinaryTypeCachePipeline,
  DeliverabilityManagerBinaryProtocol,
  QueryManagerBinaryProtocol,
} from '@electricui/protocol-binary'
import { COBSPipeline } from '@electricui/protocol-binary-cobs'
import { defaultCodecList } from '@electricui/protocol-binary-codecs'
import { HeartbeatConnectionMetadataReporter } from '@electricui/protocol-binary-heartbeats'
import { WebSocketTransport } from '@electricui/transport-node-websocket'

import { customCodecs } from './codecs'

const typeCache = new TypeCache()

// Websocket
const WebSocket = require('ws')
const websocketTransportFactory = new TransportFactory(options => {
  const connectionInterface = new ConnectionInterface()

  const transport = new WebSocketTransport(options)

  const deliverabilityManager = new DeliverabilityManagerBinaryProtocol({
    connectionInterface,
    timeout: 1000,
  })

  const queryManager = new QueryManagerBinaryProtocol({
    connectionInterface,
    timeout: 1000,
  })

  const cobsPipeline = new COBSPipeline()
  const binaryPipeline = new BinaryPipeline()
  const typeCachePipeline = new BinaryTypeCachePipeline(typeCache)

  const codecPipeline = new CodecDuplexPipeline()

  // Create instances of the custom codecs

  // Pass the array of custom codecs to the pipeline
  codecPipeline.addCodecs(customCodecs)

  codecPipeline.addCodecs(defaultCodecList)

  const connectionStaticMetadata = new ConnectionStaticMetadataReporter({
    name: 'Websocket',
    uri: options.uri,
  })

  const heartbeatMetadata = new HeartbeatConnectionMetadataReporter({
    interval: 2000,
    timeout: 5000,
    measurePipeline: true,
  })

  connectionInterface.setTransport(transport)
  connectionInterface.setQueryManager(queryManager)
  connectionInterface.setDeliverabilityManager(deliverabilityManager)
  connectionInterface.setPipelines([
    cobsPipeline,
    binaryPipeline,
    codecPipeline,
    typeCachePipeline,
  ])
  connectionInterface.addMetadataReporters([
    connectionStaticMetadata,
    heartbeatMetadata,
  ])

  return connectionInterface.finalise()
})

const websocketConsumer = new DiscoveryHintConsumer({
  factory: websocketTransportFactory,
  canConsume: (hint: Hint) => {
    return hint.getTransportKey() === 'websockets'
  },
  configure: (hint: Hint) => {
    const configuration = hint.getConfiguration()

    return {
      WebSocket,
      uri: configuration.uri,
    }
  },
})

export { websocketConsumer }
