import React, { useCallback, useState } from 'react'

import { Composition, Box } from 'atomic-layout'

import { Button, Colors, Collapse, Tag, Intent, IconName } from '@blueprintjs/core'
import { IconNames } from '@blueprintjs/icons'

import { useHardwareState } from '@electricui/components-core'

import { MSGID, SUPERVISOR_STATES, ServoInfo } from '../../../../typedState'
import { Printer } from '@electricui/components-desktop'

import { AngleChart } from './AngleChart'
import { RotationRateChart } from './RotationRateChart'
import { LoadChart } from './LoadChart'
import { PowerChart } from './PowerChart'

const servoColor: Intent[] = [
  Intent.SUCCESS,
  Intent.DANGER,
  Intent.PRIMARY,
  Intent.WARNING,
]

const MotorSafetyMode = () => {
  const motor_state = useHardwareState(state => state[MSGID.SUPERVISOR].motors)
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
      <Printer accessor={state => state[MSGID.SERVO][props.motorIndex].power} precision={0}/> W
    </Tag>
  )
}

const RotationRateTag = (props: ServoStatusBlockProps) => {

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
      <Printer accessor={state => state[MSGID.SERVO][props.motorIndex].speed} precision={0}/> °/s
    </Tag>
  )
}
const layoutDescription = `
  State State
  Angle RotationRate
  Feedback Power
`

const ServoStats = (props: MotorData) => {
  return (
    <div key={props.index} style={{ borderStyle: 'solid', borderWidth: '1px', borderRadius: '3px', borderColor: Colors.GRAY1}}>
      <Composition
        areas={layoutDescription}
        templateCols="1fr"
        gap={5}
        padding={5}
      >
        {Areas => (
          <React.Fragment>
            <Areas.State>
              <ServoStatusBlock motorIndex={props.index} />
            </Areas.State>
            <Areas.Angle>
              <TargetAngleTag motorIndex={props.index} />
            </Areas.Angle>
            <Areas.RotationRate>
              <RotationRateTag motorIndex={props.index} />
            </Areas.RotationRate>
            <Areas.Feedback>
              <FeedbackTag motorIndex={props.index} />
            </Areas.Feedback>
            <Areas.Power>
              <PowerTag motorIndex={props.index} />
            </Areas.Power>
          </React.Fragment>
        )}
      </Composition>
    </div>
  )
}

const ServoStatsBlock = () => {
  const motors = useHardwareState((state) => state[MSGID.SERVO]); //.filter(obj => obj.enabled)

  if (!motors) {
    return <span>No motor telemetry available...</span>;
  }

  return (
    <div style={{ display: "flex" }}>
      {motors.map((clearpath, index) => (
        <div style={{  padding: 5 }}>
          <ServoStats servo={clearpath} index={index} />
        </div>
      ))}
    </div>
  );
};

export const ServoSummary = () => {
  let [chartsExpanded, setChartsExpanded] = useState(false)

  const controlChartsExpansion = useCallback(_ => {
    setChartsExpanded(enabled => !enabled)
  }, [setChartsExpanded])

  return (
    <div>
      <ServoStatsBlock />

      <Button
        minimal
        outlined
        intent={Intent.NONE}
        small
        fill
        onClick={controlChartsExpansion}
        icon={
          chartsExpanded ? IconNames.CHEVRON_UP: IconNames.CHEVRON_DOWN
        }
      >
        {/* Consider a label here to explain the UX? */}
      </Button>
      <br/>
      <Collapse isOpen={chartsExpanded}>
        <Composition gap={0}>
          <AngleChart />
          <RotationRateChart />
          <LoadChart />
          <PowerChart />
        </Composition>
      </Collapse>
    </div>
  )
}
