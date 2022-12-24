import {
  Alignment,
  Button as BlueprintButton,
  ButtonGroup,
  Navbar,
  Popover,
  Position,
  Tooltip,
  HTMLTable,
  Intent,
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
  Switch,
  Checkbox,
} from '@electricui/components-desktop-blueprint'
import { Printer } from '@electricui/components-desktop'
import { MSGID, ServoInfo } from 'src/application/typedState'

export const LEDCalibrationBlock = () => {
  return (
    <Composition templateCols="1fr" justifyItems="center">
      <Box>
        <h2>LED Calibration</h2>
      </Box>
      <Box>
        <Composition templateCols="1fr 2fr" gap={30}>
          <Box>
            <Checkbox
              checked={true}
              unchecked={false}
              accessor={state => state[MSGID.LED_CALIBRATION].correct_luma}
              writer={(state, value) => {
                state[MSGID.LED_CALIBRATION].correct_luma = value
              }}
            >
              Luminance Curve Correction
            </Checkbox>
            <Checkbox
              checked={true}
              unchecked={false}
              accessor={state =>
                state[MSGID.LED_CALIBRATION].correct_whitebalance
              }
              writer={(state, value) => {
                state[MSGID.LED_CALIBRATION].correct_whitebalance = value
              }}
            >
              Whitebalance Correction
            </Checkbox>
          </Box>
          <Box>
            <h3>Whitebalance Check</h3>
            <ButtonGroup fill>
              <Button
                writer={state => {
                  state[MSGID.LED_MANUAL_REQUEST] = {
                    red: 0,
                    green: 0,
                    blue: 0,
                    enable: true,
                  }
                }}
              >
                Off
              </Button>

              <Button
                writer={state => {
                  state[MSGID.LED_MANUAL_REQUEST] = {
                    red: 0.1,
                    green: 0.1,
                    blue: 0.1,
                    enable: true,
                  }
                }}
              >
                10%
              </Button>
              <Button
                writer={state => {
                  state[MSGID.LED_MANUAL_REQUEST] = {
                    red: 0.5,
                    green: 0.5,
                    blue: 0.5,
                    enable: true,
                  }
                }}
              >
                50%
              </Button>
              <Button
                writer={state => {
                  state[MSGID.LED_MANUAL_REQUEST] = {
                    red: 1.0,
                    green: 1.0,
                    blue: 1.0,
                    enable: true,
                  }
                }}
              >
                100%
              </Button>
            </ButtonGroup>
            <br />
            <h3>Colour Check</h3>
            <ButtonGroup fill>
              <Button
                writer={state => {
                  state[MSGID.LED_MANUAL_REQUEST] = {
                    red: 0,
                    green: 0,
                    blue: 0,
                    enable: true,
                  }
                }}
              >
                Off
              </Button>

              <Button
                intent={Intent.DANGER}
                writer={state => {
                  state[MSGID.LED_MANUAL_REQUEST] = {
                    red: 0.5,
                    green: 0,
                    blue: 0,
                    enable: true,
                  }
                }}
              >
                Red
              </Button>
              <Button
                intent={Intent.SUCCESS}
                writer={state => {
                  state[MSGID.LED_MANUAL_REQUEST] = {
                    red: 0,
                    green: 0.5,
                    blue: 0,
                    enable: true,
                  }
                }}
              >
                Green
              </Button>
              <Button
                intent={Intent.PRIMARY}
                writer={state => {
                  state[MSGID.LED_MANUAL_REQUEST] = {
                    red: 0,
                    green: 0,
                    blue: 0.5,
                    enable: true,
                  }
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
                      writer={(state, values) => {
                        state[MSGID.LED_CALIBRATION].offset_red =
                          values.red_handle
                      }}
                      min={0}
                      max={1}
                      stepSize={0.01}
                      labelStepSize={0.5}
                      labelRenderer={val => `${Math.round(val * -100)}%`}
                    >
                      <Slider.Handle
                        name="red_handle"
                        accessor={state =>
                          state[MSGID.LED_CALIBRATION].offset_red
                        }
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
                      writer={(state, values) => {
                        state[MSGID.LED_CALIBRATION].offset_green =
                          values.green_handle
                      }}
                      min={0}
                      max={1}
                      stepSize={0.01}
                      labelStepSize={0.5}
                      labelRenderer={val => `${Math.round(val * -100)}%`}
                    >
                      <Slider.Handle
                        name="green_handle"
                        accessor={state =>
                          state[MSGID.LED_CALIBRATION].offset_green
                        }
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
                      writer={(state, values) => {
                        state[MSGID.LED_CALIBRATION].offset_blue =
                          values.blue_handle
                      }}
                      min={0}
                      max={1}
                      stepSize={0.01}
                      labelStepSize={0.5}
                      labelRenderer={val => `${Math.round(val * -100)}%`}
                    >
                      <Slider.Handle
                        name="blue_handle"
                        accessor={state =>
                          state[MSGID.LED_CALIBRATION].offset_blue
                        }
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
                      writer={(state, values) => {
                        state[MSGID.LED_CALIBRATION].offset_global =
                          values.luma_handle
                      }}
                      min={0}
                      max={1}
                      stepSize={0.01}
                      labelStepSize={0.5}
                      labelRenderer={val => `${Math.round(val * -100)}%`}
                    >
                      <Slider.Handle
                        name="luma_handle"
                        accessor={state =>
                          state[MSGID.LED_CALIBRATION].offset_global
                        }
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
