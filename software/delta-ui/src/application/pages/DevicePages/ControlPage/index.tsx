import { Card, Icon } from '@blueprintjs/core'
import {
  IntervalRequester,
  StateTree,
  useHardwareState,
} from '@electricui/components-core'
import { useDarkMode } from '@electricui/components-desktop'
import { RouteComponentProps } from '@reach/router'
import React from 'react'
import { Cell, Grid } from 'styled-css-grid'

import SystemController from './SystemController'
import SystemOverview from './SystemOverview'

const ControlPage = (props: RouteComponentProps) => {
  const isDarkMode = useDarkMode()

  return (
    <React.Fragment>
      <IntervalRequester interval={100} variables={['moStat', 'cpos']} />
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

        <Cell>3d</Cell>
      </Grid>
    </React.Fragment>
  )
}

export default ControlPage
