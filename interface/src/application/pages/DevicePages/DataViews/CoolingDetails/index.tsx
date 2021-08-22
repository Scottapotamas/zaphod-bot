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
} from '@blueprintjs/core'
import {
  IntervalRequester,
  StateTree,
  useHardwareState,
} from '@electricui/components-core'

import {
  ChartContainer,
  LineChart,
  RealTimeDomain,
  TimeAxis,
  VerticalAxis,
} from '@electricui/components-desktop-charts'
import {
  MessageDataSource,
} from '@electricui/core-timeseries'

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

const temperatureDataSource = new MessageDataSource('temp')
const fanDataSource = new MessageDataSource('fan')

export const CoolingDetails = () => {
  const ambient_temp = useHardwareState(state => state.temp.ambient).toFixed(0)
  const regulator_temp = useHardwareState(
    state => state.temp.regulator,
  ).toFixed(0)
  const supply_temp = useHardwareState(state => state.temp.supply).toFixed(0)
  const cpu_temp = useHardwareState(state => state.temp.cpu).toFixed(0)

  const fanspeed = useHardwareState(state => state.fan.rpm).toFixed(0)
  const fansetting = useHardwareState(state => state.fan.setpoint).toFixed(0)

  return (
    <div>
      <IntervalRequester variables={['temp']} interval={500} />
      <IntervalRequester variables={['fan']} interval={250} />

      <Composition gap={20}>
        <ChartContainer height={300}>
          <LineChart
            dataSource={temperatureDataSource}
            accessor={event => event.ambient}
            maxItems={1000}
            lineWidth={3}
          />
          <LineChart
            dataSource={temperatureDataSource}
            accessor={event => event.regulator}
            maxItems={1000}
            lineWidth={3}
          />
          <LineChart
            dataSource={temperatureDataSource}
            accessor={event => event.supply}
            maxItems={1000}
            lineWidth={3}
          />
          <LineChart
            dataSource={temperatureDataSource}
            accessor={event => event.cpu}
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
          <Statistic value={ambient_temp} label={`Ambient`} suffix="º" />
          <Statistic value={regulator_temp} label={`DC-DC Reg`} suffix="º" />
          <Statistic value={supply_temp} label={`AC-DC PSU`} suffix="º" />
          <Statistic value={cpu_temp} label={`CPU Core`} suffix="º" />
        </Statistics>
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
    </div>
  )
}
