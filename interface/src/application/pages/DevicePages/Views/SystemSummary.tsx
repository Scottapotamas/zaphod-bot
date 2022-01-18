import React from 'react'

import { Composition, Box } from 'atomic-layout'

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

import { CPUTag } from './SummaryTags/CPUTag'
import { QueueText, LEDQueueText } from './SummaryTags/QueueTag'

const lightModeColours = [
  Colors.GREEN2,
  Colors.RED2,
  Colors.BLUE2,
  Colors.GOLD2,
]
const darkModeColours = [Colors.GREEN5, Colors.RED5, Colors.BLUE5, Colors.GOLD5]

const queueDataSource = new MessageDataSource(MSGID.QUEUE_INFO)

const layoutDescription = `
            CPU Chart
            MoveQueue Chart
            LEDQueue Chart
          `

export const SystemSummary = () => {
  const numMotors: number | null = useHardwareState(
    state => (state[MSGID.SERVO] || []).length,
  )

  const isDark = useDarkMode()
  const servoColours = isDark ? darkModeColours : lightModeColours

  return (
    <Composition areas={layoutDescription} templateCols="1fr 3fr">
      {Areas => (
        <React.Fragment>
          <Areas.CPU>
            <CPUTag />
          </Areas.CPU>
          <Areas.MoveQueue>
            <QueueText />
          </Areas.MoveQueue>
          <Areas.LEDQueue>
            <LEDQueueText />
          </Areas.LEDQueue>
          <Areas.Chart>
            <ChartContainer height="8em">
              {/* 
              <LineChart
                dataSource={queueDataSource}
                accessor={state => state.movements}
                color={Colors.CERULEAN4}
              />
              <LineChart
                dataSource={queueDataSource}
                accessor={state => state.lighting}
                color={Colors.TURQUOISE4}
              /> 
              */}
              <RealTimeDomain window={20_000} />
            </ChartContainer>
          </Areas.Chart>
        </React.Fragment>
      )}
    </Composition>
  )
}
