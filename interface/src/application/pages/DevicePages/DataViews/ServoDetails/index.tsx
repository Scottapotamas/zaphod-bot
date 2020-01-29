import React from 'react'
import { RouteComponentProps } from '@reach/router'
import { Composition, Box } from 'atomic-layout'

import {
  Button,
  Statistic,
  Statistics,
  Slider,
  Switch,
} from '@electricui/components-desktop-blueprint'
import {
  Card,
  Divider,
  HTMLTable,
  Icon,
  Label,
  Text,
  Tab,
} from '@blueprintjs/core'
import {
  IntervalRequester,
  StateTree,
  useHardwareState,
} from '@electricui/components-core'

import { Chart } from '@electricui/components-desktop-charts'
import { Printer } from '@electricui/components-desktop'

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
  const servoA = useHardwareState(state => state.mo1)
  const servoB = useHardwareState(state => state.mo2)
  const servoC = useHardwareState(state => state.mo3)

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
                <tr>
                  <td>
                    <EnabledIndicator en={servoA.enabled} />
                  </td>
                  <td>
                    <ServoMode state={servoA.state} />
                  </td>
                  <td>
                    {servoA.feedback}%
                    {/* <FeedbackIndicator fb={servoA.feedback} /> */}
                  </td>
                  <td>{servoA.power.toFixed(1)}</td>
                  <td>{servoA.target_angle.toFixed(2)}</td>
                </tr>
                <tr>
                  <td>
                    <EnabledIndicator en={servoB.enabled} />
                  </td>
                  <td>
                    <ServoMode state={servoB.state} />
                  </td>
                  <td>
                    {servoB.feedback}%
                    {/* <FeedbackIndicator fb={servoB.feedback} /> */}
                  </td>
                  <td>{servoB.power.toFixed(1)}</td>
                  <td>{servoB.target_angle.toFixed(2)}</td>
                </tr>
                <tr>
                  <td>
                    <EnabledIndicator en={servoC.enabled} />
                  </td>
                  <td>
                    <ServoMode state={servoC.state} />
                  </td>
                  <td>
                    {servoC.feedback}%
                    {/* <FeedbackIndicator fb={servoC.feedback} /> */}
                  </td>
                  <td>{servoC.power.toFixed(1)}</td>
                  <td>{servoC.target_angle.toFixed(2)}</td>
                </tr>
              </tbody>
            </HTMLTable>
          </TableArea>
        </React.Fragment>
      )}
    </Composition>
  )
}

export const ServoDetails = () => {
  return (
    <div>
      <Chart
        timeseriesKey="motor_angles"
        duration={25000}
        hideLegend={false}
        yMin={-60}
        yMax={20}
        delay={50}
        height={250}
      />
      <ServoSummaryCard />
    </div>
  )
}
