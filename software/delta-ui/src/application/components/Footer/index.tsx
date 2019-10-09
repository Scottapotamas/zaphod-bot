import {
  Accessor,
  IntervalRequester,
  useDeviceMetadataKey,
  useHardwareState,
} from '@electricui/components-core'
import { Alignment, Icon, Intent, Navbar, Text } from '@blueprintjs/core'

import { Button } from '@electricui/components-desktop-blueprint'
import { CALL_CALLBACK } from '@electricui/core'
import React from 'react'
import { RouteComponentProps } from '@reach/router'

interface InjectDeviceIDFromLocation {
  deviceID?: string
  '*'?: string // we get passed the path as the wildcard
}

const CPUText = () => {
  const cpu_percentage = useHardwareState(state => state.sys.cpu_load)
  let iconColour: Intent

  if (cpu_percentage >= 0 && cpu_percentage < 60) {
    iconColour = Intent.SUCCESS
  } else if (cpu_percentage > 60 && cpu_percentage < 85) {
    iconColour = Intent.WARNING
  } else {
    iconColour = Intent.DANGER
  }

  return (
    <div>
      <Icon icon="time" intent={iconColour} /> {cpu_percentage}%
    </div>
  )
}

const QueueText = () => {
  const queue_depth = useHardwareState(state => state.queue.movements)
  const is_moving = useHardwareState(state => state.moStat.pathing_state) == 1
  const queue_depth_ui = useDeviceMetadataKey('uiSideMovementQueueDepth')

  let iconColour: Intent

  if (queue_depth == 0) {
    iconColour = Intent.NONE
  } else if (queue_depth > 0 && queue_depth < 25) {
    if (is_moving) {
      iconColour = Intent.SUCCESS
    } else {
      iconColour = Intent.WARNING
    }
  } else {
    iconColour = Intent.DANGER
  }

  return (
    <div>
      <Icon icon="move" intent={iconColour} /> {queue_depth} ({queue_depth_ui})
    </div>
  )
}

const LEDQueueText = () => {
  const queue_depth = useHardwareState(state => state.queue.lighting)
  const queue_depth_ui = useDeviceMetadataKey('uiSideLightQueueDepth')

  let iconColour: Intent

  if (queue_depth == 0) {
    iconColour = Intent.NONE
  } else if (queue_depth > 0 && queue_depth < 35) {
    iconColour = Intent.SUCCESS
  } else {
    iconColour = Intent.WARNING
  }

  return (
    <div>
      <Icon icon="lightbulb" intent={iconColour} /> {queue_depth} (
      {queue_depth_ui})
    </div>
  )
}

const Footer = (props: RouteComponentProps & InjectDeviceIDFromLocation) => {
  // const page = props['*'] // we get passed the path as the wildcard, so we read it here

  const psu_voltage = useHardwareState(
    state => state.sys.input_voltage,
  ).toFixed(1)

  return (
    <div className="device-footer">
      <IntervalRequester interval={1000} variables={['sys']} />

      <Navbar style={{ background: 'transparent', boxShadow: 'none' }}>
        <div style={{ margin: '0 auto', width: '100%' }}>
          <Navbar.Group align={Alignment.LEFT}>
            <Text>
              <CPUText />
            </Text>
            <Navbar.Divider />
            <Text>
              <QueueText />
            </Text>
            <Navbar.Divider />
            <Text>
              <LEDQueueText />
            </Text>
            <Navbar.Divider />
            <Text>{psu_voltage}V Input</Text>
          </Navbar.Group>
          <Navbar.Group align={Alignment.RIGHT}>
            <div style={{ minWidth: '300px' }}>
              <Button
                fill
                large
                intent="danger"
                writer={{ estop: CALL_CALLBACK }}
              >
                <b>E-STOP</b>
              </Button>
            </div>
          </Navbar.Group>
        </div>
      </Navbar>
    </div>
  )
}

export default Footer
