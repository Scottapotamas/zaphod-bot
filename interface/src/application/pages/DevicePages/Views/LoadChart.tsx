import React from 'react'

import {
  ChartContainer,
  LineChart,
  RealTimeDomain,
  TimeAxis,
  VerticalAxis,
} from '@electricui/components-desktop-charts'

import { Colors } from '@blueprintjs/core'
import { useHardwareState } from '@electricui/components-core'

import { MessageDataSource } from '@electricui/core-timeseries'
import { useDarkMode } from '@electricui/components-desktop'
import { MSGID, ServoInfo } from '../../../typedState'

const lightModeColours = [
  Colors.GREEN2,
  Colors.RED2,
  Colors.BLUE2,
  Colors.GOLD2,
]
const darkModeColours = [Colors.GREEN5, Colors.RED5, Colors.BLUE5, Colors.GOLD5]

const servoTelemetryDataSource = new MessageDataSource(MSGID.SERVO)

export const LoadChart = () => {
  const numMotors: number | null = useHardwareState(
    state => (state.servo || []).length,
  )

  const isDark = useDarkMode()
  const servoColours = isDark ? darkModeColours : lightModeColours

  return (
    <ChartContainer>
      {Array.from(new Array(numMotors)).map((_, index) => (
        <LineChart
          dataSource={servoTelemetryDataSource}
          accessor={state => state[index].feedback}
          maxItems={10000}
          color={servoColours[index]}
          key={`torque_${index}`}
          // lineWidth={3}
        />
      ))}
      <RealTimeDomain
        window={[10_000, 30_000]}
        yMin={-10}
        yMax={10}
        delay={50}
      />
      <TimeAxis />
      <VerticalAxis label="Servo Torque %" />
    </ChartContainer>
  )
}
