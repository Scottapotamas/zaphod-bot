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
  Tag,
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

export const PowerCalibrationBlock = () => {
  const expansion_servo = useHardwareState(state => !!state[MSGID.SERVO][3])

  return (
    <Composition justifyItems="center">
      <Box>
        <h2>Power Calibration</h2>
      </Box>
      <Box>
        <IntervalRequester
          variables={[MSGID.SYSTEM, MSGID.SERVO]}
          interval={50}
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
                <Printer
                  accessor={state => state[MSGID.SYSTEM].input_voltage}
                />{' '}
                V
              </td>
              <td>
                <NumberInput
                  accessor={state => state[MSGID.POWER_CALIBRATION].voltage}
                  writer={(state, value) => {
                    state[MSGID.POWER_CALIBRATION].voltage = value
                  }}
                  rightElement={<Tag minimal>mV</Tag>}
                  // style={{ maxWidth: '150px' }}
                />
              </td>
            </tr>
            <tr>
              <td>
                <b>Servo 1</b>
              </td>
              <td>
                <Printer accessor={state => state[MSGID.SERVO][0].power} /> W
              </td>
              <td>
                <NumberInput
                  accessor={state =>
                    state[MSGID.POWER_CALIBRATION].current_servo_1
                  }
                  writer={(state, value) => {
                    state[MSGID.POWER_CALIBRATION].current_servo_1 = value
                  }}
                  rightElement={<Tag minimal>mA</Tag>}
                  // style={{ maxWidth: '150px' }}
                />
              </td>
            </tr>
            <tr>
              <td>
                <b>Servo 2</b>
              </td>
              <td>
                <Printer accessor={state => state[MSGID.SERVO][1].power} /> W
              </td>
              <td>
                <NumberInput
                  accessor={state =>
                    state[MSGID.POWER_CALIBRATION].current_servo_2
                  }
                  writer={(state, value) => {
                    state[MSGID.POWER_CALIBRATION].current_servo_2 = value
                  }}
                  rightElement={<Tag minimal>mA</Tag>}
                  // style={{ maxWidth: '150px' }}
                />
              </td>
            </tr>
            <tr>
              <td>
                <b>Servo 3</b>
              </td>
              <td>
                <Printer accessor={state => state[MSGID.SERVO][2].power} /> W
              </td>
              <td>
                <NumberInput
                  accessor={state =>
                    state[MSGID.POWER_CALIBRATION].current_servo_3
                  }
                  writer={(state, value) => {
                    state[MSGID.POWER_CALIBRATION].current_servo_3 = value
                  }}
                  rightElement={<Tag minimal>mA</Tag>}
                  // style={{ maxWidth: '150px' }}
                />
              </td>
            </tr>

            {expansion_servo ? (
              <></>
            ) : (
              <tr>
                <td>
                  <b>Servo 4</b>
                </td>
                <td>
                  <Printer accessor={state => state[MSGID.SERVO][3]?.power} /> W
                </td>
                <td>
                  <NumberInput
                    accessor={state =>
                      state[MSGID.POWER_CALIBRATION].current_servo_4
                    }
                    writer={(state, value) => {
                        state[MSGID.POWER_CALIBRATION].current_servo_4 = value
                      }}
                    rightElement={<Tag minimal>mA</Tag>}
                    // style={{ maxWidth: '150px' }}
                  />
                </td>
              </tr>
            )}
          </tbody>
        </HTMLTable>
      </Box>
    </Composition>
  )
}
