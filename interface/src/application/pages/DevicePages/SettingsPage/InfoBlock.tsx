import { HTMLTable, Icon, Intent } from '@blueprintjs/core'
import {
  IntervalRequester,
  useHardwareState,
} from '@electricui/components-core'

import React from 'react'
import { Composition, Box } from 'atomic-layout'
import { Statistic } from '@electricui/components-desktop-blueprint'
import { Printer } from '@electricui/components-desktop'
import { MSGID } from 'src/application/typedState'

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
    return <div>Add-in module Enabled</div>
  }

  return <div>Internal Comms Disabled</div>
}

const LastResetReason = () => {
  const last_reset_reason = useHardwareState(state => state[MSGID.RESET_CAUSE])

  if (last_reset_reason) {
    return <div>Last reset: {last_reset_reason}</div>
  }

  return <div>Error getting last reset reason</div>
}

const CPUClockText = () => {
  const cpu_clock = useHardwareState(state => state.sys.cpu_clock)

  if (cpu_clock) {
    return <div>CPU Clock: {cpu_clock}Mhz</div>
  }

  return <div>Error getting CPU clockspeed</div>
}

const BuildInfoTable = () => {
  return <HTMLTable striped style={{ minWidth: '100%', width: '300px' }}>
  <tbody>
    <tr>
      <td>
        <b>Name</b>
      </td>
      <td>
        <Printer accessor={state => state[MSGID.FIRMWARE_INFO].name} />
      </td>
    </tr>
    <tr>
      <td>
        <b>Branch</b>
      </td>
      <td>
        <Printer accessor={state => state[MSGID.FIRMWARE_INFO].branch} />
      </td>
    </tr>
    <tr>
      <td>
        <b>Info</b>
      </td>
      <td>
        <Printer accessor={state => state[MSGID.FIRMWARE_INFO].info} />
      </td>
    </tr>
    <tr>
      <td>
        <b>Type</b>
      </td>
      <td>
        <Printer accessor={state => state[MSGID.FIRMWARE_INFO].type} />
      </td>
    </tr>
    <tr>
      <td>
        <b>Date</b>
      </td>
      <td>
        <Printer accessor={state => state[MSGID.FIRMWARE_INFO].date} />
      </td>
    </tr>
    <tr>
      <td>
        <b>Time</b>
      </td>
      <td>
        <Printer accessor={state => state[MSGID.FIRMWARE_INFO].time} />
      </td>
    </tr>
  </tbody>
</HTMLTable>
}


const SystemInfoLayout = `
Stats Build
`

export const InfoBlock = () => {
  const num_tasks: number | null = useHardwareState(
    state => (state.tasks || []).length,
  )

  return (
    <Composition areas={SystemInfoLayout} gap={10} justifyItems="center center" style={{backgroundColor: 'blue', width: '100%'}}>
      {Areas => (
        <React.Fragment>
          <Areas.Stats>
            <IntervalRequester interval={200} variables={[MSGID.SYSTEM]} />
            <h3>System Configuration</h3>
            <SensorsActive />
            <br />
            <ModuleActive />
            <br />
            <LastResetReason />
            <br />
            <CPUClockText />
          </Areas.Stats>

          <Areas.Build>
            <BuildInfoTable/>
          </Areas.Build>
        </React.Fragment>
      )}
    </Composition>
  )
}
