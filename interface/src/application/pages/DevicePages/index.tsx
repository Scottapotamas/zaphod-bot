import './device-pages.css'

import { RouteComponentProps, Router } from '@reach/router'
import React from 'react'

import { DisconnectionModal } from '@electricui/components-desktop-blueprint'
import { navigate } from '@electricui/utility-electron'
import { Intent } from '@blueprintjs/core'

import Header from '../../components/Header'
import MainWindow from './MainWindow'

interface InjectDeviceIDFromLocation {
  deviceID?: string
}

const DevicePages = (
  props: RouteComponentProps & InjectDeviceIDFromLocation,
) => {
  if (!props.deviceID) {
    return <div>No deviceID?</div>
  }

  return (
    <React.Fragment>
      <DisconnectionModal
        intent={Intent.WARNING}
        icon="satellite"
        navigateToConnectionsScreen={() => navigate('/')}
      >
        <p>
          The communications link isn't stable. Attempting to recover the
          connection...
        </p>
      </DisconnectionModal>

      <div className="device-pages">
        <Header deviceID={props.deviceID} {...props} />
        <div className="device-content">
          <MainWindow />
        </div>
      </div>
    </React.Fragment>
  )
}

export default DevicePages
