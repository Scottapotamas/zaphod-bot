import {
  ConnectionInterface,
  DeliverabilityManagerDumb,
  QueryManagerNone,
  TransportFactory,
} from '@electricui/core'

import CameraTransport from './transport'

const CameraTransportFactory = new TransportFactory(options => {
  const connectionInterface = new ConnectionInterface()

  const transport = new CameraTransport(options)

  const deliverabilityManager = new DeliverabilityManagerDumb(
    connectionInterface,
  )
  const queryManager = new QueryManagerNone(connectionInterface)

  connectionInterface.setTransport(transport)
  connectionInterface.setQueryManager(queryManager)
  connectionInterface.setDeliverabilityManager(deliverabilityManager)

  connectionInterface.finalise()

  return connectionInterface
})

export default CameraTransportFactory
