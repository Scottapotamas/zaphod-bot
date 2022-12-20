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
    useHardwareState<number>(state => state[MSGID.SYSTEM].sensors_enable) === 1
  if (sensorEnabledState) {
    return <div>ADC Enabled</div>
  }

  return <div>ADC Disabled</div>
}

const ModuleActive = () => {
  const ExpansionModuleState =
    useHardwareState<number>(state => state[MSGID.SYSTEM].module_enable) === 1
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

const LastAssertReason = () => {
  const last_assert_reason = useHardwareState(
    state => state[MSGID.ASSERT_CAUSE]
  )

  if (last_assert_reason == "_") {
    return <div>Last assert: N/A</div>
  }

  if (last_assert_reason) {
    return <div>Last assert: {last_assert_reason}</div>
  }

  return <div>Error getting last assert reason</div>
}

const CPUClockText = () => {
  const cpu_clock = useHardwareState(state => state[MSGID.SYSTEM].cpu_clock)

  if (cpu_clock) {
    return <div>CPU Clock: {cpu_clock}Mhz</div>
  }

  return <div>Error getting CPU clockspeed</div>
}

const CPULoadText = () => {
  const cpu_load = useHardwareState(state => state[MSGID.SYSTEM].cpu_load)

  if (cpu_load) {
    return <div>CPU Load: {cpu_load}%</div>
  }

  return <div>???</div>
}

const BuildInfoTable = () => {
  return (
    <HTMLTable condensed interactive striped style={{ minWidth: '100%', width: '300px' }}>
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
  )
}

export const InfoBlock = () => {
  const num_tasks: number | null = useHardwareState(
    state => (state.tasks || []).length,
  )

  return (
    <Composition templateCols="1fr 1fr" justifyItems="start">
      <Box>
        <IntervalRequester interval={200} variables={[MSGID.SYSTEM]} />
        <h2>System Configuration</h2>
        <SensorsActive />
        <ModuleActive />
        <br />
        <IntervalRequester interval={1000} variables={[[MSGID.ASSERT_CAUSE], [MSGID.RESET_CAUSE]]} />

        <LastResetReason />
        <LastAssertReason />
        <br />
        <CPUClockText /> <CPULoadText /> 
      </Box>
      <Box>
        <h2>Firmware Build</h2>
        <BuildInfoTable />
      </Box>
      <Box>
        <h2>Thermals</h2>
        Temperature sensors here
        <br/>
        Fan speed/duty here
      </Box>
      <Box>
        <h2>Manual Update</h2>
        Load a valid update file for flashing:
        <br/>
        File selector here
      </Box>

    </Composition>
  )
}
