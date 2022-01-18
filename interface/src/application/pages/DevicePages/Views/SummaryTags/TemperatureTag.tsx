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
import { MSGID } from 'src/application/typedState'

const temperatureDataSource = new MessageDataSource(MSGID.SYSTEM)

export const TemperatureTag = () => {
  return (
    <div>
      <IntervalRequester variables={[MSGID.SYSTEM]} interval={500} />

      <Popover interactionKind={PopoverInteractionKind.HOVER}>
        <Tag fill large minimal icon={IconNames.SNOWFLAKE} intent={Intent.NONE}>
          <div
            style={{
              width: '3em',
              height: '1em',
              contain: 'strict',
              textAlign: 'right',
              display: 'inline-block',
            }}
          >
            <Printer
              accessor={state => state[MSGID.SYSTEM].temp_regulator}
              precision={0}
            />{' '}
            °C
          </div>
        </Tag>
        <Composition padding={30} gap={20} minWidth="500px">
          <ChartContainer height={300}>
            <LineChart
              dataSource={temperatureDataSource}
              accessor={event => event.temp_ambient}
              maxItems={1000}
              lineWidth={3}
            />
            <LineChart
              dataSource={temperatureDataSource}
              accessor={event => event.temp_regulator}
              maxItems={1000}
              lineWidth={3}
            />
            <LineChart
              dataSource={temperatureDataSource}
              accessor={event => event.temp_supply}
              maxItems={1000}
              lineWidth={3}
            />
            <LineChart
              dataSource={temperatureDataSource}
              accessor={event => event.temp_cpu}
              maxItems={1000}
              lineWidth={3}
            />

            {/* Plot a 10-minute window */}
            <RealTimeDomain
              window={[5000, 10_000, 20_000, 60_000]}
              delay={500}
              yMin={20}
              yMax={50}
            />
            <TimeAxis />
            <VerticalAxis label="Temperature °C" />
          </ChartContainer>
          <Statistics>
            <Statistic
              value={
                <Printer accessor={state => state.temp.ambient} precision={0} />
              }
              label={`Ambient`}
              suffix="º"
            />
            <Statistic
              value={
                <Printer
                  accessor={state => state.temp.regulator}
                  precision={0}
                />
              }
              label={`DC-DC Reg`}
              suffix="º"
            />
            <Statistic
              value={
                <Printer accessor={state => state.temp.supply} precision={0} />
              }
              label={`AC-DC PSU`}
              suffix="º"
            />
            <Statistic
              value={
                <Printer accessor={state => state.temp.cpu} precision={0} />
              }
              label={`CPU Core`}
              suffix="º"
            />
          </Statistics>
        </Composition>
      </Popover>
    </div>
  )
}
