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
import { ServoInfo, MSGID } from '../../../../typedState'

const lightModeColours = [
  Colors.GREEN2,
  Colors.RED2,
  Colors.BLUE2,
  Colors.GOLD2,
]
const darkModeColours = [Colors.GREEN5, Colors.RED5, Colors.BLUE5, Colors.GOLD5]

const servoTelemetryDataSource = new MessageDataSource(MSGID.SERVO)

export const AngleChart = () => {
  const numMotors: number | null = useHardwareState(
    state => (state[MSGID.SERVO] || []).length,
  )
  
  const isDark = useDarkMode()
  const servoColours = isDark ? darkModeColours : lightModeColours

  return (
    <ChartContainer>
      {Array.from(new Array(numMotors)).map((_, index) => (
        <LineChart
          dataSource={servoTelemetryDataSource}
          accessor={state => state[index].target_angle}
          color={servoColours[index]}
          key={`angle_${index}`}
          // lineWidth={3}
        />
      ))}
      <RealTimeDomain
        window={[10_000, 30_000]}
        yMin={-45}
        yMaxSoft={20}
        delay={50}
      />
      <TimeAxis />
      <VerticalAxis tickFormat={(tick: number) => `${tick}°`} tickCount={4}/>

    </ChartContainer>
  )
}
