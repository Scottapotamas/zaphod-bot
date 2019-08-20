import React from 'react'
import { RouteComponentProps } from '@reach/router'
import {
  IntervalRequester,
  useHardwareState,
  StateTree,
} from '@electricui/components-core'
import { Printer } from '@electricui/components-desktop'
import { Chart } from '@electricui/components-desktop-charts'
import { Card, Label, Text, Divider, HTMLTable, Icon } from '@blueprintjs/core'
import { Grid, Cell } from 'styled-css-grid'
import {
  Statistics,
  Statistic,
  Button,
} from '@electricui/components-desktop-blueprint'
import ServoData from '../../components/ServoData'

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

const DetailPage = (props: RouteComponentProps) => {
  const servoA = useHardwareState(state => state.mo1)
  const servoB = useHardwareState(state => state.mo2)
  const servoC = useHardwareState(state => state.mo3)

  return (
    <div>
      <IntervalRequester interval={500} variables={['temp']} />
      <IntervalRequester interval={200} variables={['mo1', 'mo2', 'mo3']} />

      <br />
      <Card>
        <Grid columns={'30% 70%'} gap="1em">
          <Cell center middle>
            <Statistic>
              <Statistic.Label>Motors are</Statistic.Label>
              <Statistic.Value>
                <MotorSafetyMode />
              </Statistic.Value>
            </Statistic>

            <br />
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
                    <FeedbackIndicator fb={servoA.feedback} />
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
                    <FeedbackIndicator fb={servoB.feedback} />
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
                    <FeedbackIndicator fb={servoC.feedback} />
                  </td>
                  <td>{servoC.power.toFixed(1)}</td>
                  <td>{servoC.target_angle.toFixed(2)}</td>
                </tr>
              </tbody>
            </HTMLTable>
          </Cell>
          <Cell>
            <Chart
              timeseriesKey="motor_angles"
              duration={30000}
              yMin={-42}
              yMax={50}
              delay={300}
              height={300}
            />
            <br />
            <Chart
              timeseriesKey="motor_power"
              duration={30000}
              delay={300}
              yMin={0}
              yMax={60}
              height={300}
            />
          </Cell>
        </Grid>
      </Card>
    </div>
  )
}

export default DetailPage
