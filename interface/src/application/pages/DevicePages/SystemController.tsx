import {
  Button,
  Statistic,
  Statistics,
} from '@electricui/components-desktop-blueprint'
import { CONTROL_MODES, SUPERVISOR_STATES } from '../../typedState'

import {
  IntervalRequester,
  useHardwareState,
} from '@electricui/components-core'

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

import { MessageDataSource } from '@electricui/core-timeseries'

import { Composition, Box } from 'atomic-layout'
import React from 'react'
import { Printer } from '@electricui/components-desktop'

import { RobotSummary } from './Views/RobotSummary'

const effectorPositionDataSource = new MessageDataSource('cpos')

const PositionVisualiser = () => {
  return (
    <div>
      <ChartContainer>
        <TimeSlicedLineChart
          dataSource={effectorPositionDataSource}
          xAccessor={event => event.x}
          yAccessor={event => event.y}
        />

        <RealTimeSlicingDomain
          window={500}
          xMin={-200}
          xMax={200}
          yMin={-200}
          yMax={200}
        />
        <HorizontalAxis label="X" />
        <VerticalAxis label="Y" />
        <Fog color="#fff" />
      </ChartContainer>
      <ChartContainer height={300}>
        <LineChart
          dataSource={effectorPositionDataSource}
          accessor={state => state.z}
          maxItems={10000}
        />
        <RealTimeDomain window={1000} yMin={0} yMax={300} />
        <TimeAxis />
        <VerticalAxis label="Z" />
      </ChartContainer>
    </div>
  )
}

const SupervisorState = () => {
  const supervisor_state = useHardwareState(state => state.super.supervisor)
  const is_moving = useHardwareState(state => state.moStat.pathing_state)

  let supervisor_text: string = 'null'

  if (supervisor_state === SUPERVISOR_STATES[SUPERVISOR_STATES.ARMED]) {
    if (is_moving == 1) {
      supervisor_text = 'MOVING'
    } else {
      supervisor_text = 'ARMED'
    }
  } else {
    supervisor_text = supervisor_state || 'UNKNOWN'
  }

  return <div>{supervisor_text}</div>
}

const ArmControlButton = () => {
  const supervisor = useHardwareState<string>(state => state.super.supervisor)
  const control_mode = useHardwareState(state => state.super.mode)

  const modeNotSelected = control_mode === CONTROL_MODES[CONTROL_MODES.NONE]
  const isArmed = supervisor === SUPERVISOR_STATES[SUPERVISOR_STATES.ARMED]

  if (!isArmed) {
    if (modeNotSelected) {
      return (
        <Button fill large disabled intent="none" callback="disarm">
          Select a mode before arming
        </Button>
      )
    } else {
      return (
        <Button fill large intent="warning" callback="arm">
          Arm ({control_mode})
        </Button>
      )
    }
  }

  return (
    <Button fill large intent="primary" callback="disarm">
      Disarm
    </Button>
  )
}

const systemOverviewAreas = `
RobotMode RobotMode
ArmButton HomeButton
`

const SystemController = () => {
  return (
    <div>
      <IntervalRequester variables={['cpos']} interval={50} />
      <Composition areas={systemOverviewAreas} gap={20} templateCols="4fr 3fr">
        {Areas => (
          <>
            <Areas.RobotMode>
              <RobotSummary />
              
            </Areas.RobotMode>
           
            <Areas.HomeButton>
              <Button fill large intent="success" callback="home">
                Home
              </Button>
            </Areas.HomeButton>
            <Areas.ArmButton>
              <ArmControlButton />
            </Areas.ArmButton>
          </>
        )}
      </Composition>
    </div>
  )
}

export default SystemController
