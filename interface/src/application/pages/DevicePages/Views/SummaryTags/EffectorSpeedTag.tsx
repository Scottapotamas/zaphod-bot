import React from 'react'
import { Composition, Box, Only } from 'atomic-layout'
import {
  Tag,
} from '@blueprintjs/core'
import { IconNames } from '@blueprintjs/icons'
import { Printer } from '@electricui/components-desktop'
import { MSGID } from 'src/application/typedState'

type TagProps = {
  isLarge?: boolean
}

export const EffectorSpeedTag = (props: TagProps) => {
  return (
      <Tag
        minimal
        fill
        large={props.isLarge}
        icon={IconNames.PIVOT}
      >
        <div style={{ textAlign: 'right' }}>
          <Printer accessor={state => state[MSGID.MOTION].effector_speed} precision={1} /> mm/sec
        </div>
      </Tag>
  )
}

// TODO: consider making the chart available in a popover?
{/* 

const moveDataSource = new MessageDataSource(MSGID.MOTION)


<ChartContainer height="8em">
  <LineChart
    dataSource={moveDataSource}
    accessor={state => state.effector_speed}
    color={Colors.BLUE4}
  />
  <RealTimeDomain window={20_000} />
  <VerticalAxis />
  <TimeAxis />
</ChartContainer>  
*/}
