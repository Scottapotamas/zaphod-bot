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

const FanMode = () => {
  const fanstate = useHardwareState(state => state.fan.state)

  if (fanstate == 0) {
    return <div>Off</div>
  } else if (fanstate == 1) {
    return <div>Stall</div>
  } else if (fanstate == 2) {
    return <div>Startup</div>
  } else if (fanstate == 3) {
    return <div>OK</div>
  }

  return <div>null</div>
}

const fanDataSource = new MessageDataSource('fan')

export const FanTag = () => {
  const fanspeed = useHardwareState(state => state.fan.rpm).toFixed(0)
  const fansetting = useHardwareState(state => state.fan.setpoint).toFixed(0)

  return (
    <div>
      <IntervalRequester variables={['fan']} interval={250} />

      <Popover interactionKind={PopoverInteractionKind.HOVER}>
        <Tag fill large minimal icon={IconNames.FLAME} intent={Intent.NONE}>
          <div
            style={{
              width: '2em',
              height: '1em',
              contain: 'strict',
              textAlign: 'right',
              display: 'inline-block',
            }}
          >
            <FanMode />
          </div>
        </Tag>
        <Composition padding={30} gap={20} minWidth="500px">
          <ChartContainer height={300}>
            <LineChart
              dataSource={fanDataSource}
              accessor={event => event.rpm}
              maxItems={1000}
              lineWidth={3}
            />
            {/* Plot a 10-minute window */}
            <RealTimeDomain
              window={[5000, 10_000, 20_000, 60_000]}
              delay={500}
              yMin={0}
              yMax={1400}
            />
            <TimeAxis />
            <VerticalAxis label="Fan RPM" />
          </ChartContainer>
          <Statistics>
            <Statistic value={<FanMode />} label="operation" />
            <Statistic value={fanspeed} label={`RPM, at ${fansetting}%`} />
          </Statistics>
        </Composition>
      </Popover>
    </div>
  )
}
