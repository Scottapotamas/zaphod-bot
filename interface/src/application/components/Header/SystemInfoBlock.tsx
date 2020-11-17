import { HTMLTable, Icon, Intent } from '@blueprintjs/core'
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
    return <div>Add-in module Enabled</div>
  }

  return <div>Internal Comms Disabled</div>
}

const LastResetReason = () => {
  const last_reset_reason = useHardwareState(state => state.reset_type)

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

const SystemInfoLayout = `
Stats Build
Tasks Tasks
`

export const CoreSystemsInfoCard = () => {
  const num_tasks: number | null = useHardwareState(
    state => (state.tasks || []).length,
  )

  return (
    <Composition
      areas={SystemInfoLayout}
      padding={30}
      justifyItems="center"
      gap={10}
    >
      {Areas => (
        <React.Fragment>
          <Areas.Stats>
            <IntervalRequester interval={200} variables={['sys', 'tasks']} />
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
            <HTMLTable striped style={{ minWidth: '100%' }}>
              <tbody>
                <tr>
                  <td>
                    <b>Name</b>
                  </td>
                  <td>
                    <Printer accessor={state => state.fwb.name} />
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
                    <b>Type</b>
                  </td>
                  <td>
                    <Printer accessor={state => state.fwb.type} />
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
              </tbody>
            </HTMLTable>
          </Areas.Build>
          <Areas.Tasks>
            <HTMLTable striped style={{ minWidth: '100%' }}>
              <thead>
                <tr>
                  <th>Task</th>
                  <th>ID</th>
                  <th>Ready</th>
                  <th>Queue Usage</th>
                  <th>Waiting Max</th>
                  <th>Burst Max</th>
                </tr>
              </thead>
              <tbody>
                {Array.from(new Array(num_tasks)).map((_, index) => (
                  <>
                    <tr>
                      <td>
                        <b>
                          <Printer
                            accessor={state => state.tasks[index].name}
                          />
                        </b>
                      </td>
                      <td>
                        <Printer accessor={state => state.tasks[index].id} />
                      </td>
                      <td>
                        <Printer accessor={state => state.tasks[index].ready} />
                      </td>
                      <td>
                        <Printer
                          accessor={state => state.tasks[index].queue_used}
                        />{' '}
                        /{' '}
                        <Printer
                          accessor={state => state.tasks[index].queue_max}
                        />
                      </td>
                      <td>
                        <Printer
                          accessor={state => state.tasks[index].waiting_max}
                        />
                      </td>
                      <td>
                        <Printer
                          accessor={state => state.tasks[index].burst_max}
                        />
                      </td>
                    </tr>
                  </>
                ))}
              </tbody>
            </HTMLTable>
          </Areas.Tasks>
        </React.Fragment>
      )}
    </Composition>
  )
}
