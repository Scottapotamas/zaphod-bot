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
  Colors,
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
import { ChartContainer, LineChart, RealTimeDomain } from '@electricui/charts'
import { MessageDataSource } from '@electricui/core-timeseries'

const voltageDataSource = new MessageDataSource(MSGID.SYSTEM)
const servoDataSource = new MessageDataSource(MSGID.SERVO)

interface ServoRowProps {
  index: number
}

const ServoTableRow = (props: ServoRowProps) => {
  let servo_number_readable: number = props.index + 1

  let color: string = Colors.WHITE
  switch (props.index) {
    case 0:
      color = Colors.GREEN3
      break
    case 1:
      color = Colors.RED3
      break
    case 2:
      color = Colors.BLUE3
      break
    case 3:
      color = Colors.GOLD3
      break
  }

  return (
    <tr key={servo_number_readable}>
      <td>
        <b>Servo {servo_number_readable}</b>
      </td>
      <td>
        <ChartContainer height={50} width={200}>
          <LineChart
            dataSource={servoDataSource}
            accessor={event => event[props.index].power}
            lineWidth={3}
            color={color}
          />
          <RealTimeDomain
            window={10_000}
            delay={50}
            yMinSoft={0}
            yMaxSoft={10}
          />
        </ChartContainer>
      </td>
      <td>
        <Printer accessor={state => state[MSGID.SERVO][props.index].power} /> W
      </td>
      <td>
        <NumberInput
          accessor={state => state[MSGID.POWER_CALIBRATION].current_servo[props.index]}
          writer={(state, value) => {
            state[MSGID.POWER_CALIBRATION].current_servo[props.index] = value
          }}
          rightElement={<Tag minimal>mA</Tag>}
          minorStepSize={1}
          stepSize={10}
          majorStepSize={100}
          style={{ maxWidth: '120px' }}
        />
      </td>
    </tr>
  )
}

export const PowerCalibrationBlock = () => {
  const expansion_servo = useHardwareState(state => !!state[MSGID.SERVO][3])

  return (
    <Composition justifyItems="center">
      <Box>
        <IntervalRequester
          variables={[MSGID.SYSTEM, MSGID.SERVO]}
          interval={50}
        />
        <HTMLTable style={{ minWidth: '100%' }}>
          <thead>
            <tr>
              <th>Sensor</th>
              <th>Trend</th>
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
                <ChartContainer height={50} width={200}>
                  <LineChart
                    dataSource={voltageDataSource}
                    accessor={event => event.input_voltage}
                    lineWidth={3}
                    color={Colors.INDIGO3}
                  />
                  <RealTimeDomain
                    window={10_000}
                    delay={50}
                  />
                </ChartContainer>
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
                  minorStepSize={1}
                  stepSize={50}
                  majorStepSize={500}
                  style={{ maxWidth: '120px' }}
                />
              </td>
            </tr>

            {[0, 1, 2].map(servoIndex => (
              <ServoTableRow index={servoIndex} />
            ))}

            {expansion_servo ? <></> : <ServoTableRow index={3} />}
          </tbody>
        </HTMLTable>
      </Box>
    </Composition>
  )
}
