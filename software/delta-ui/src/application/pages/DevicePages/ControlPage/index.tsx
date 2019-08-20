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

const ControlPage = (props: RouteComponentProps) => {
  const isDarkMode = useDarkMode()

  return (
    <React.Fragment>
      <IntervalRequester interval={125} variables={['moStat', 'cpos', 'rgb']} />
      <IntervalRequester interval={200} variables={['mo1', 'mo2', 'mo3']} />

      <Grid
        columns="repeat(auto-fit,minmax(600px,0.6fr))"
        justifyContent="space-around"
        alignContent="space-around"
      >
        <Cell>
          <SystemController />
          <br />
          <SystemOverview />
        </Cell>
        <Cell />
        <Cell>
          <CameraCard />
        </Cell>
      </Grid>
    </React.Fragment>
  )
}

export default ControlPage
