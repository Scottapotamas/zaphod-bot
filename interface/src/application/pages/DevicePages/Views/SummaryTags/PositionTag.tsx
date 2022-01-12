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

type TagProps = {
  isLarge?: boolean
}

export const PositionTag = (props: TagProps) => {
  return (
    <Composition templateCols="1fr" gap={props.isLarge ? 10 : 5}>
      <Tag
        minimal
        fill
        large={props.isLarge}
        icon={IconNames.DOUBLE_CARET_HORIZONTAL}
      >
        <div style={{ textAlign: 'right' }}>
          <Printer accessor={state => state.cpos.x} precision={2} /> mm
        </div>
      </Tag>
      <Tag
        minimal
        fill
        large={props.isLarge}
        icon={IconNames.DOUBLE_CARET_VERTICAL}
      >
        <div style={{ textAlign: 'right' }}>
          <Printer accessor={state => state.cpos.y} precision={2} /> mm
        </div>
      </Tag>
      <Tag minimal fill large={props.isLarge} icon={IconNames.CARET_UP}>
        <div style={{ textAlign: 'right' }}>
          <Printer accessor={state => state.cpos.z} precision={2} /> mm
        </div>
      </Tag>
    </Composition>
  )
}
