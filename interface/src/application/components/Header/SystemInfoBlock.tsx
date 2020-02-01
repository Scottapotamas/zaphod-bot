import { HTMLTable } from '@blueprintjs/core'
import {
  IntervalRequester,
  useHardwareState,
} from '@electricui/components-core'

import React from 'react'
import { Composition, Box } from 'atomic-layout'
import { Statistic } from '@electricui/components-desktop-blueprint'
import { Printer } from '@electricui/components-desktop'

const SensorsActive = () => {
  const sensorEnabledState =
    useHardwareState<number>(state => state.sys.sensors_enable) === 1
  if (sensorEnabledState) {
    return <div>ADC Enabled</div>
  }

  return <div>ADC Disabled</div>
}

const ModuleActive = () => {
  const ExpansionModuleState =
    useHardwareState<number>(state => state.sys.module_enable) === 1
  if (ExpansionModuleState) {
    return <div>Add-in Module On</div>
  }

  return <div>Add-in Module Off</div>
}

export const CoreSystemsInfoCard = () => {
  const cpu_percentage = useHardwareState(state => state.sys.cpu_load)
  const cpu_clock = useHardwareState(state => state.sys.cpu_clock)
  const cpu_temp = useHardwareState(state => state.sys.cpu_temp).toFixed(0)

  return (
    <Composition
      templateCols="1fr 1fr"
      padding="30px"
      justifyItems="center"
      gap={30}
    >
      <Box>
        <IntervalRequester interval={200} variables={['sys']} />
        <Composition templateCols="auto" gap={20} justifyItems="center">
          <Box>
            <Statistic
              value={cpu_percentage}
              label={`load at ${cpu_clock}MHz`}
              suffix="%"
            />
          </Box>
          <Box>
            <Statistic value={cpu_temp} label="CPU Temp" suffix="º" />
          </Box>
          <Box>
            <Composition templateCols="1fr 1fr" justifyItems="center">
              <Box>
                <SensorsActive />
              </Box>
              <Box>
                <ModuleActive />
              </Box>
            </Composition>
          </Box>
        </Composition>
      </Box>
      <Box>
        <HTMLTable striped style={{ minWidth: '100%' }}>
          <tbody>
            <tr>
              <td>
                <b>Version</b>
              </td>
              <td>
                <Printer accessor={state => state.fwb.version} />
              </td>
            </tr>
            <tr>
              <td>
                <b>Branch</b>
              </td>
              <td>
                <Printer accessor={state => state.fwb.branch} />
              </td>
            </tr>
            <tr>
              <td>
                <b>Info</b>
              </td>
              <td>
                <Printer accessor={state => state.fwb.info} />
              </td>
            </tr>
            <tr>
              <td>
                <b>Date</b>
              </td>
              <td>
                <Printer accessor={state => state.fwb.date} />
              </td>
            </tr>
            <tr>
              <td>
                <b>Time</b>
              </td>
              <td>
                <Printer accessor={state => state.fwb.time} />
              </td>
            </tr>
            <tr>
              <td>
                <b>Type</b>
              </td>
              <td>
                <Printer accessor={state => state.fwb.type} />
              </td>
            </tr>
          </tbody>
        </HTMLTable>
      </Box>
    </Composition>
  )
}
