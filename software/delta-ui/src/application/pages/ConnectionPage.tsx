import { Button } from '@blueprintjs/core'
import { Connections } from '../components/ConnectionDeviceList'
import Logo from '../components/Logo'
import React from 'react'
import { RouteComponentProps } from '@reach/router'
import { ipcRenderer } from 'electron'
import { navigate } from '@electricui/utility-electron'

const ConnectionPage = (props: RouteComponentProps) => {
  return (
    <React.Fragment>
      <div style={{ height: '100vh' }}>
        <Logo />

        <Connections
          preConnect={deviceID => {
            navigate(`/device_loading/${deviceID}`)
          }}
          postHandshake={deviceID => {
            navigate(`/devices/${deviceID}`)
          }}
          onFailure={deviceID => {
            navigate(`/`)
          }}
          style={{
            minHeight: '40vh',
            paddingTop: '10vh',
          }}
        />
      </div>
      <Button
        onClick={() => {
          ipcRenderer.send('open-debug-window')
        }}
        style={{ position: 'fixed', bottom: 10, right: 10 }}
      >
        Show transport window
      </Button>
    </React.Fragment>
  )
}

export default ConnectionPage
