import React, { useState } from 'react'
import { RouteComponentProps } from '@reach/router'

import { Card } from '@blueprintjs/core'
import { Composition, Box, Only, useBreakpointChange } from 'atomic-layout'

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
            Controls Controls Controls
          `

const MainWindow = (props: RouteComponentProps) => {
  return (
    <React.Fragment>
      <div
        style={{
          position: 'absolute',
          top: '0',
          left: '0',
          width: '100%',
          height: '100vh',
          zIndex: '-1',
        }}
      >
        <RiggedModel />
      </div>

      <Composition
        areas={layoutDescription}
        templateCols="minmax(200px, max-content) auto minmax(200px, max-content)"
        gap={10}
        // placeItems="center end"

        justifyContent="space-between"
        justifyItems="center"
        alignContent="space-between"
        style={{ height: '100%' }}
      >
        {Areas => (
          <React.Fragment>
            <Areas.Sidebar>
              <Composition gap={30} templateCols="auto">
                <RobotSummary />

                <Composition gap={10} templateCols="1fr 1fr">
                  <ArmControlButton />
                  <HomeButton />
                </Composition>

                <ServoSummary />
              </Composition>
            </Areas.Sidebar>

            <Areas.Controls>
              <Composition gap={10} templateCols="1fr auto 1fr">
                <ModeSelection />
                <ControlBlock />
              </Composition>
            </Areas.Controls>

            <Areas.Charts>
              <Composition gap={10} templateCols="minmax(300px, max-content)">
                <AngleChart />
                <LoadChart />
                <PowerChart />
              </Composition>
            </Areas.Charts>
          </React.Fragment>
        )}
      </Composition>
    </React.Fragment>
  )
}

export default MainWindow
