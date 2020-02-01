import {
  Alignment,
  Button as BlueprintButton,
  ButtonGroup,
  Navbar,
  Popover,
  Position,
  Tooltip,
  HTMLTable,
} from '@blueprintjs/core'
import {
  IntervalRequester,
  useDeviceConnect,
  useDeviceConnectionRequested,
  useDeviceDisconnect,
  useHardwareState,
} from '@electricui/components-core'

import React from 'react'
import { Composition, Box } from 'atomic-layout'
import {
  Statistic,
  Button,
  NumberInput,
  Slider,
} from '@electricui/components-desktop-blueprint'
import { Printer } from '@electricui/components-desktop'
import { CALL_CALLBACK } from '@electricui/core'

import LEDCalibrationModal from '../LEDCalibrationModal'

import KinematicsDiagram from '../KinematicsDiagram'

const KinematicsAreas = `
KinematicsLimitsArea KinematicsValuesArea
`

const KinematicsInfoCard = () => {
  const shoulder = useHardwareState(state => state.kinematics.shoulder_radius)
  const bicep = useHardwareState(state => state.kinematics.bicep_length)
  const forearm = useHardwareState(state => state.kinematics.forearm_length)
  const eff = useHardwareState(state => state.kinematics.effector_radius)

  return (
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
  )
}

const LEDCalibrationCard = () => {
  return (
    <Composition templateCols="1fr" justifyItems="center">
      <Box>
        <h2>LED Calibration</h2>
      </Box>
      <Box>
        <Composition templateCols="1fr 2fr" gap={30}>
          <Box>
            <h3>Whitebalance Check</h3>
            <ButtonGroup fill>
              <Button
                writer={{
                  hsv: {
                    hue: 0,
                    saturation: 0,
                    lightness: 0,
                    enable: 0,
                  },
                }}
              >
                Off
              </Button>

              <Button
                writer={{
                  hsv: {
                    hue: 0,
                    saturation: 0,
                    lightness: 0.1,
                    enable: 1,
                  },
                }}
              >
                10%
              </Button>
              <Button
                writer={{
                  hsv: {
                    hue: 0,
                    saturation: 0,
                    lightness: 0.5,
                    enable: 1,
                  },
                }}
              >
                50%
              </Button>
              <Button
                writer={{
                  hsv: {
                    hue: 0,
                    saturation: 0,
                    lightness: 1.0,
                    enable: 1,
                  },
                }}
              >
                100%
              </Button>
            </ButtonGroup>
            <br />
            <h3>Colour Check</h3>
            <ButtonGroup fill>
              <Button
                writer={{
                  hsv: {
                    hue: 0,
                    saturation: 0.0,
                    lightness: 0.0,
                    enable: 0,
                  },
                }}
              >
                Off
              </Button>

              <Button
                intent="danger"
                writer={{
                  hsv: {
                    hue: 0,
                    saturation: 1.0,
                    lightness: 0.5,
                    enable: 1,
                  },
                }}
              >
                Red
              </Button>
              <Button
                intent="success"
                writer={{
                  hsv: {
                    hue: 0.33,
                    saturation: 1,
                    lightness: 0.5,
                    enable: 1,
                  },
                }}
              >
                Green
              </Button>
              <Button
                intent="primary"
                writer={{
                  hsv: {
                    hue: 0.66,
                    saturation: 0,
                    lightness: 0.5,
                    enable: 1,
                  },
                }}
              >
                Blue
              </Button>
            </ButtonGroup>
          </Box>

          <Box>
            <HTMLTable striped style={{ minWidth: '100%' }}>
              <thead>
                <tr>
                  <th>Channel</th>
                  <th>Whitebalance Offset</th>
                </tr>
              </thead>
              <tbody>
                <tr>
                  <td>
                    <b>Red</b>
                  </td>
                  <td>
                    <Slider
                      showTrackFill={false}
                      writer={values => ({
                        ledset: {
                          offset_red: values.red_handle,
                        },
                      })}
                      min={-1}
                      max={1}
                      stepSize={0.02}
                      labelStepSize={0.5}
                      labelRenderer={val => `${Math.round(val * 100)}%`}
                    >
                      <Slider.Handle
                        name="red_handle"
                        accessor={state => state.ledset.offset_red}
                      />
                    </Slider>
                  </td>
                </tr>
                <tr>
                  <td>
                    <b>Green</b>
                  </td>
                  <td>
                    <Slider
                      showTrackFill={false}
                      writer={values => ({
                        ledset: {
                          offset_green: values.green_handle,
                        },
                      })}
                      min={-1}
                      max={1}
                      stepSize={0.02}
                      labelStepSize={0.5}
                      labelRenderer={val => `${Math.round(val * 100)}%`}
                    >
                      <Slider.Handle
                        name="green_handle"
                        accessor={state => state.ledset.offset_green}
                      />
                    </Slider>
                  </td>
                </tr>
                <tr>
                  <td>
                    <b>Blue</b>
                  </td>
                  <td>
                    <Slider
                      showTrackFill={false}
                      writer={values => ({
                        ledset: {
                          offset_blue: values.blue_handle,
                        },
                      })}
                      min={-1}
                      max={1}
                      stepSize={0.02}
                      labelStepSize={0.5}
                      labelRenderer={val => `${Math.round(val * 100)}%`}
                    >
                      <Slider.Handle
                        name="blue_handle"
                        accessor={state => state.ledset.offset_blue}
                      />
                    </Slider>
                  </td>
                </tr>
                <tr>
                  <td>
                    <b>Brightness</b>
                  </td>
                  <td>
                    <Slider
                      showTrackFill={false}
                      writer={values => ({
                        ledset: {
                          offset_global: values.luma_handle,
                        },
                      })}
                      min={-1}
                      max={1}
                      stepSize={0.02}
                      labelStepSize={0.5}
                      labelRenderer={val => `${Math.round(val * 100)}%`}
                    >
                      <Slider.Handle
                        name="luma_handle"
                        accessor={state => state.ledset.offset_global}
                      />
                    </Slider>
                  </td>
                </tr>
              </tbody>
            </HTMLTable>
          </Box>
        </Composition>
      </Box>
    </Composition>
  )
}

const PowerCalibrationCard = () => {
  return (
    <Composition templateCols="1fr" justifyItems="center">
      <Box>
        <h2>Power Calibration</h2>
      </Box>
      <Box>
        <IntervalRequester
          variables={['sys', 'mo1', 'mo2', 'mo3']}
          interval={100}
        />
        <HTMLTable striped style={{ minWidth: '100%' }}>
          <thead>
            <tr>
              <th>Sensor</th>
              <th>Current Value</th>
              <th>Calibration Offset</th>
            </tr>
          </thead>
          <tbody>
            <tr>
              <td>
                <b>Input Voltage</b>
              </td>
              <td>
                <Printer accessor={state => state.sys.input_voltage} />V
              </td>
              <td>
                <NumberInput
                  accessor={state => state.pwr_cal.voltage}
                  writer={value => ({
                    pwr_cal: {
                      voltage: value,
                    },
                  })}
                  style={{ maxWidth: '150px' }}
                />
              </td>
            </tr>
            <tr>
              <td>
                <b>Servo 1</b>
              </td>
              <td>
                <Printer accessor={state => state.mo1.power} />
              </td>
              <td>
                <NumberInput
                  accessor={state => state.pwr_cal.current_servo_1}
                  writer={value => ({
                    pwr_cal: {
                      current_servo_1: value,
                    },
                  })}
                  style={{ maxWidth: '150px' }}
                />
              </td>
            </tr>
            <tr>
              <td>
                <b>Servo 2</b>
              </td>
              <td>
                <Printer accessor={state => state.mo2.power} />
              </td>
              <td>
                <NumberInput
                  accessor={state => state.pwr_cal.current_servo_2}
                  writer={value => ({
                    pwr_cal: {
                      current_servo_2: value,
                    },
                  })}
                  style={{ maxWidth: '150px' }}
                />
              </td>
            </tr>
            <tr>
              <td>
                <b>Servo 3</b>
              </td>
              <td>
                <Printer accessor={state => state.mo3.power} />
              </td>
              <td>
                <NumberInput
                  accessor={state => state.pwr_cal.current_servo_3}
                  writer={value => ({
                    pwr_cal: {
                      current_servo_3: value,
                    },
                  })}
                  style={{ maxWidth: '150px' }}
                />
              </td>
            </tr>
            <tr>
              <td>
                <b>Servo 4</b>
              </td>
              <td>
                <Printer accessor={state => state.mo4.power} />
              </td>
              <td>
                <NumberInput
                  accessor={state => state.pwr_cal.voltage}
                  writer={value => ({
                    pwr_cal: {
                      voltage: value,
                    },
                  })}
                  style={{ maxWidth: '150px' }}
                />
              </td>
            </tr>
          </tbody>
        </HTMLTable>
      </Box>
    </Composition>
  )
}

export const SystemSettingsCard = () => {
  return (
    <>
      <Composition
        templateCols="1fr"
        padding="30px"
        justifyItems="center"
        gap={30}
      >
        <Box>
          <LEDCalibrationCard />
        </Box>
        <Box>
          <PowerCalibrationCard />
        </Box>
        <Box>
          <Button
            large
            fill
            intent="success"
            writer={{
              save: CALL_CALLBACK,
            }}
          >
            Store Calibration Values
          </Button>
        </Box>
      </Composition>
    </>
  )
}
