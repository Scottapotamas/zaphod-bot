import React from 'react'
import {
  Button,
  Slider,
  Statistic,
  Statistics,
  NumberInput,
} from '@electricui/components-desktop-blueprint'
import { CALL_CALLBACK } from '@electricui/core'
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
import {
  IntervalRequester,
  StateTree,
  useHardwareState,
} from '@electricui/components-core'
import { Composition, Box } from 'atomic-layout'

import { Printer } from '@electricui/components-desktop'
import { RouteComponentProps } from '@reach/router'

import LEDCalibrationModal from '../../components/LEDCalibrationModal'
import KinematicsDiagram from '../../components/KinematicsDiagram'

const SensorsActive = () => {
  const sensorEnabledState =
    useHardwareState<number>(state => state.sys.sensors_enable) === 1
  if (sensorEnabledState) {
    return <div>ADC Enabled</div>
  }

  return <div>ADC Disabled</div>
}

const ModuleActive = () => {
  const ExpansionModuleState =
    useHardwareState<number>(state => state.sys.module_enable) === 1
  if (ExpansionModuleState) {
    return <div>Add-in Module On</div>
  }

  return <div>Add-in Module Off</div>
}

const KinematicsAreas = `
KinematicsLimitsArea KinematicsValuesArea
`

const KinematicsInfoCard = () => {
  const shoulder = useHardwareState(state => state.kinematics.shoulder_radius)
  const bicep = useHardwareState(state => state.kinematics.bicep_length)
  const forearm = useHardwareState(state => state.kinematics.forearm_length)
  const eff = useHardwareState(state => state.kinematics.effector_radius)

  return (
    <Card>
      <Composition
        areas={KinematicsAreas}
        gap={30}
        padding={20}
        templateCols="1fr auto"
      >
        {({ KinematicsLimitsArea, KinematicsValuesArea }) => (
          <React.Fragment>
            <KinematicsLimitsArea>
              <h2>Kinematics Overview</h2>
              <HTMLTable striped style={{ minWidth: '100%' }}>
                <thead>
                  <td></td>
                  <td>Min</td>
                  <td>Max</td>
                  <td>Inverted</td>
                </thead>
                <tbody>
                  <tr>
                    <td>Radius</td>
                    <td></td>
                    <td>
                      <Printer
                        accessor={state => state.kinematics.limit_radius}
                      />
                    </td>
                    <td></td>
                  </tr>
                  <tr>
                    <td>Z</td>
                    <td>
                      <Printer
                        accessor={state => state.kinematics.limit_z_min}
                      />
                    </td>
                    <td>
                      <Printer
                        accessor={state => state.kinematics.limit_z_max}
                      />
                    </td>
                    <td>
                      <Printer accessor={state => state.kinematics.flip_z} />
                    </td>
                  </tr>
                </tbody>
              </HTMLTable>
              <br />
              <h3>Adjust Z axis alignment</h3>
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
            </KinematicsLimitsArea>
            <KinematicsValuesArea>
              <KinematicsDiagram
                shoulderDistance={shoulder}
                bicepLength={bicep}
                forearmLength={forearm}
                effectorDistance={eff}
              />
            </KinematicsValuesArea>
          </React.Fragment>
        )}
      </Composition>
    </Card>
  )
}

const LEDCalibrationAreas = `
CalibrationArea OffsetDisplayArea
`

const LEDCalibrationCard = () => {
  return (
    <Card>
      <Composition areas={LEDCalibrationAreas} templateCols="auto auto">
        {({ CalibrationArea, OffsetDisplayArea }) => (
          <React.Fragment>
            <CalibrationArea>
              <h2>LED Calibration</h2>
              <LEDCalibrationModal />
            </CalibrationArea>

            <OffsetDisplayArea>
              <HTMLTable>
                <tbody>
                  <tr>
                    <td>
                      <b>Red Offset</b>
                    </td>
                    <td>
                      <Printer accessor={state => state.ledset.offset_red} />
                    </td>
                  </tr>
                  <tr>
                    <td>
                      <b>Green Offset</b>
                    </td>
                    <td>
                      <Printer accessor={state => state.ledset.offset_green} />
                    </td>
                  </tr>
                  <tr>
                    <td>
                      <b>Blue Offset</b>
                    </td>
                    <td>
                      <Printer accessor={state => state.ledset.offset_blue} />
                    </td>
                  </tr>
                  <tr>
                    <td>
                      <b>Brightness Override</b>
                    </td>
                    <td>
                      <Printer accessor={state => state.ledset.offset_global} />
                    </td>
                  </tr>
                </tbody>
              </HTMLTable>
            </OffsetDisplayArea>
          </React.Fragment>
        )}
      </Composition>
    </Card>
  )
}

const PowerCalibrationCard = () => {
  return (
    <Card>
      <Composition templateCols="1fr 1fr" gap={20}>
        <Box>
          <Statistic
            accessor={state => state.sys.input_voltage}
            label="Voltage"
          />
          <br />
          <NumberInput
            accessor={state => state.pwr_cal.voltage}
            writer={value => ({
              pwr_cal: {
                voltage: value,
              },
            })}
            style={{ maxWidth: '100px' }}
          />
        </Box>
        <Box>
          <Statistic accessor={state => state.mo1.power} label="Servo 1" />
          <br />
          <NumberInput
            accessor={state => state.pwr_cal.current_servo_1}
            writer={value => ({
              pwr_cal: {
                current_servo_1: value,
              },
            })}
            style={{ maxWidth: '100px' }}
          />
        </Box>
        <Box>
          <Statistic accessor={state => state.mo2.power} label="Servo 2" />
          <br />
          <NumberInput
            accessor={state => state.pwr_cal.current_servo_2}
            writer={value => ({
              pwr_cal: {
                current_servo_2: value,
              },
            })}
            style={{ maxWidth: '100px' }}
          />
        </Box>
        <Box>
          <Statistic accessor={state => state.mo3.power} label="Servo 3" />
          <br />
          <NumberInput
            accessor={state => state.pwr_cal.current_servo_3}
            writer={value => ({
              pwr_cal: {
                current_servo_3: value,
              },
            })}
            style={{ maxWidth: '100px' }}
          />
        </Box>
      </Composition>
      <Button
        writer={{
          save: CALL_CALLBACK,
        }}
      >
        Save Calibration Values
      </Button>
    </Card>
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
    <Card>
      <Composition areas={CoreInfoAreas}>
        {({ CPUArea, FirmwareArea }) => (
          <React.Fragment>
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
              <br />
              <Button writer={{ wipe: CALL_CALLBACK }}>
                Wipe stored settings
              </Button>
            </CPUArea>
            <FirmwareArea>
              <h3>Firmware Build Info</h3>
              <HTMLTable striped style={{ minWidth: '100%' }}>
                <tbody>
                  <tr>
                    <td>
                      <b>Version</b>
                    </td>
                    <td>
                      <Printer accessor={state => state.fwb.version} />
                    </td>
                  </tr>
                  <tr>
                    <td>
                      <b>Branch</b>
                    </td>
                    <td>
                      <Printer accessor={state => state.fwb.branch} />
                    </td>
                  </tr>
                  <tr>
                    <td>
                      <b>Info</b>
                    </td>
                    <td>
                      <Printer accessor={state => state.fwb.info} />
                    </td>
                  </tr>
                  <tr>
                    <td>
                      <b>Date</b>
                    </td>
                    <td>
                      <Printer accessor={state => state.fwb.date} />
                    </td>
                  </tr>
                  <tr>
                    <td>
                      <b>Time</b>
                    </td>
                    <td>
                      <Printer accessor={state => state.fwb.time} />
                    </td>
                  </tr>
                  <tr>
                    <td>
                      <b>Type</b>
                    </td>
                    <td>
                      <Printer accessor={state => state.fwb.type} />
                    </td>
                  </tr>
                </tbody>
              </HTMLTable>
            </FirmwareArea>
          </React.Fragment>
        )}
      </Composition>
    </Card>
  )
}

const SystemAreas = `
KinematicsArea CPUArea
LEDCalibrationArea PowerCalibrationArea
`

const SystemPage = (props: RouteComponentProps) => {
  return (
    <div>
      <IntervalRequester
        interval={100}
        variables={['sys', 'mo1', 'mo2', 'mo3']}
      />
      <IntervalRequester interval={2000} variables={['curve']} />
      <IntervalRequester interval={350} variables={['fan']} />

      <Composition areas={SystemAreas} gap={20} templateCols="1fr 1fr">
        {({
          KinematicsArea,
          LEDCalibrationArea,
          CPUArea,
          PowerCalibrationArea,
        }) => (
          <React.Fragment>
            <KinematicsArea>
              <KinematicsInfoCard />
            </KinematicsArea>
            <LEDCalibrationArea>
              <LEDCalibrationCard />
            </LEDCalibrationArea>
            <CPUArea>
              <CoreSystemsInfoCard />
            </CPUArea>
            <PowerCalibrationArea>
              <PowerCalibrationCard />
            </PowerCalibrationArea>
          </React.Fragment>
        )}
      </Composition>
    </div>
  )
}

export default SystemPage
