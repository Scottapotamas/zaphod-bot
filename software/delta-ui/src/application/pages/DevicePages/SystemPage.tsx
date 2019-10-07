import {
  Button,
  Statistic,
  Statistics,
} from '@electricui/components-desktop-blueprint'
import {
  ButtonGroup,
  Card,
  Divider,
  HTMLTable,
  Label,
  Menu,
  MenuItem,
  Text,
} from '@blueprintjs/core'
import { Cell, Grid } from 'styled-css-grid'
import {
  IntervalRequester,
  StateTree,
  useHardwareState,
} from '@electricui/components-core'

import { Printer } from '@electricui/components-desktop'
import React from 'react'
import { RouteComponentProps } from '@reach/router'

const SensorsActive = () => {
  const isBlinking =
    useHardwareState<number>(state => state.sys.sensors_enable) === 1
  if (isBlinking) {
    return <div>ADC Enabled</div>
  }

  return <div>ADC Disabled</div>
}

const ModuleActive = () => {
  const isBlinking =
    useHardwareState<number>(state => state.sys.module_enable) === 1
  if (isBlinking) {
    return <div>Add-in Module On</div>
  }

  return <div>Add-in Module Off</div>
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

const SystemPage = (props: RouteComponentProps) => {
  const cpu_percentage = useHardwareState(state => state.sys.cpu_load)
  const cpu_clock = useHardwareState(state => state.sys.cpu_clock)
  const cpu_temp = useHardwareState(state => state.sys.cpu_temp).toFixed(0)

  const ambient_temp = useHardwareState(state => state.temp.ambient).toFixed(0)
  const regulator_temp = useHardwareState(
    state => state.temp.regulator,
  ).toFixed(0)
  const supply_temp = useHardwareState(state => state.temp.supply).toFixed(0)
  const fanspeed = useHardwareState(state => state.fan.rpm).toFixed(0)
  const fansetting = useHardwareState(state => state.fan.setpoint).toFixed(0)

  return (
    <div>
      <IntervalRequester interval={250} variables={['sys']} />
      <IntervalRequester interval={2000} variables={['curve']} />
      <IntervalRequester interval={350} variables={['fan']} />

      <Grid
        columns={'1fr 3fr 2fr 1fr'}
        rows={2}
        areas={[
          'left-pad main status right-pad',
          'left-pad curve temps right-pad',
        ]}
        gap={'2em'}
      >
        <Cell area="main" center>
          <Card>
            <h2>IO Configuration</h2>
            <Menu>
              <MenuItem text="Submenu">
                <MenuItem text="Child one" />
                <MenuItem text="Child two" />
                <MenuItem text="Child three" />
              </MenuItem>
            </Menu>{' '}
            <div>
              Internal IO
              <ButtonGroup>
                <Button>Off</Button>
                <Button>RGB LED</Button>
                <Button>Placeholder</Button>
              </ButtonGroup>
            </div>
            <div>
              Internal Serial
              <ButtonGroup>
                <Button>Inactive</Button>
                <Button>UART</Button>
                <Button>I2C</Button>
              </ButtonGroup>
            </div>
            <div>
              External Group A
              <ButtonGroup>
                <Button>Inactive</Button>
                <Button>In/Out</Button>
                <Button>UART</Button>
              </ButtonGroup>
            </div>
            <div>
              External Group B
              <ButtonGroup>
                <Button>Inactive</Button>
                <Button>In/Out</Button>
                <Button>CANBus</Button>
              </ButtonGroup>
            </div>
          </Card>
        </Cell>
        <Cell area="status" center>
          <Card>
            <h2>System Info</h2>
            <Statistics>
              <Statistic
                value={cpu_percentage}
                label={`load at ${cpu_clock}MHz`}
                suffix="%"
              />
              <Statistic value={cpu_temp} label="CPU Temp" suffix="º" />
            </Statistics>
            <br />
            <br />
            <Grid columns={2}>
              <Cell>
                <SensorsActive />
              </Cell>
              <Cell>
                <ModuleActive />
              </Cell>
            </Grid>
            <br />
            <Divider vertical />
            <h3>Firmware Info</h3>
            <HTMLTable striped style={{ minWidth: '100%' }}>
              <thead>
                <td>Version</td>
                <td>
                  <Printer accessor={state => state.fwb.version} />
                </td>
              </thead>
              <tbody>
                <tr>
                  <td>Branch</td>
                  <td>
                    <Printer accessor={state => state.fwb.branch} />
                  </td>
                </tr>
                <tr>
                  <td>Info</td>
                  <td>
                    <Printer accessor={state => state.fwb.info} />
                  </td>
                </tr>
                <tr>
                  <td>Date</td>
                  <td>
                    <Printer accessor={state => state.fwb.date} />
                  </td>
                </tr>
                <tr>
                  <td>Time</td>
                  <td>
                    <Printer accessor={state => state.fwb.time} />
                  </td>
                </tr>
                <tr>
                  <td>Type</td>
                  <td>
                    <Printer accessor={state => state.fwb.type} />
                  </td>
                </tr>
              </tbody>
            </HTMLTable>
          </Card>
        </Cell>
        <Cell area="curve">
          <Card>
            <h2>Custom Fan Curve</h2>
            <br />
            <br />
            <br />
            <br />
            <br />
            <br />
            <br />
            <br />
          </Card>
        </Cell>
        <Cell area="temps">
          <Card>
            <Statistics>
              <Statistic value={ambient_temp} label={`Ambient`} suffix="º" />
              <Statistic
                value={regulator_temp}
                label={`DC-DC Reg`}
                suffix="º"
              />
              <Statistic value={supply_temp} label={`AC-DC PSU`} suffix="º" />
            </Statistics>
            <br />
            <h2>Fan Status</h2>
            <Statistics>
              <Statistic value={<FanMode />} label="operation" />
              <Statistic value={fanspeed} label={`RPM, at ${fansetting}%`} />
            </Statistics>
          </Card>
        </Cell>
      </Grid>
    </div>
  )
}

export default SystemPage
