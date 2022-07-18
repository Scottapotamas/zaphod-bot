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

import {
  IntervalRequester,
  useHardwareStateSubscription,
} from '@electricui/components-core'

import { MSGID } from 'src/application/typedState'

import { SystemSummary } from './Views/SystemSummary'
import { RobotSummary } from './Views/RobotSummary'

import { ServoSummary } from './Views/ServoStatistics/ServoSummary'
import { AngleChart } from './Views/ServoStatistics/AngleChart'
import { LoadChart } from './Views/ServoStatistics/LoadChart'
import { PowerChart } from './Views/ServoStatistics/PowerChart'
import { RotationRateChart } from './Views/ServoStatistics/RotationRateChart'

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

  // Report errors to console
  useHardwareStateSubscription(
    state => state.err,
    (err: string) => {
      console.error(`Hardware reporting error: ${err}`)
    },
  )

  return (
    <Composition gap={20} templateCols="1fr">
      <Composition templateCols="auto 1fr" gap={10}>
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

        <RobotSummary isLarge={true} />
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
      <IntervalRequester interval={16} variables={[MSGID.POSITION_CURRENT]} />
      <IntervalRequester interval={100} variables={[MSGID.SERVO]} />

      {/* 3D Model view and toolpath planning */}
      <div
        style={{
          position: 'absolute',
          top: '0',
          left: '0',
          width: '100vw',
          height: '100vh',
        }}
      >
        <GeometryToolpathViewer />
      </div>

      {/* Right sidebar for high level config/controls */}
      <div style={{ minWidth: 450, maxWidth: 450, left: '1em', position: 'absolute' }}>
        <SideBar />
      </div>

      <div
        style={{ minWidth: 450, maxWidth: 600,left: '1em', bottom: '1em', position: 'absolute' }}
      >
        <ControlBlock />
      </div>

      {/* Left Sidebar for telemetry */}
      <Only from={{ minWidth: 1200 }}>
      <div style={{ width: '30vw', right: '1em', position: 'absolute' }}>
        <Composition gap={10}>
          <SystemSummary />
          <ServoSummary />
          <AngleChart />
          <RotationRateChart />
          <LoadChart />
          <PowerChart />
        </Composition>
      </div>
      </Only>

    </React.Fragment>
  )
}

export default MainWindow
