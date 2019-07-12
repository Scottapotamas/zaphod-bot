import React from 'react'
import { Navbar, Button, Alignment } from '@blueprintjs/core'
import { RouteComponentProps } from '@reach/router'
import { navigate } from '@electricui/utility-electron'
import {
  IntervalRequester,
  useHardwareState,
  useDeviceConnect,
  useDeviceDisconnect,
  useDeviceConnectionRequested,
} from '@electricui/components-core'
import { SetDarkModeButton } from '../SetDarkModeButton'

interface InjectDeviceIDFromLocation {
  deviceID?: string
  '*'?: string // we get passed the path as the wildcard
}

const Header = (props: RouteComponentProps & InjectDeviceIDFromLocation) => {
  const disconnect = useDeviceDisconnect()
  const connect = useDeviceConnect()

  const page = props['*'] // we get passed the path as the wildcard, so we read it here

  return (
    <div className="device-header">
      <Navbar style={{ background: 'transparent', boxShadow: 'none' }}>
        <div style={{ margin: '0 auto', width: '100%' }}>
          <Navbar.Group align={Alignment.LEFT}>
            <Button
              minimal
              large
              icon="home"
              text="Back"
              onClick={() => {
                navigate('/')
              }}
            />
            {/* <SetDarkModeButton /> */}
          </Navbar.Group>
          <Navbar.Group>SYSTEM IDLE</Navbar.Group>

          <Navbar.Group align={Alignment.RIGHT}>
            <Button
              minimal
              large
              icon="dashboard"
              text="Control"
              onClick={() => {
                navigate(`/devices/${props.deviceID}/`)
              }}
              active={page === ''}
            />

            <Button
              minimal
              large
              icon="timeline-line-chart"
              text="Detail"
              onClick={() => {
                navigate(`/devices/${props.deviceID}/detail`)
              }}
              active={page === 'detail'}
            />
            <Navbar.Divider />
            <Button
              minimal
              large
              icon="settings"
              text="System"
              onClick={() => {
                navigate(`/devices/${props.deviceID}/system`)
              }}
              active={page === 'secondary'}
            />
          </Navbar.Group>
        </div>
      </Navbar>
    </div>
  )
}

export default Header
