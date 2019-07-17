import React from 'react'
import { RouteComponentProps } from '@reach/router'
import {
  IntervalRequester,
  useHardwareState,
  StateTree,
} from '@electricui/components-core'
import { Printer } from '@electricui/components-desktop'
import { Card, Label, Text, Divider } from '@blueprintjs/core'
import { Grid, Cell } from 'styled-css-grid'
import {
  Statistics,
  Statistic,
  Button,
} from '@electricui/components-desktop-blueprint'
import ServoData from '../../components/ServoData'

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

const DetailPage = (props: RouteComponentProps) => {
  const ambient_temp = useHardwareState(state => state.temp.ambient).toFixed(0)
  const regulator_temp = useHardwareState(
    state => state.temp.regulator,
  ).toFixed(0)
  const supply_temp = useHardwareState(state => state.temp.supply).toFixed(0)
  const fanspeed = useHardwareState(state => state.fan.rpm).toFixed(0)
  const fansetting = useHardwareState(state => state.fan.setpoint).toFixed(0)

  const servoA = useHardwareState(state => state.mo1)
  const servoB = useHardwareState(state => state.mo2)
  const servoC = useHardwareState(state => state.mo3)

  return (
    <div>
      <IntervalRequester interval={350} variables={['fan']} />
      <IntervalRequester interval={500} variables={['temp']} />
      <IntervalRequester interval={200} variables={['mo1', 'mo2', 'mo3']} />

      <Card style={{ maxWidth: '80%', marginLeft: '10%' }}>
        <Grid columns={2}>
          <Cell center>
            <h2>Thermals</h2>
            <Statistics>
              <Statistic value={ambient_temp} label={`Ambient`} suffix="º" />
              <Statistic
                value={regulator_temp}
                label={`DC-DC Reg`}
                suffix="º"
              />
              <Statistic value={supply_temp} label={`AC-DC PSU`} suffix="º" />
            </Statistics>
          </Cell>
          <Cell center>
            <h2>Fan Status</h2>
            <Statistics>
              <Statistic value={<FanMode />} label="operation" />
              <Statistic value={fanspeed} label={`RPM, at ${fansetting}%`} />
            </Statistics>
          </Cell>
        </Grid>
        <br />
      </Card>
      <br />
      <Grid columns={3} gap="1em">
        <Cell>
          <Card>
            <ServoData name="A" servo_object={servoA} />
          </Card>
        </Cell>
        <Cell>
          <Card>
            <ServoData name="B" servo_object={servoB} />
          </Card>
        </Cell>
        <Cell>
          <Card>
            <ServoData name="C" servo_object={servoC} />
          </Card>
        </Cell>
      </Grid>
    </div>
  )
}

export default DetailPage
