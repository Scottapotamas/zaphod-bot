import React, { useCallback, useState } from 'react'

import { Composition, Box, Only } from 'atomic-layout'

import {
  Button,
  Colors,
  Collapse,
  Tag,
  Intent,
  IconName,
} from '@blueprintjs/core'
import { IconNames } from '@blueprintjs/icons'

import { useHardwareState } from '@electricui/components-core'

import { MSGID, ServoInfo } from '../../../../typedState'
import { Printer } from '@electricui/components-desktop'

import { OutlineCard } from '../../../../components/OutlineCard'
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
      status_icon = IconNames.SMALL_MINUS
      break
    case 1:
      operation_mode = 'DISABLING'
      status_icon = IconNames.SMALL_CROSS
      break
    case 2:
      operation_mode = 'CHECK FEEDBACK'
      status_icon = IconNames.SMALL_PLUS
      break
    case 3:
      operation_mode = 'ENDSTOP SEARCH'
      status_icon = IconNames.SMALL_SQUARE
      break
    case 4:
      operation_mode = 'ENDSTOP FOUND'
      status_icon = IconNames.SMALL_TICK
      break
    case 5:
      operation_mode = 'FOLDBACK CHECK'
      status_icon = IconNames.SMALL_SQUARE
      break
    case 6:
      operation_mode = 'HOMING SUCCESS'
      status_icon = IconNames.SMALL_TICK
      break
    case 7:
      operation_mode = 'ACTIVE'
      status_icon = IconNames.DOT
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
      icon={status_icon}
      // intent={Intent.PRIMARY}
      style={{
        fontWeight: 'bold',
        // fontSize: 'larger',
        textAlign: 'center',
        minWidth: '150px',
        maxWidth: '200px',
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
        minWidth: '60px',
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
        minWidth: '60px',
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
        minWidth: '60px',
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
        minWidth: '60px',
        textAlign: 'center',
      }}
    >
      <Printer accessor={state => state[MSGID.SERVO][props.motorIndex].speed} precision={0}/> °/s
    </Tag>
  )
}
const oldlayoutDescription = `
  State Angle RotationRate
  State Feedback Power
`
const layoutDescription = `
  State Angle RotationRate Feedback Power
`

const ServoStats = (props: MotorData) => {
  const expansion_enabled = useHardwareState(state => state[MSGID.USER_CONFIG]?.flags?.expansion_enabled)

  if( props.index == 3 && !expansion_enabled )
  {
    return (<></>)
  }

  return (
    <OutlineCard>
      <Composition
        areas={layoutDescription}
        gap={5}
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
    </OutlineCard>
  )
}

const ServoStatsBlock = () => {
  const motors = useHardwareState(state => state[MSGID.SERVO])

  if (!motors) {
    return <span>No motor telemetry available...</span>
  }

  return (
    <Composition gap={5} >
      {motors.map((clearpath, index) => 
        <ServoStats servo={clearpath} index={index} key={index}/>
      )}
    </Composition>
  )
}

export const ServoSummary = () => {
  let [chartsExpanded, setChartsExpanded] = useState(false)

  const controlChartsExpansion = useCallback(
    _ => {
      setChartsExpanded(enabled => !enabled)
    },
    [setChartsExpanded],
  )

  return (
    <Composition gap={5}>
      <ServoStatsBlock />

      <Button
        minimal
        outlined
        intent={Intent.NONE}
        small
        fill
        onClick={controlChartsExpansion}
        icon={chartsExpanded ? IconNames.CHEVRON_UP : IconNames.CHEVRON_DOWN}
      >
        {/* Consider a label here to explain the UX? */}
        CHARTS
      </Button>

      <Collapse isOpen={chartsExpanded} transitionDuration={1500}>
        <Composition gap={5}>
          <OutlineCard>
            <Composition padding={5}>
              <AngleChart />
              <RotationRateChart />
            </Composition>
          </OutlineCard>
          <Only from={{ minHeight: 980 }}>
            <OutlineCard>
              <Composition padding={5}>
                <LoadChart />
                <PowerChart />
              </Composition>
            </OutlineCard>
          </Only>
        </Composition>
      </Collapse>
    </Composition>
  )
}
