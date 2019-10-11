import React from 'react'
import {
  Button,
  Slider,
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
import { Composition } from 'atomic-layout'

import { Printer } from '@electricui/components-desktop'
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

const KinematicsAreas = `
KA1 KA2
`

const KinematicsInfoCard = () => {
  return (
    <Composition areas={KinematicsAreas}>
      {({ KA1, KA2 }) => (
        <Card>
          <KA1>
            <HTMLTable striped style={{ minWidth: '100%' }}>
              <thead>
                <td>Axis</td>
                <td>Min</td>
                <td>Max</td>
                <td>Inverted</td>
              </thead>
              <tbody>
                <tr>
                  <td>X</td>
                  <td>
                    <Printer accessor={state => state.kinematics.limit_x_min} />
                  </td>
                  <td>
                    <Printer accessor={state => state.kinematics.limit_x_max} />
                  </td>
                  <td>
                    <Printer accessor={state => state.kinematics.flip_x} />
                  </td>
                </tr>
                <tr>
                  <td>Y</td>
                  <td>
                    <Printer accessor={state => state.kinematics.limit_y_min} />
                  </td>
                  <td>
                    <Printer accessor={state => state.kinematics.limit_y_max} />
                  </td>
                  <td>
                    <Printer accessor={state => state.kinematics.flip_y} />
                  </td>
                </tr>
                <tr>
                  <td>Z</td>
                  <td>
                    <Printer accessor={state => state.kinematics.limit_z_min} />
                  </td>
                  <td>
                    <Printer accessor={state => state.kinematics.limit_z_max} />
                  </td>
                  <td>
                    <Printer accessor={state => state.kinematics.flip_z} />
                  </td>
                </tr>
              </tbody>
            </HTMLTable>
          </KA1>
          <KA2>
            Motor shaft radius:{' '}
            <Printer accessor={state => state.kinematics.shoulder_radius} />
            <br />
            Bicep lenth:{' '}
            <Printer accessor={state => state.kinematics.bicep_length} />
            <br />
            Forearm lenth:{' '}
            <Printer accessor={state => state.kinematics.forearm_length} />
            <br />
            Effector Radius:{' '}
            <Printer accessor={state => state.kinematics.effector_radius} />
            <br />
            <br />
            Rotation around Z axis
            <br />
            <Slider
              min={0}
              max={360}
              labelStepSize={45}
              stepSize={15}
              sendOnlyOnRelease
              labelRenderer={val => `${val}º`}
            >
              <Slider.Handle accessor="rotZ" />
            </Slider>
          </KA2>
        </Card>
      )}
    </Composition>
  )
}

const LEDCalibrationAreas = `
AArea BArea
`

const LEDCalibrationCard = () => {
  return (
    <Composition areas={LEDCalibrationAreas}>
      {({ AArea, BArea }) => (
        <Card>
          <AArea>
            Red offset <Printer accessor={state => state.ledset.offset_red} />
            <br />
            Green offset{' '}
            <Printer accessor={state => state.ledset.offset_green} />
            <br />
            Blue offset <Printer accessor={state => state.ledset.offset_blue} />
            <br />
            Brightness override{' '}
            <Printer accessor={state => state.ledset.offset_global} />
            <br />
          </AArea>
          <BArea>Preview? </BArea>
        </Card>
      )}
    </Composition>
  )
}

const CoreInfoAreas = `
CPUArea FirmwareArea
`

const CoreSystemsInfoCard = () => {
  const cpu_percentage = useHardwareState(state => state.sys.cpu_load)
  const cpu_clock = useHardwareState(state => state.sys.cpu_clock)
  const cpu_temp = useHardwareState(state => state.sys.cpu_temp).toFixed(0)

  return (
    <Composition areas={CoreInfoAreas}>
      {({ CPUArea, FirmwareArea }) => (
        <Card>
          <CPUArea>
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
            <SensorsActive />
            <ModuleActive />
          </CPUArea>
          <FirmwareArea>
            <h3>Firmware Info</h3>
            <HTMLTable striped style={{ minWidth: '100%' }}>
              <tbody>
                <tr>
                  <td>Version</td>
                  <td>
                    <Printer accessor={state => state.fwb.version} />
                  </td>
                </tr>
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
          </FirmwareArea>
        </Card>
      )}
    </Composition>
  )
}

const CoolingAreas = `
TemperaturesArea FanArea
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
    <Composition areas={CoolingAreas}>
      {({ TemperaturesArea, FanArea }) => (
        <Card>
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
        </Card>
      )}
    </Composition>
  )
}

const SystemAreas = `
KinematicsArea CPUArea
ExpansionArea FanArea
`

const SystemPage = (props: RouteComponentProps) => {
  return (
    <div>
      <IntervalRequester interval={250} variables={['sys']} />
      <IntervalRequester interval={2000} variables={['curve']} />
      <IntervalRequester interval={350} variables={['fan']} />

      <Composition areas={SystemAreas} gap={20} templateCols="1fr 1fr">
        {({ KinematicsArea, ExpansionArea, CPUArea, FanArea }) => (
          <React.Fragment>
            <KinematicsArea>
              <KinematicsInfoCard />
            </KinematicsArea>
            <ExpansionArea>IO stuff</ExpansionArea>
            <CPUArea>
              <CoreSystemsInfoCard />
            </CPUArea>
            <FanArea>
              <CoolingInfoCard />
            </FanArea>
          </React.Fragment>
        )}
      </Composition>
    </div>
  )
}

export default SystemPage
