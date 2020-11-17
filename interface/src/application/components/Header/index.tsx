import {
  Button as BlueprintButton,
  Popover,
  Position,
  Tooltip,
  Colors,
} from '@blueprintjs/core'
import {
  IntervalRequester,
  useHardwareState,
} from '@electricui/components-core'

import { Button } from '@electricui/components-desktop-blueprint'

import { ToastConnector } from '../../Toaster'
import React from 'react'
import { RouteComponentProps } from '@reach/router'
import { navigate } from '@electricui/utility-electron'
import { Composition, Box } from 'atomic-layout'

import { CoreSystemsInfoCard } from './SystemInfoBlock'
import { SystemSettingsCard } from './SystemConfigBlock'

import {
  ChartContainer,
  LineChart,
  RealTimeDomain,
} from '@electricui/components-desktop-charts'

import {
  MessageDataSource,
  RollingStorageRequest,
} from '@electricui/core-timeseries'

interface InjectDeviceIDFromLocation {
  deviceID?: string
  '*'?: string // we get passed the path as the wildcard
}

const CPULoadSparkline = () => {
  const cpu_percentage = useHardwareState(state => state.sys.cpu_load)

  return (
    <div>
      <IntervalRequester interval={200} variables={['sys']} />
      <Composition templateCols="1fr 2fr" alignItems="center">
        <div
          style={{
            opacity: '50%',
            textAlign: 'right',
            position: 'relative',
            zIndex: 2,
          }}
        >
          {cpu_percentage}%
        </div>
        <ChartContainer height={40}>
          <LineChart
            dataSource={cpuLoadDataSource}
            accessor={state => state.sys.cpu_load}
            maxItems={1000}
            color={Colors.GRAY1}
          />
          <RealTimeDomain
            window={[10_000, 60_000]}
            yMin={0}
            yMax={100}
            delay={100}
          />
        </ChartContainer>
      </Composition>
    </div>
  )
}

const BarAreas = `
Left Center Right
`
const cpuLoadDataSource = new MessageDataSource('sys')

export const Header = (
  props: RouteComponentProps & InjectDeviceIDFromLocation,
) => {
  const page = props['*'] // we get passed the path as the wildcard, so we read it here

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
            </Areas.Left>
            <Areas.Center>
              <div style={{ minWidth: '300px', maxWidth: '500px' }}>
                <Button callback="estop" intent="danger" fill large>
                  E-STOP
                </Button>
              </div>
            </Areas.Center>
            <Areas.Right>
              <Composition templateCols="3fr 1fr 1fr" gap={10}>
                <CPULoadSparkline />

                <Popover
                  content={
                    <div style={{ width: 'auto', height: 'auto' }}>
                      <SystemSettingsCard />
                    </div>
                  }
                  position={Position.BOTTOM}
                >
                  <Tooltip content="Settings" position={Position.BOTTOM}>
                    <BlueprintButton minimal large icon="settings" />
                  </Tooltip>
                </Popover>

                <Popover
                  content={
                    <div style={{ width: 'auto', height: 'auto' }}>
                      <CoreSystemsInfoCard />
                    </div>
                  }
                  position={Position.BOTTOM}
                >
                  <Tooltip content="System Info" position={Position.BOTTOM}>
                    <BlueprintButton minimal large icon="info-sign" />
                  </Tooltip>
                </Popover>
              </Composition>
            </Areas.Right>
          </React.Fragment>
        )}
      </Composition>

      {/* <Navbar style={{ background: 'transparent', boxShadow: 'none' }}>
        <div style={{ margin: '0 auto', width: '100%' }}>
          <Navbar.Group align={Alignment.LEFT}></Navbar.Group>

          <Navbar.Group></Navbar.Group>
          <Navbar.Group align={Alignment.RIGHT}></Navbar.Group>
        </div>
      </Navbar> */}
      <ToastConnector />
    </div>
  )
}
