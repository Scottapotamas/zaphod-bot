import React, { useState } from 'react'
import { RouteComponentProps } from '@reach/router'

import { Card } from '@blueprintjs/core'
import { Composition, Box, Only, useBreakpointChange } from 'atomic-layout'
import { IntervalRequester } from '@electricui/components-core'

import RiggedModel from '../../components/RiggedModel'

import { RobotSummary } from './Views/RobotSummary'
import { ServoSummary } from './Views/ServoSummary'

import { AngleChart } from './Views/AngleChart'
import { LoadChart } from './Views/LoadChart'
import { PowerChart } from './Views/PowerChart'

import { ControlBlock } from './Controls/ControlBlock'
import { ArmControlButton } from './Controls/ArmButton'
import { HomeButton } from './Controls/HomeButton'
import { ModeSelection } from './Controls/ModeSelection'

const layoutDescription = `
            Sidebar . Charts
            Sidebar . Charts
            Controls . Charts
          `

const MainWindow = (props: RouteComponentProps) => {
  return (
    <React.Fragment>
      <IntervalRequester interval={50} variables={['cpos']} />
      <IntervalRequester interval={80} variables={['servo']} />

      <div
        style={{
          position: 'absolute',
          top: '0',
          left: '0',
          width: '100vw',
          height: '100vh',
          zIndex: '-1',
        }}
      >
        <RiggedModel />
      </div>

      <Composition
        areas={layoutDescription}
        templateCols="minmax(300px, max-content) auto minmax(300px, max-content)"
        gap={10}
        justifyContent="space-between"
        alignContent="space-between"
        style={{ height: '100%' }}
      >
        {Areas => (
          <React.Fragment>
            <Areas.Sidebar>
              <Composition gapRow={30} templateCols="auto">
                <RobotSummary />
                <Composition gap={10} templateCols="1fr 1fr">
                  <ArmControlButton />
                  <HomeButton />
                </Composition>
                <ModeSelection />
              </Composition>
            </Areas.Sidebar>

            <Areas.Controls>
                <ControlBlock />
            </Areas.Controls>

            <Areas.Charts>
              <Composition gap={10}>
                <ServoSummary />
                {/* <AngleChart />
                <LoadChart />
                <PowerChart /> */}
              </Composition>
            </Areas.Charts>
          </React.Fragment>
        )}
      </Composition>
    </React.Fragment>
  )
}

export default MainWindow
