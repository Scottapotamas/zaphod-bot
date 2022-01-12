import React, { useCallback, useState } from 'react'
import { RouteComponentProps } from '@reach/router'
import { navigate } from '@electricui/utility-electron'

import { Composition, Box, Only, useBreakpointChange } from 'atomic-layout'

import {
  Card,
  Tooltip,
  Dialog,
  Intent,
  Position,
  Button as BlueprintButton,
} from '@blueprintjs/core'
import { IconNames } from '@blueprintjs/icons'

import { IntervalRequester } from '@electricui/components-core'

import { MSGID } from 'src/application/typedState'

import { RobotSummary } from './Views/RobotSummary'
import { ServoSummary } from './Views/ServoSummary'
import { AngleChart } from './Views/AngleChart'
import { LoadChart } from './Views/LoadChart'
import { PowerChart } from './Views/PowerChart'
import { ControlBlock } from './Controls/ControlBlock'
import { EStopButton } from './Controls/EStopButton'
import { ModeSelection } from './Controls/ModeSelection'
import { SettingsPage } from './SettingsPage'

import { GeometryToolpathViewer } from './Views/GeometryToolpaths'

const SideBar = () => {
  const [configOpen, setConfigOpen] = useState(false)
  const toggleConfig = useCallback(
    () => setConfigOpen(value => !value),
    [setConfigOpen],
  )
  const openConfig = useCallback(() => setConfigOpen(true), [setConfigOpen])
  const closeConfig = useCallback(() => setConfigOpen(false), [setConfigOpen])

  return (
    <Composition gap={20} templateCols="1fr">
      <Composition templateCols="auto 1fr"  gap={10}>
        <Composition templateCols="1fr" alignItems="center">
          <Tooltip content="Back to device list" position={Position.BOTTOM}>
            <BlueprintButton
              minimal
              outlined
              large
              icon="key-backspace"
              onClick={() => {
                navigate('/')
              }}
            />
          </Tooltip>

          <Tooltip content="Settings" position={Position.BOTTOM}>
            <BlueprintButton
              minimal
              outlined
              large
              icon={IconNames.WRENCH}
              onClick={toggleConfig}
            />
          </Tooltip>
        </Composition>

        <RobotSummary isLarge={true}/>
      </Composition>

      <ModeSelection />

         <EStopButton /> 

      <Dialog
        isOpen={configOpen}
        onClose={closeConfig}
        autoFocus
        title="System Configuration"
        icon={IconNames.WRENCH}
        style={{ minWidth: '800px', minHeight: '400px' }}
      >
        <SettingsPage />
      </Dialog>
    </Composition>
  )
}

const layoutDescription = `
            Sidebar Three Charts
            Sidebar Three Charts
            Controls Three Charts
          `

const MainWindow = (props: RouteComponentProps) => {
  return (
    <React.Fragment>
      <IntervalRequester interval={20} variables={['cpos']} />
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
        <GeometryToolpathViewer />
      </div>

      <Composition
        areas={layoutDescription}
        templateCols="minmax(300px, 30vw) auto minmax(300px, max-content)"
        gap={10}
        padding={10}
        justifyContent="space-between"
        alignContent="space-between"
        style={{ height: '100%' }}
      >
        {Areas => (
          <React.Fragment>
            <Areas.Sidebar>
              <SideBar />
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
