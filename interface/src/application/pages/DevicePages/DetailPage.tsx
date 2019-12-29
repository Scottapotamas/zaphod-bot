import React from 'react'
import { RouteComponentProps } from '@reach/router'
import { Composition } from 'atomic-layout'

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
    <Card>
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
            </TableArea>
          </React.Fragment>
        )}
      </Composition>
    </Card>
  )
}

const FanMode = () => {
  const fanstate = useHardwareState(state => state.fan.state)

  if (fanstate == 0) {
    return <div>Off</div>
  } else if (fanstate == 1) {
    return <div>Stall</div>
  } else if (fanstate == 2) {
    return <div>Startup</div>
  } else if (fanstate == 3) {
    return <div>OK</div>
  }

  return <div>null</div>
}

const CoolingAreas = `
TemperaturesArea
FanArea
`

const CoolingInfoCard = () => {
  const ambient_temp = useHardwareState(state => state.temp.ambient).toFixed(0)
  const regulator_temp = useHardwareState(
    state => state.temp.regulator,
  ).toFixed(0)
  const supply_temp = useHardwareState(state => state.temp.supply).toFixed(0)
  const fanspeed = useHardwareState(state => state.fan.rpm).toFixed(0)
  const fansetting = useHardwareState(state => state.fan.setpoint).toFixed(0)

  return (
    <Card>
      <Composition areas={CoolingAreas} gap={20}>
        {({ TemperaturesArea, FanArea }) => (
          <React.Fragment>
            <TemperaturesArea>
              <Statistics>
                <Statistic value={ambient_temp} label={`Ambient`} suffix="º" />
                <Statistic
                  value={regulator_temp}
                  label={`DC-DC Reg`}
                  suffix="º"
                />
                <Statistic value={supply_temp} label={`AC-DC PSU`} suffix="º" />
              </Statistics>
            </TemperaturesArea>
            <FanArea>
              <Statistics>
                <Statistic value={<FanMode />} label="operation" />
                <Statistic value={fanspeed} label={`RPM, at ${fansetting}%`} />
              </Statistics>
            </FanArea>
          </React.Fragment>
        )}
      </Composition>
      Fan Controls
      <Slider min={0} max={100} labelStepSize={25}>
        <Slider.Handle accessor="fan_man_speed" />
      </Slider>
      <Switch unchecked={{ fan_manual_en: 0 }} checked={{ fan_manual_en: 1 }}>
        Manual Fan Control
      </Switch>
    </Card>
  )
}

const DetailPageAreas = `
MotorStatsArea MotorGraphArea
CoolingStatsArea CoolingGraphArea
`

const DetailPage = (props: RouteComponentProps) => {
  return (
    <div>
      <IntervalRequester interval={500} variables={['temp, fan']} />
      <IntervalRequester interval={200} variables={['mo1', 'mo2', 'mo3']} />

      <Composition areas={DetailPageAreas} gap={10} templateCols="auto auto">
        {({
          MotorStatsArea,
          MotorGraphArea,
          CoolingStatsArea,
          CoolingGraphArea,
        }) => (
          <React.Fragment>
            <MotorStatsArea>
              <ServoSummaryCard />
            </MotorStatsArea>
            <MotorGraphArea>
              <Card>
                <Chart
                  timeseriesKey="motor_angles"
                  duration={25000}
                  hideLegend={false}
                  yMin={-60}
                  yMax={20}
                  delay={50}
                  height={250}
                />
              </Card>
            </MotorGraphArea>
            <CoolingStatsArea>
              <CoolingInfoCard />
            </CoolingStatsArea>
            <CoolingGraphArea>
              <Card>
                <Chart
                  timeseriesKey="temperatures"
                  duration={29000}
                  hideLegend={false}
                  yMin={15}
                  yMax={45}
                  delay={500}
                  height={250}
                />
              </Card>
            </CoolingGraphArea>
          </React.Fragment>
        )}
      </Composition>
    </div>
  )
}

export default DetailPage
