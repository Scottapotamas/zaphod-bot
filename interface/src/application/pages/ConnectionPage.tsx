import { Button, Classes } from '@blueprintjs/core'
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
      <div
        style={{
          minWidth: '450px',
          minHeight: '40vh',
          paddingTop: '20vh',
          marginRight: '10vw',
          float: 'right'
        }}
      >
        <Connections
          preConnect={deviceID => navigate(`/device_loading/${deviceID}`)}
          postHandshake={deviceID => navigate(`/devices/${deviceID}`)}
          onFailure={(deviceID, err) => {
            console.log('Connections component got error', err, deviceID)
            navigate(`/`)
          }}
          internalCardComponent={<CardInternals />}
        />

        <Button
          fill
          onClick={() => navigate(`/toolpaths/`)}
          style={{
            marginTop: '2em',
          }}
        >
          Standalone Toolpath Viewer
        </Button>
      </div>
    </Landing>
  )
}
