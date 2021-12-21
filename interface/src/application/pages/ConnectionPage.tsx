import { Classes } from '@blueprintjs/core'
import { RouteComponentProps } from '@reach/router'

import { Connections } from '@electricui/components-desktop-blueprint'
import { Landing } from '../components/Landing'
import React from 'react'
import { navigate } from '@electricui/utility-electron'
import { useDeviceMetadataKey } from '@electricui/components-core'

const CardInternals = () => {
  const metadataName = useDeviceMetadataKey('name') || 'No name'

  return (
    <React.Fragment>
      <h3 className={Classes.HEADING}>{metadataName}</h3>
      <p>Device information!</p>
    </React.Fragment>
  )
}

export const ConnectionPage = (props: RouteComponentProps) => {
  return (
    <Landing>
      <Connections
        preConnect={deviceID => navigate(`/device_loading/${deviceID}`)}
        postHandshake={deviceID => navigate(`/devices/${deviceID}`)}
        onFailure={(deviceID, err) => {
          console.log('Connections component got error', err, deviceID)
          navigate(`/`)
        }}
        style={{
          minHeight: '40vh',
          paddingTop: '15vh',
          marginRight: '15vw',
        }}
        internalCardComponent={<CardInternals />}
      />
    </Landing>
  )
}
