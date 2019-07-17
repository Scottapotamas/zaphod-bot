import React from 'react'
import { RouteComponentProps } from '@reach/router'
import {
  IntervalRequester,
  useHardwareState,
} from '@electricui/components-core'
import { Navbar, Alignment, Text } from '@blueprintjs/core'
import { Button } from '@electricui/components-desktop-blueprint'

import { CALL_CALLBACK } from '@electricui/core'

interface InjectDeviceIDFromLocation {
  deviceID?: string
  '*'?: string // we get passed the path as the wildcard
}

const QueueText = () => {
  const queue_depth = useHardwareState(state => state.moStat.queue_depth)

  if (queue_depth > 0) {
    return <div>{queue_depth} queued</div>
  }

  return <div>Queue Emptyish</div>
}

const Footer = (props: RouteComponentProps & InjectDeviceIDFromLocation) => {
  const page = props['*'] // we get passed the path as the wildcard, so we read it here

  const cpu_percentage = useHardwareState(state => state.sys.cpu_load)
  const psu_voltage = useHardwareState(state =>
    state.sys.input_voltage.toFixed(1),
  )

  return (
    <div className="device-footer">
      <IntervalRequester interval={1000} variables={['sys']} />

      <Navbar style={{ background: 'transparent', boxShadow: 'none' }}>
        <div style={{ margin: '0 auto', width: '100%' }}>
          <Navbar.Group align={Alignment.LEFT}>
            <Text>{cpu_percentage}% CPU</Text>
            <Navbar.Divider />
            <Text>{psu_voltage}V Input</Text>
            <Navbar.Divider />
            <Text>
              <QueueText />
            </Text>
          </Navbar.Group>

          <Navbar.Group align={Alignment.RIGHT}>
            <div style={{ minWidth: '200px' }}>
              <Button
                fill
                large
                intent="danger"
                writer={{ estop: CALL_CALLBACK }}
              >
                STOP
              </Button>
            </div>
          </Navbar.Group>
        </div>
      </Navbar>
    </div>
  )
}

export default Footer
