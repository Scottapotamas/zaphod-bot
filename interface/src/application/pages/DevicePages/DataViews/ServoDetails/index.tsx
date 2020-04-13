import React from 'react'
import { RouteComponentProps } from '@reach/router'
import { Composition, Box } from 'atomic-layout'

import {
  ChartContainer,
  LineChart,
  RealTimeDomain,
  TimeAxis,
  VerticalAxis,
} from '@electricui/components-desktop-charts'

import {
  Button,
  Statistic,
  Statistics,
  Slider,
} from '@electricui/components-desktop-blueprint'
import {
  Card,
  Divider,
  HTMLTable,
  Icon,
  Label,
  Text,
  Tab,
  Colors,
} from '@blueprintjs/core'
import {
  IntervalRequester,
  StateTree,
  useHardwareState,
} from '@electricui/components-core'

import {
  MessageDataSource,
  RollingStorageRequest,
} from '@electricui/core-timeseries'
import { Printer, useDarkMode } from '@electricui/components-desktop'
import { ServoTelemetry } from '../../../../../transport-manager/config/codecs'

type EnabledDataProps = {
  en: boolean
}

type FeedbackDataProps = {
  fb: number
}

type ServoModeDataProps = {
  state: number
  // SERVO_STATE_INACTIVE,
  // SERVO_STATE_ERROR_RECOVERY,
  // SERVO_STATE_HOMING,
  // SERVO_STATE_IDLE,
  // SERVO_STATE_IDLE_HIGH_LOAD,
  // SERVO_STATE_ACTIVE,
}

const EnabledIndicator = (props: EnabledDataProps) => {
  return (
    <Icon
      icon={props.en ? 'symbol-circle' : 'symbol-square'}
      intent={props.en ? 'success' : 'none'}
    />
  )
}

const FeedbackIndicator = (props: FeedbackDataProps) => {
  return (
    <Icon
      icon={props.fb == 1 ? 'thumbs-up' : 'thumbs-down'}
      intent={props.fb == 1 ? 'success' : 'warning'}
    />
  )
}

const ServoMode = (props: ServoModeDataProps) => {
  if (props.state == 0) {
    return <div>Inactive</div>
  } else if (props.state == 1) {
    return <div>Recovery</div>
  } else if (props.state == 2) {
    return <div>Homing</div>
  } else if (props.state == 3) {
    return <div>Static</div>
  } else if (props.state == 4) {
    return <div>High Load</div>
  } else if (props.state == 5) {
    return <div>Motion</div>
  }

  return <div>null</div>
}

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

const ServoSummaryAreas = `
  a TableArea
  TextArea TableArea
  b TableArea
  `

const ServoSummaryCard = () => {
  const motors: ServoTelemetry[] | null = useHardwareState(state => state.servo)
  if (motors === null) {
    return <span>No motor telemetry available...</span>
  }

  return (
    <Composition
      areas={ServoSummaryAreas}
      padding={10}
      gap={20}
      templateCols="auto auto"
    >
      {({ TextArea, TableArea }) => (
        <React.Fragment>
          <TextArea>
            <Statistic>
              <Statistic.Label>Motors are</Statistic.Label>
              <Statistic.Value>
                <MotorSafetyMode />
              </Statistic.Value>
            </Statistic>
          </TextArea>
          <TableArea>
            <HTMLTable striped>
              <thead>
                <tr>
                  <th>Enabled</th>
                  <th>State</th>
                  <th>Feedback</th>
                  <th>Power (W)</th>
                  <th>Target º</th>
                </tr>
              </thead>
              <tbody>
                {motors.map((clearpath, index) => (
                  <tr>
                    <td>
                      <EnabledIndicator en={clearpath.enabled} />
                    </td>
                    <td>
                      <ServoMode state={clearpath.state} />
                    </td>
                    <td>
                      {clearpath.feedback}%
                      {/* <FeedbackIndicator fb={servoA.feedback} /> */}
                    </td>
                    <td>{clearpath.power.toFixed(1)}</td>
                    <td>{clearpath.target_angle.toFixed(2)}</td>
                  </tr>
                ))}
              </tbody>
            </HTMLTable>
          </TableArea>
        </React.Fragment>
      )}
    </Composition>
  )
}

const lightModeColours = [
  Colors.RED1,
  Colors.GREEN1,
  Colors.BLUE1,
  Colors.GOLD1,
]
const darkModeColours = [Colors.RED5, Colors.GREEN5, Colors.BLUE5, Colors.GOLD5]

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

      <ChartContainer>
        {Array.from(new Array(numMotors)).map((_, index) => (
          <LineChart
            dataSource={servoTelemetryDataSource}
            accessor={state => state.servo[index].target_angle}
            maxItems={10000}
            color={servoColours[index]}
          />
        ))}
        <RealTimeDomain window={10000} yMin={-45} yMax={20} delay={100} />
        <TimeAxis />
        <VerticalAxis />
      </ChartContainer>
      <ChartContainer>
        {Array.from(new Array(numMotors)).map((_, index) => (
          <LineChart
            dataSource={servoTelemetryDataSource}
            accessor={state => state.servo[index].feedback}
            maxItems={10000}
            color={servoColours[index]}
          />
        ))}
        <RealTimeDomain window={10000} yMin={-100} yMax={100} delay={100} />
        <TimeAxis />
        <VerticalAxis />
      </ChartContainer>
      <ChartContainer>
        {Array.from(new Array(numMotors)).map((_, index) => (
          <LineChart
            dataSource={servoTelemetryDataSource}
            accessor={state => state.servo[index].power}
            maxItems={10000}
            color={servoColours[index]}
          />
        ))}
        <RealTimeDomain window={10000} yMin={0} yMax={50} delay={100} />
        <TimeAxis />
        <VerticalAxis />
      </ChartContainer>
      <ServoSummaryCard />
    </div>
  )
}
