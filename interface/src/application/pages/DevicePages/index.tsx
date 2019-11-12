import './device-pages.css'

import { RouteComponentProps, Router } from '@reach/router'

import ControlPage from './ControlPage'
import DetailPage from './DetailPage'
import { DisconnectionModal } from '@electricui/components-desktop-blueprint'
import Footer from '../../components/Footer'
import Header from '../../components/Header'
import { Intent } from '@blueprintjs/core'
import React from 'react'
import { SceneController } from './SceneControl/SceneController'
import SystemPage from './SystemPage'
import { navigate } from '@electricui/utility-electron'

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
          <Router>
            <ControlPage path="/" />
            <SystemPage path="system" />
            <DetailPage path="detail" />
          </Router>
        </div>
        <Footer deviceID={props.deviceID} {...props} />
      </div>
    </React.Fragment>
  )
}

export default DevicePages
