import React from 'react'

import { Composition, Box } from 'atomic-layout'

import { Colors, Callout, Tag, Intent, IconName } from '@blueprintjs/core'
import { IconNames } from '@blueprintjs/icons'

import { useHardwareState } from '@electricui/components-core'

import { MSGID, SUPERVISOR_STATES, ServoInfo } from '../../../typedState'
import { Printer } from '@electricui/components-desktop'

const servoColor: Intent[] = [
  Intent.SUCCESS,
  Intent.DANGER,
  Intent.PRIMARY,
  Intent.WARNING,
]

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

interface ServoStatusBlockProps {
  motorIndex: number
}

const ServoStatusBlock = (props: ServoStatusBlockProps) => {
  const servoState = useHardwareState(
    state => state[MSGID.SERVO][props.motorIndex].state,
  )

  let operation_mode: string
  let status_icon: IconName

  switch (servoState) {
    case 0:
      operation_mode = 'INACTIVE'
      status_icon = IconNames.DELETE
      break
    case 1:
      operation_mode = 'DISABLING'
      status_icon = IconNames.DISABLE
      break
    case 2:
      operation_mode = 'CALIBRATING FEEDBACK'
      status_icon = IconNames.HISTORY
      break
    case 3:
      operation_mode = 'ENDSTOP SEARCH'
      status_icon = IconNames.RESET
      break
    case 4:
      operation_mode = 'ENDSTOP FOUND'
      status_icon = IconNames.UPDATED
      break
    case 5:
      operation_mode = 'FOLDBACK CHECK'
      status_icon = IconNames.SELECTION
      break
    case 6:
      operation_mode = 'HOMING SUCCESS'
      status_icon = IconNames.CONFIRM
      break
    case 7:
      operation_mode = 'IDLE'
      status_icon = IconNames.LAYOUT_CIRCLE
      break
    case 8:
      operation_mode = 'HIGH LOAD'
      status_icon = IconNames.ISSUE
      break
    case 9:
      operation_mode = 'MOVING'
      status_icon = IconNames.SOCIAL_MEDIA
      break
    default:
      operation_mode = 'INVALID MODE'
      status_icon = IconNames.HELP
      break
  }

  return (
    <Tag
      fill
      minimal
      large
      style={{
        fontWeight: 'bold',
        fontSize: 'larger',
        textAlign: 'center',
        height: '100%',
      }}
    >
      {operation_mode}
    </Tag>
  )
}

const TargetAngleTag = (props: ServoStatusBlockProps) => {

  return (
    <Tag
      minimal
      fill
      intent={servoColor[props.motorIndex]}
      style={{
        fontWeight: 'bold',
        minWidth: '50px',
        textAlign: 'center',
      }}
    >
      <Printer accessor={state => state[MSGID.SERVO][props.motorIndex].target_angle} precision={1}/> °

    </Tag>
  )
}

const FeedbackTag = (props: ServoStatusBlockProps) => {

  return (
    <Tag
      minimal
      fill
      intent={servoColor[props.motorIndex]}
      style={{
        fontWeight: 'bold',
        minWidth: '50px',
        textAlign: 'center',
      }}
    >
      <Printer accessor={state => state[MSGID.SERVO][props.motorIndex].feedback} precision={1}/> %
    </Tag>
  )
}

const PowerTag = (props: ServoStatusBlockProps) => {

  return (
    <Tag
      minimal
      fill
      intent={servoColor[props.motorIndex]}
      style={{
        fontWeight: 'bold',
        minWidth: '50px',
        textAlign: 'center',

      }}
    >
      <Printer accessor={state => state[MSGID.SERVO][props.motorIndex].power} precision={1}/> W
    </Tag>
  )
}

const layoutDescription = `
  State 
  Angle
  Feedback 
  Power
`

const ServoStats = (props: MotorData) => {
  return (
    <React.Fragment>
      <Composition
        areas={layoutDescription}
        templateCols=" 1fr"
        gapCol={10}
        gapRow={5}
      >
        {Areas => (
          <React.Fragment>
            <Areas.State>
              <ServoStatusBlock motorIndex={props.index} />
            </Areas.State>
            <Areas.Angle>
              <TargetAngleTag motorIndex={props.index} />
            </Areas.Angle>
            <Areas.Feedback>
              <FeedbackTag motorIndex={props.index} />
            </Areas.Feedback>
            <Areas.Power>
              <PowerTag motorIndex={props.index} />
            </Areas.Power>
          </React.Fragment>
        )}
      </Composition>
    </React.Fragment>
  )
}

export const ServoSummary = () => {
  const motors = useHardwareState(state => state[MSGID.SERVO])

  if (!motors) {
    return <span>No motor telemetry available...</span>
  }

  return (
    <Composition gap={10} >
      {motors.map((clearpath, index) => (
        <Box key={index} style={{gridColumnStart: index+1}} width="1fr">
          <ServoStats servo={clearpath} index={index} />
        </Box>
      ))}
    </Composition>
  )
}
