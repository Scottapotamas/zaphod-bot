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
