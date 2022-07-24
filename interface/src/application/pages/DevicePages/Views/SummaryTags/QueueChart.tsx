import React from 'react'
import { IconNames } from '@blueprintjs/icons'
import {
  IntervalRequester,
  StateTree,
  useHardwareState,
} from '@electricui/components-core'
import { MessageDataSource } from '@electricui/core-timeseries'
import {
  ChartContainer,
  LineChart,
  RealTimeDomain,
  TimeAxis,
  VerticalAxis,
} from '@electricui/components-desktop-charts'
import { Colors, Icon } from '@blueprintjs/core'
import { Statistic, Statistics } from '@electricui/components-desktop-blueprint'
import { Printer } from '@electricui/components-desktop'
import { useSetting } from 'src/application/pages/toolpaths/interface/state'
import { MSGID } from 'src/application/typedState'

export const MoveQueueText = () => {
  const queue_depth = useHardwareState(
    state => state[MSGID.QUEUE_INFO].movements,
  )
  const queue_depth_ui = useSetting(state => state.movementQueueUI)

  return (
    <div
      style={{
        width: '100%',
        color: Colors.CERULEAN4,
      }}
    >
      {/* <Icon icon={IconNames.EXCHANGE} size={32}/>  */}
      {/* M <Printer accessor={state => state[MSGID.QUEUE_INFO].movements} /> / {queue_depth_ui} */}

      <Statistics>
        <Statistic>
          <Statistic.Value
            accessor={state => state[MSGID.QUEUE_INFO].movements}
          />
          <Statistic.Label>
            <div style={{ fontWeight: 'normal' }}>MOVES QUEUED</div>
          </Statistic.Label>
        </Statistic>
        <Statistic>
          <Statistic.Value>{queue_depth_ui}</Statistic.Value>
          <Statistic.Label>
            <div style={{ fontWeight: 'normal' }}>TOTAL MOVES</div>
          </Statistic.Label>
        </Statistic>
      </Statistics>
    </div>
  )
}

export const LEDQueueText = () => {
  const queue_depth_ui = useSetting(state => state.lightQueueUI)

  return (
    <div
      style={{
        width: '100%',
        color: Colors.TURQUOISE4,
      }}
    >
      {/* <Icon icon={IconNames.FLASH} size={32} /> */}
      <Statistics>
        <Statistic>
          <Statistic.Value
            accessor={state => state[MSGID.QUEUE_INFO].lighting}
          />
          <Statistic.Label>
            <div style={{ fontWeight: 'normal' }}>FADES QUEUED</div>
          </Statistic.Label>
        </Statistic>
        <Statistic>
          <Statistic.Value>{queue_depth_ui}</Statistic.Value>
          <Statistic.Label>
            <div style={{ fontWeight: 'normal' }}>TOTAL FADES</div>
          </Statistic.Label>
        </Statistic>
      </Statistics>
    </div>
  )
}

const queueDataSource = new MessageDataSource(MSGID.QUEUE_INFO)

export const QueueChart = () => {
  return (
    <div style={{ position: 'relative', height: '100px' }}>
      <div
        style={{
          width: '100%',
          height: '100%',
          position: 'absolute',
          top: 0,
          left: 0,
          zIndex: -1,
        }}
      >
        <ChartContainer height="100%" width="100%">
          <LineChart
            dataSource={queueDataSource}
            accessor={state => state.movements}
            color={Colors.CERULEAN4}
          />
          <LineChart
            dataSource={queueDataSource}
            accessor={state => state.lighting}
            color={Colors.TURQUOISE4}
          />
          <RealTimeDomain window={20_000} yMin={0} yMaxSoft={10} />
        </ChartContainer>
      </div>
      <div
        style={{
          position: 'relative',
          top: '50%',
          transform: 'translateY(-50%)',
          fontSize: '1em',
        }}
      >
        <MoveQueueText />
        <LEDQueueText />
      </div>
    </div>
  )
}
