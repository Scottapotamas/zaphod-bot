import React from 'react'
import { RouteComponentProps } from '@reach/router'
import { Card } from '@blueprintjs/core'
import { Grid, Cell } from 'styled-css-grid'

import { useDarkMode } from '@electricui/components-desktop'
import {
  IntervalRequester,
  useHardwareState,
  StateTree,
} from '@electricui/components-core'

import CameraCard from './CameraCard'
import SystemOverview from './SystemOverview'
import SystemController from './SystemController'

const MotorSafetyMode = () => {
  const motor_state = useHardwareState(state => state.super.motors)
  var motors_are_active: string = 'null'

  switch (motor_state) {
    case 0: {
      motors_are_active = 'OFF'
      break
    }
    case 1: {
      motors_are_active = 'ON'
      break
    }
    default: {
      motors_are_active = 'INVALID'
      break
    }
  }

  return <div>{motors_are_active}</div>
}

const ControlPage = (props: RouteComponentProps) => {
  const isDarkMode = useDarkMode()

  return (
    <React.Fragment>
      <IntervalRequester interval={125} variables={['moStat', 'cpos', 'rgb']} />

      <Grid
        columns="repeat(auto-fit,minmax(600px,0.6fr))"
        justifyContent="space-around"
        alignContent="space-around"
      >
        <Cell>
          <SystemController />
        </Cell>
        <Cell>
          <SystemOverview />
        </Cell>
        <Cell>
          <CameraCard />
        </Cell>
      </Grid>
    </React.Fragment>
  )
}

export default ControlPage
