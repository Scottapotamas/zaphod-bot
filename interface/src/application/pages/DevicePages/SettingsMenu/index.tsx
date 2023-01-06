import React from 'react'

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

import { GeneralSettings } from './General'
import { InfoBlock } from './Info'
import { LEDCalibrationBlock } from './LEDCalibrationBlock'
import { PowerCalibrationBlock } from './PowerCalibrationBlock'
import { ExpansionServoSettings } from './ExpansionServo'

export const SettingsPage = () => {
  return (
    <div>
      <h2 style={{ textAlign: 'center' }}>SYSTEM CONFIGURATION</h2>
      <div style={{ margin: '3em', marginTop: '0' }}>
        <Tabs renderActiveTabPanelOnly>
          <Tabs.Expander />
          <Tab id="general" title="GENERAL" panel={<GeneralSettings />} />
          <Tab id="expansion" title="EXPANSION" panel={<ExpansionServoSettings />} />
          <Tab id="info" title="INFO" panel={<InfoBlock />} />
          <Tab id="led" title="LED CALIBRATION" panel={<LEDCalibrationBlock />} />
          <Tab id="power" title="POWER CALIBRATION" panel={<PowerCalibrationBlock />} />
          <Tabs.Expander />
        </Tabs>
      </div>
    </div>
  )
}
