import React from 'react'

import { Composition, Box } from 'atomic-layout'

import {
  Card,
  Icon,
  Colors,
  Tag,
  Callout,
  Intent,
  IconName,
  Tooltip,
  Position,
} from '@blueprintjs/core'
import { Statistic, Statistics } from '@electricui/components-desktop-blueprint'
import { IconNames } from '@blueprintjs/icons'

import { useHardwareState } from '@electricui/components-core'
import { Printer } from '@electricui/components-desktop'

import { SUPERVISOR_STATES, ServoInfo } from '../../../typedState'

const MotorSafetyMode = () => {
  const motor_state = useHardwareState(state => state.super.motors)
  var motors_are_active: string = 'null'

  switch (motor_state) {
    case 0: {
      motors_are_active = 'OFF'
      break
    }
    case 1: {
      motors_are_active = 'ON'
      break
    }
    default: {
      motors_are_active = 'INVALID'
      break
    }
  }

  return <div>{motors_are_active}</div>
}

type MotorData = {
  servo: ServoInfo
  index: number
}

const ServoStats = (props: MotorData) => {
  let operation_mode: string
  let status_icon: IconName
  let servo_colour: Intent

  switch (props.servo.state) {
    case 0:
      operation_mode = 'Inactive'
      status_icon = IconNames.DELETE
      break
    case 1:
      operation_mode = 'Disabling'
      status_icon = IconNames.DISABLE
      break
    case 2:
      operation_mode = 'Calibrating Torque Feedback'
      status_icon = IconNames.HISTORY
      break
    case 3:
      operation_mode = 'Finding Endstop'
      status_icon = IconNames.RESET
      break
    case 4:
      operation_mode = 'Found Endstop'
      status_icon = IconNames.UPDATED
      break
    case 5:
      operation_mode = 'Checking Foldback'
      status_icon = IconNames.SELECTION
      break
    case 6:
      operation_mode = 'Homing Success'
      status_icon = IconNames.CONFIRM
      break
    case 7:
      operation_mode = 'Idle'
      status_icon = IconNames.LAYOUT_CIRCLE
      break
    case 8:
      operation_mode = 'Idle, High Load'
      status_icon = IconNames.ISSUE
      break
    case 9:
      operation_mode = 'Move'
      status_icon = IconNames.SOCIAL_MEDIA
      break
    default:
      operation_mode = 'Invalid Mode'
      status_icon = IconNames.HELP
      break
  }

  switch (props.index) {
    case 0:
      servo_colour = Intent.SUCCESS
      break
    case 1:
      servo_colour = Intent.DANGER
      break

    case 2:
      servo_colour = Intent.PRIMARY
      break
    case 3:
      servo_colour = Intent.WARNING
      break
    default:
      servo_colour = Intent.NONE
      break
  }

  return (
    <React.Fragment>
      <Callout intent={servo_colour} icon={status_icon}>
        <Composition
          templateCols="1fr 1fr 1fr 1fr"
          paddingHorizontal={20}
          gap={30}
          placeItems="center end"
          justifyContent="space-around"
          minWidth="280px"
        >
          <Box>
            <h4 className="bp3-heading">{operation_mode}</h4>
          </Box>
          <Box>
            <h4 className="bp3-heading">{props.servo.feedback.toFixed(1)}%</h4>
          </Box>
          <Box>
            <h4 className="bp3-heading">
              {props.servo.target_angle.toFixed(1)}°
            </h4>
          </Box>
          <Box>
            <h4 className="bp3-heading">{props.servo.power.toFixed(1)}W</h4>
          </Box>
        </Composition>
      </Callout>
    </React.Fragment>
  )
}

export const ServoSummary = () => {
  const motors: ServoInfo[] | null = useHardwareState(state => state.servo)
  if (motors === null) {
    return <span>No motor telemetry available...</span>
  }

  return (
      <Composition gap={10}>
        {motors.map((clearpath, index) => (
          <Box key={index}>
            <ServoStats servo={clearpath} index={index} />
          </Box>
        ))}
      </Composition>
  )
}