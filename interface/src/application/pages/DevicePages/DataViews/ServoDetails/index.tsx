import React from 'react'
import { RouteComponentProps } from '@reach/router'
import { Composition, Box, Only } from 'atomic-layout'

import {
  ChartContainer,
  LineChart,
  RealTimeDomain,
  TimeAxis,
  VerticalAxis,
} from '@electricui/components-desktop-charts'

import { Statistic, Statistics } from '@electricui/components-desktop-blueprint'
import { Colors, Callout, Tooltip, Position, Intent } from '@blueprintjs/core'
import { IconNames } from '@blueprintjs/icons'
import {
  IntervalRequester,
  useHardwareState,
} from '@electricui/components-core'

import {
  MessageDataSource,
  RollingStorageRequest,
} from '@electricui/core-timeseries'
import { useDarkMode } from '@electricui/components-desktop'
import { ServoTelemetry } from '../../../../../transport-manager/config/codecs'

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
  servo: ServoTelemetry
  index: number
}

const ServoStats = (props: MotorData) => {
  let operation_mode: string
  let status_icon: any
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
      <Tooltip
        content={operation_mode}
        intent={servo_colour}
        position={Position.LEFT}
      >
        <Callout intent={servo_colour} icon={status_icon}>
          <Composition
            templateCols="1fr 1fr 1fr"
            paddingHorizontal={20}
            gap={30}
            placeItems="center end"
            justifyContent="space-around"
            minWidth="280px"
          >
            <Box>
              <h4 className="bp3-heading">
                {props.servo.feedback.toFixed(1)}%
              </h4>
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
      </Tooltip>
    </React.Fragment>
  )
}

const ServoSummaryCard = () => {
  const motors: ServoTelemetry[] | null = useHardwareState(state => state.servo)
  if (motors === null) {
    return <span>No motor telemetry available...</span>
  }

  return (
    <Composition
      padding={10}
      gap={20}
      templateCols="1fr 2fr"
      placeItems="center end"
      justifyContent="space-around"
    >
      <Statistics>
        <Statistic>
          <Statistic.Label>Motors are</Statistic.Label>
          <Statistic.Value>
            <MotorSafetyMode />
          </Statistic.Value>
        </Statistic>
      </Statistics>

      <Composition gap={10}>
        {motors.map((clearpath, index) => (
          <Box>
            <ServoStats servo={clearpath} index={index} />
          </Box>
        ))}
      </Composition>
    </Composition>
  )
}

const lightModeColours = [
  Colors.GREEN1,
  Colors.RED1,
  Colors.BLUE1,
  Colors.GOLD1,
]
const darkModeColours = [Colors.GREEN5, Colors.RED5, Colors.BLUE5, Colors.GOLD5]

const servoTelemetryDataSource = new MessageDataSource('servo')

export const ServoDetails = () => {
  const numMotors: number | null = useHardwareState(
    state => (state.servo || []).length,
  )

  const isDark = useDarkMode()
  const servoColours = darkModeColours //isDark ? darkModeColours : lightModeColours

  return (
    <div>
      <IntervalRequester variables={['servo']} interval={50} />
      <RollingStorageRequest
        dataSource={servoTelemetryDataSource}
        maxItems={250}
        persist
      />
      <Composition>
        <ChartContainer height={300}>
          {Array.from(new Array(numMotors)).map((_, index) => (
            <LineChart
              dataSource={servoTelemetryDataSource}
              accessor={state => state.servo[index].target_angle}
              maxItems={10000}
              color={servoColours[index]}
              key={`angle_${index}`}
            />
          ))}
          <RealTimeDomain window={10000} yMin={-45} yMax={20} delay={50} />
          <TimeAxis />
          <VerticalAxis label="Arm Angle °" />
        </ChartContainer>
        <Only from={{ minHeight: 940 }}>
          <ChartContainer height={300}>
            {Array.from(new Array(numMotors)).map((_, index) => (
              <LineChart
                dataSource={servoTelemetryDataSource}
                accessor={state => state.servo[index].feedback}
                maxItems={10000}
                color={servoColours[index]}
                key={`torque_${index}`}
              />
            ))}
            <RealTimeDomain window={10000} yMin={-10} yMax={10} delay={50} />
            <TimeAxis />
            <VerticalAxis label="Servo Torque %" />
          </ChartContainer>
        </Only>
        <Only from={{ minHeight: 1240 }}>
          <ChartContainer height={300}>
            {Array.from(new Array(numMotors)).map((_, index) => (
              <LineChart
                dataSource={servoTelemetryDataSource}
                accessor={state => state.servo[index].power}
                maxItems={10000}
                color={servoColours[index]}
                key={`power_${index}`}
              />
            ))}
            <RealTimeDomain window={10000} yMin={0} yMax={50} delay={50} />
            <TimeAxis />
            <VerticalAxis label="Servo Power W" />
          </ChartContainer>
        </Only>
        <ServoSummaryCard />
      </Composition>
    </div>
  )
}
