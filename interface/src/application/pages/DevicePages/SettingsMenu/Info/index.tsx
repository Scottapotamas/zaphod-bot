import { Colors, HTMLTable, Icon, Intent } from '@blueprintjs/core'
import {
  IntervalRequester,
  useHardwareState,
} from '@electricui/components-core'

import React from 'react'
import { Composition, Box } from 'atomic-layout'
import { Statistic } from '@electricui/components-desktop-blueprint'
import { Printer } from '@electricui/components-desktop'
import { MSGID } from 'src/application/typedState'
import { MessageDataSource } from '@electricui/core-timeseries'

import { SectionHeading } from '../SectionHeading'
import { SparkTag } from 'src/application/components/SparkTag'

const LastResetReason = () => {
  const last_reset_reason = useHardwareState(state => state[MSGID.RESET_CAUSE])

  if (last_reset_reason) {
    return <div>Last reset: {last_reset_reason}</div>
  }

  return <div>Error getting last reset reason</div>
}

const LastAssertReason = () => {
  const last_assert_reason = useHardwareState(
    state => state[MSGID.ASSERT_CAUSE],
  )

  if (last_assert_reason == '_') {
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
    return <div>CPU ({cpu_clock}MHz)</div>
  }

  return <div>CPU (? MHZ)</div>
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
    <HTMLTable
      condensed
      interactive
      striped
      style={{ minWidth: '100%', width: '300px' }}
    >
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

const sysDS = new MessageDataSource(MSGID.SYSTEM)
const fanDS = new MessageDataSource(MSGID.FAN)
const queueDS = new MessageDataSource(MSGID.QUEUE_INFO)

export const InfoBlock = () => {
  const num_tasks: number | null = useHardwareState(
    state => (state.tasks || []).length,
  )

  return (
    <Composition templateCols="1fr 1fr 1fr" justifyItems="center" gap={20}>
      <Box>
        <IntervalRequester interval={200} variables={[MSGID.SYSTEM]} />
        <SectionHeading text="Status" />

        <Composition
          autoCols="minmax(100px, 150px)"
          autoFlow="column"
          gapCol={5}
        >
          <SparkTag
            title="CPU" //TODO put the clock speed in here
            color={Colors.GRAY4}
            drawValue
            datasource={sysDS}
            accessor={state => state.cpu_load}
          />
        </Composition>
      </Box>

      <Box>
        <SectionHeading text="Queues" />
        <Composition
          autoCols="minmax(80px, 180px)"
          autoFlow="column"
          gapCol={5}
        >
          <SparkTag
            title="Move"
            color={Colors.BLUE4}
            drawValue
            datasource={queueDS}
            accessor={state => state.movements}
            min={0}
            max={150}
          />
          <SparkTag
            title="Fade"
            color={Colors.GREEN4}
            drawValue
            datasource={queueDS}
            accessor={state => state.lighting}
            min={0}
            max={250}
          />
        </Composition>
      </Box>

      <Box>
        <SectionHeading text="Thermals" />
        <Composition
          autoCols="minmax(100px, 180px)"
          autoFlow="column"
          gapCol={5}
        >
          <SparkTag
            title="RPM"
            color={Colors.RED4}
            drawValue
            datasource={fanDS}
            accessor={state => state.rpm}
            min={0}
            max={2500}
          />
          <SparkTag
            title="Ambient"
            color={Colors.INDIGO3}
            drawValue
            datasource={sysDS}
            accessor={state => state.temp_ambient}
            min={0}
            max={60}
          />
          <SparkTag
            title="75V PSU"
            color={Colors.CERULEAN4}
            drawValue
            datasource={sysDS}
            accessor={state => state.temp_supply}
            min={0}
            max={60}
          />
          <SparkTag
            title="CPU"
            color={Colors.TURQUOISE4}
            drawValue
            datasource={sysDS}
            accessor={state => state.temp_cpu}
            min={0}
            max={60}
          />
        </Composition>
      </Box>

      <Box>
        <SectionHeading text="Debug" />

        <br />
        <IntervalRequester
          interval={1000}
          variables={[MSGID.ASSERT_CAUSE, MSGID.RESET_CAUSE, MSGID.FAN]}
        />
        <LastResetReason />
        <LastAssertReason />
      </Box>

      <Box>
        <SectionHeading text="Update" />
        TODO: File selector here
      </Box>

      <Box>
        <SectionHeading text="Firmware Build" />
        <BuildInfoTable />
      </Box>
    </Composition>
  )
}
