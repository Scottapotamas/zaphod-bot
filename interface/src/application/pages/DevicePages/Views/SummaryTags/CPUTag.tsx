import {
  Popover,
  Colors,
  Tag,
  Intent,
  PopoverInteractionKind,
} from '@blueprintjs/core'

import React, { useCallback, useState } from 'react'
import { Printer } from '@electricui/components-desktop'
import { IntervalRequester } from '@electricui/components-core'
import { Composition } from 'atomic-layout'

import {
  ChartContainer,
  LineChart,
  RealTimeDomain,
  VerticalAxis,
  TimeAxis,
} from '@electricui/components-desktop-charts'

import { MessageDataSource } from '@electricui/core-timeseries'
import { IconNames } from '@blueprintjs/icons'

const cpuDataSource = new MessageDataSource('sys')

export const CPUTag = () => {
  return (
    <div>
      <IntervalRequester interval={200} variables={['sys']} />

      {/* <Popover interactionKind={PopoverInteractionKind.HOVER}> */}
        <Tag fill large minimal icon={IconNames.STOPWATCH} intent={Intent.NONE}>
          <div
            style={{
              minWidth: '2em',
              width: '100%',
              height: '1em',
              contain: 'strict',
              textAlign: 'right',
              display: 'inline-block',
            }}
          >
            <Printer accessor={state => state.sys.cpu_load} precision={0} />%
          </div>
        </Tag>
        {/* <div style={{minWidth: '250px', minHeight: "80px"}}>Chart removed because <br/>of material issue</div> */}
        {/* 
        <Composition padding={30} minWidth="500px">
          <ChartContainer height={300}>
            <LineChart
              dataSource={cpuDataSource}
              accessor={event => event.cpu_load}
              maxItems={1000}
              lineWidth={3}
            />

            <RealTimeDomain
              window={[5000, 10_000]}
              delay={500}
              yMin={0}
              yMax={100}
            />
            <TimeAxis />
            <VerticalAxis label="Active Time %" /> 
          </ChartContainer>
        </Composition>
        */}
      {/* </Popover> */}
    </div>
  )
}
