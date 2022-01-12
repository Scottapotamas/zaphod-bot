import React from 'react'
import { RouteComponentProps } from '@reach/router'
import { Composition, Box, Only } from 'atomic-layout'

import {
  Colors,
  Callout,
  Tooltip,
  Position,
  Intent,
  Menu,
  MenuItem,
  MenuDivider,
  Tab,
  Tabs,
} from '@blueprintjs/core'
import { IconNames, IconName } from '@blueprintjs/icons'
import {
  IntervalRequester,
  useHardwareState,
} from '@electricui/components-core'

import { InfoBlock } from './InfoBlock'
import { LEDCalibrationBlock } from './LEDCalibrationBlock'
import { KinematicsBlock } from './KinematicsBlock'

export const SettingsPage = () => {
  return (
    <div style={{ margin: '1em' }}>
      <Tabs animate vertical large renderActiveTabPanelOnly>
        <Tab id="info" title="Info" panel={
        // <InfoBlock />
        <div style={{backgroundColor: 'blue'}}>
          blah blah
        </div>
        } style={{width: "100%"}}/>
        <Tab id="kinematics" title="Kinematics" panel={<KinematicsBlock />} />
        <Tab id="led" title="LED Calibration" panel={<LEDCalibrationBlock />} />
        <Tab
          id="power"
          title="Power Calibration"
          panel={<div>Power Cal</div>}
        />

        <Tabs.Expander />
      </Tabs>
    </div>
  )
}
