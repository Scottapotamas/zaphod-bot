import {
  Alignment,
  Button,
  Navbar,
  Popover,
  Position,
  Tooltip,
} from '@blueprintjs/core'
import {
  IntervalRequester,
  useDeviceConnect,
  useDeviceConnectionRequested,
  useDeviceDisconnect,
  useHardwareState,
} from '@electricui/components-core'

import { BreadFactory } from '../../Toaster'
import React from 'react'
import { RouteComponentProps } from '@reach/router'
import { navigate } from '@electricui/utility-electron'

interface InjectDeviceIDFromLocation {
  deviceID?: string
  '*'?: string // we get passed the path as the wildcard
}

const Header = (props: RouteComponentProps & InjectDeviceIDFromLocation) => {
  const page = props['*'] // we get passed the path as the wildcard, so we read it here

  return (
    <div>
      <Navbar style={{ background: 'transparent', boxShadow: 'none' }}>
        <div style={{ margin: '0 auto', width: '100%' }}>
          <Navbar.Group align={Alignment.LEFT}>
            <Tooltip content="Back to device list" position={Position.BOTTOM}>
              <Button
                minimal
                large
                icon="key-backspace"
                onClick={() => {
                  navigate('/')
                }}
              />
            </Tooltip>
          </Navbar.Group>

          <Navbar.Group align={Alignment.RIGHT}>
            <Popover
              content={
                <div style={{ width: '200px', height: '400px' }}>
                  Settings go here!
                </div>
              }
              position={Position.BOTTOM}
            >
              <Tooltip content="Settings" position={Position.BOTTOM}>
                <Button minimal large icon="settings" />
              </Tooltip>
            </Popover>

            <Popover
              content={
                <div style={{ width: '200px', height: '400px' }}>
                  Info goes here
                </div>
              }
              position={Position.BOTTOM}
            >
              <Tooltip content="System Info" position={Position.BOTTOM}>
                <Button minimal large icon="info-sign" />
              </Tooltip>
            </Popover>
          </Navbar.Group>
        </div>
      </Navbar>
      <BreadFactory />
    </div>
  )
}

export default Header
