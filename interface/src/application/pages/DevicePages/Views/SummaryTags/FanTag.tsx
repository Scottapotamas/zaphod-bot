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
import { MSGID, FanState } from '../../../../typedState'

function fanModeAccessor(state: ElectricUIDeveloperState) {
  switch (state[MSGID.FAN].state) {
    case FanState.OFF:
      return 'Off'
    case FanState.STALL:
      return 'Stall'
    case FanState.STARTUP:
      return 'Startup'
    case FanState.OK:
      return 'Ok'

    default:
      return 'unknown'
  }
}

const fanDataSource = new MessageDataSource(MSGID.FAN)

export const FanTag = () => {
  return (
    <div>
      <IntervalRequester variables={[MSGID.FAN]} interval={250} />

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
            <Printer accessor={fanModeAccessor} />
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
            <Statistic accessor={fanModeAccessor} label="operation" />
            <Statistic
              // TODO: use precision={0} once Statistics support a precision prop
              // accessor={state => state.fan.rpm}
              value={
                <Printer accessor={state => state[MSGID.FAN].rpm} precision={0} />
              }
              label={
                <>
                  RPM, at{' '}
                  {
                    <Printer
                      accessor={state => state[MSGID.FAN].setpoint}
                      precision={0}
                    />
                  }
                  %
                </>
              }
            />
          </Statistics>
        </Composition>
      </Popover>
    </div>
  )
}
