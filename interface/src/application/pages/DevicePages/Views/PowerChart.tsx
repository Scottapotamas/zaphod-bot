import React from 'react'
import { RouteComponentProps } from '@reach/router'
import { Composition, Box, Only } from 'atomic-layout'

import {
  ChartContainer,
  LineChart,
  RealTimeDomain,
  TimeAxis,
  TimeSlicedLineChart,
  RealTimeSlicingDomain,
  HorizontalAxis,
  VerticalAxis,
  Fog,
} from '@electricui/components-desktop-charts'

import { Statistic, Statistics } from '@electricui/components-desktop-blueprint'
import { Colors, Callout, Tooltip, Position, Intent } from '@blueprintjs/core'
import { IconNames, IconName } from '@blueprintjs/icons'
import {
  IntervalRequester,
  useHardwareState,
} from '@electricui/components-core'

import { MessageDataSource } from '@electricui/core-timeseries'
import { useDarkMode } from '@electricui/components-desktop'
import { ServoInfo } from '../../../typedState'

const lightModeColours = [
  Colors.GREEN2,
  Colors.RED2,
  Colors.BLUE2,
  Colors.GOLD2,
]
const darkModeColours = [Colors.GREEN5, Colors.RED5, Colors.BLUE5, Colors.GOLD5]

const servoTelemetryDataSource = new MessageDataSource('servo')

export const PowerChart = () => {
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
          accessor={state => state[index].power}
          maxItems={10000}
          color={servoColours[index]}
          key={`power_${index}`}
          // lineWidth={3}
        />
      ))}
      <RealTimeDomain window={[10_000, 30_000]} yMin={0} yMax={50} delay={50} />
      <TimeAxis />
      <VerticalAxis label="Servo Power W" />
    </ChartContainer>
  )
}
