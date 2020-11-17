import React, { useState } from 'react'
import { RouteComponentProps } from '@reach/router'

import { Card } from '@blueprintjs/core'
import { Composition, Box, Only, useBreakpointChange } from 'atomic-layout'

import ModeSelector from './ControlPalette/ModeSelector'
import SystemController from './SystemController'
import DataViews from './DataViews'
import { ThreeD } from './DataViews/ThreeD'

import LightingPalette from '../../components/LEDControlPalette'

const VisualisationPanel = () => (
  <>
    <Only from={{ minWidth: 1800 }}>
      <Composition templateCols="1fr 600px">
        <ThreeD />
        <DataViews />
      </Composition>
    </Only>
    <Only to={{ minWidth: 1800 }}>
      <DataViews />
    </Only>
  </>
)

const MainWindow = (props: RouteComponentProps) => {
  // const [currentBreakpoint, setCurrentBreakpoint] = useState()

  // useBreakpointChange(breakpointName => {
  //   setCurrentBreakpoint(breakpointName)
  // })

  return (
    <React.Fragment>
      <Composition
        templateCols="600px 1fr"
        templateRows="1fr"
        justifyContent="space-around"
        style={{ height: '100%' }}
        gutter={15}
      >
        <Composition alignContent="space-between" width="600px">
          <Card>
            <ModeSelector />
          </Card>
          <Card>
            <LightingPalette />
          </Card>
          <Card>
            <SystemController />
          </Card>
        </Composition>
        <Only from={{ minWidth: 1180 }}>
          <VisualisationPanel />
        </Only>
      </Composition>
    </React.Fragment>
  )
}

export default MainWindow
