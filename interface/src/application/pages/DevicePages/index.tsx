import './device-pages.css'

import { RouteComponentProps, Router } from '@reach/router'

import { DisconnectionModal } from '@electricui/components-desktop-blueprint'
import { Intent } from '@blueprintjs/core'
import React from 'react'
import { navigate } from '@electricui/utility-electron'

import MainWindow from './MainWindow'
import { IconNames } from '@blueprintjs/icons'

interface InjectDeviceIDFromLocation {
  deviceID?: string
}

export const DevicePages = (
  props: RouteComponentProps & InjectDeviceIDFromLocation,
) => {
  if (!props.deviceID) {
    return <div>No deviceID?</div>
  }

  return (
    <React.Fragment>
      <DisconnectionModal
        intent={Intent.WARNING}
        icon={IconNames.WARNING_SIGN}
        navigateToConnectionsScreen={() => navigate('/')}
        disconnectText="DISCONNECT"
        backText=""
      >
        <h2
          style={{
            textAlign: 'center',
            fontWeight: 'initial',
            opacity: '0.8',
            marginTop: '0.25em',
          }}
        >
          UNSTABLE COMMUNICATIONS
        </h2>
      </DisconnectionModal>

      <div className="device-pages">
        <div className="device-content">
          <MainWindow />
        </div>
      </div>
    </React.Fragment>
  )
}
