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
  Colors,
} from '@blueprintjs/core'
import { IconNames } from '@blueprintjs/icons'

import {
  IntervalRequester,
  useHardwareStateSubscription,
} from '@electricui/components-core'

import { MSGID } from 'src/application/typedState'

import { RobotSummary } from './Views/RobotSummary'
import { ServoSummary } from './Views/ServoStatistics/ServoSummary'

import { ControlBlock } from './Controls/ControlBlock'
import { EStopButton } from './Controls/EStopButton'
import { ModeSelection } from './Controls/ModeSelection'
import { SettingsPage } from './SettingsMenu'

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

        <RobotSummary isLarge={false} />
      </Composition>

      <ModeSelection />

      <EStopButton />

      <Dialog
        isOpen={configOpen}
        onClose={closeConfig}
        autoFocus
        transitionDuration={150}
        backdropClassName="settingsBackgroundBlur"
        style={{
          minWidth: '800px',
          minHeight: '400px',
          borderStyle: 'solid',
          background: 'unset',
          borderWidth: '2px',
          borderRadius: '6px',
          borderColor: `${Colors.BLUE1}`,
          backdropFilter: `blur(15px) brightness(0.5)`,
          padding: `0.25em`,
        }}
      >
        <SettingsPage />
      </Dialog>
    </Composition>
  )
}


const MainWindow = (props: RouteComponentProps) => {
  return (
    <React.Fragment>
      <IntervalRequester interval={100} variables={[MSGID.SERVO]} />
      <IntervalRequester
        variables={[MSGID.SUPERVISOR, MSGID.POSITION_CURRENT]}
        interval={50}
      />
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
        <div style={{ maxWidth: '40vw', right: '1em', position: 'absolute' }}>
          <ServoSummary />
        </div>
      </Only>
    </React.Fragment>
  )
}

export default MainWindow
