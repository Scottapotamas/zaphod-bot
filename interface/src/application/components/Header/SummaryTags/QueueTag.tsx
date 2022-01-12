import React from 'react'
import { RouteComponentProps } from '@reach/router'
import { Composition, Box, Only } from 'atomic-layout'
import {
  Button,
  Statistic,
  Statistics,
  Slider,
  Switch,
} from '@electricui/components-desktop-blueprint'
import {
  Card,
  Divider,
  HTMLTable,
  Icon,
  Label,
  Text,
  Tab,
  Tag,
  Popover,
  Intent,
  PopoverInteractionKind,
} from '@blueprintjs/core'
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
import { Printer } from '@electricui/components-desktop'
import { useSetting } from 'src/application/pages/toolpaths/interface/state'

export const QueueText = () => {
  const queue_depth = useHardwareState(state => state.queue.movements)
  const is_moving = useHardwareState(state => state.moStat.pathing_state) == 1
  const queue_depth_ui = useSetting(state => state.movementQueueUI)

  let iconColour: Intent

  if (queue_depth === 0) {
    iconColour = Intent.NONE
  } else if (queue_depth && queue_depth > 0 && queue_depth < 25) {
    if (is_moving) {
      iconColour = Intent.SUCCESS
    } else {
      iconColour = Intent.WARNING
    }
  } else {
    iconColour = Intent.DANGER
  }

  return (
    <Tag minimal large icon={IconNames.EXCHANGE} intent={iconColour}>
      <div
        style={{
          width: '4em',
          height: '1em',
          contain: 'strict',
          textAlign: 'right',
          display: 'inline-block',
        }}
      >
        {queue_depth} / {queue_depth_ui}
      </div>
    </Tag>
  )
}

export const LEDQueueText = () => {
  const queue_depth = useHardwareState(state => state.queue.lighting)
  const queue_depth_ui = useSetting(state => state.lightQueueUI)

  let iconColour: Intent

  if (queue_depth === 0) {
    iconColour = Intent.NONE
  } else if (queue_depth && queue_depth > 0 && queue_depth < 35) {
    iconColour = Intent.SUCCESS
  } else {
    iconColour = Intent.WARNING
  }

  return (
    <Tag minimal large icon={IconNames.FLASH} intent={iconColour}>
      <div
        style={{
          width: '4em',
          height: '1em',
          contain: 'strict',
          textAlign: 'right',
          display: 'inline-block',
        }}
      >
        {queue_depth} / {queue_depth_ui}
      </div>
    </Tag>
  )
}
