import {
  Button as BlueprintButton,
  Position,
  Tooltip,
  Dialog,
  Intent,
  PopoverInteractionKind,
} from '@blueprintjs/core'

import { Button } from '@electricui/components-desktop-blueprint'

import { ToastConnector } from '../../Toaster'
import React, { useCallback, useState } from 'react'
import { RouteComponentProps } from '@reach/router'
import { navigate } from '@electricui/utility-electron'
import { Composition, Box } from 'atomic-layout'

import { MessageDataSource } from '@electricui/core-timeseries'
import { IconNames } from '@blueprintjs/icons'

import { SettingsPage } from './SettingsPage'
import { TemperatureTag } from './SummaryTags/TemperatureTag'
import { CPUTag } from './SummaryTags/CPUTag'
import { FanTag } from './SummaryTags/FanTag'
import { MSGID } from 'src/application/typedState'

interface InjectDeviceIDFromLocation {
  deviceID?: string
  '*'?: string // we get passed the path as the wildcard
}

const cpuLoadDataSource = new MessageDataSource('sys')

const BarAreas = `
Left Center Right
`

export const Header = (
  props: RouteComponentProps & InjectDeviceIDFromLocation,
) => {
  const page = props['*'] // we get passed the path as the wildcard, so we read it here

  const [configOpen, setConfigOpen] = useState(false)
  const toggleConfig = useCallback(() => setConfigOpen(value => !value), [
    setConfigOpen,
  ])
  const openConfig = useCallback(() => setConfigOpen(true), [setConfigOpen])
  const closeConfig = useCallback(() => setConfigOpen(false), [setConfigOpen])

  // const cpu_percentage = useHardwareState(state => state.sys.cpu_load)

  return (
    <div>
      <Composition
        areas={BarAreas}
        gap={10}
        padding={10}
        justifyContent="space-between"
      >
        {Areas => (
          <React.Fragment>
            <Areas.Left>
              <Tooltip content="Back to device list" position={Position.BOTTOM}>
                <BlueprintButton
                  minimal
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
                  large
                  icon={IconNames.WRENCH}
                  onClick={toggleConfig}
                />
              </Tooltip>
            </Areas.Left>

            <Areas.Center>
              <div style={{ minWidth: '300px', maxWidth: '500px' }}>
                <Button
                  callback={MSGID.EMERGENCY_STOP}
                  intent={Intent.DANGER}
                  fill
                  large
                >
                  E-STOP
                </Button>
              </div>
            </Areas.Center>

            <Areas.Right>
              <Composition templateCols="auto auto auto auto" gap={10}>
                <CPUTag />
                <TemperatureTag />
                <FanTag />
              </Composition>
            </Areas.Right>
          </React.Fragment>
        )}
      </Composition>

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

      <ToastConnector />
    </div>
  )
}
