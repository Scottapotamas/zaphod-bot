import React from 'react'

import {
  IntervalRequester,
  useHardwareState,
  StateTree,
} from '@electricui/components-core'
import { Printer } from '@electricui/components-desktop'
import { Card, Text } from '@blueprintjs/core'
import { Grid, Cell } from 'styled-css-grid'
import { Statistics, Statistic } from '@electricui/components-desktop-blueprint'
import { Chart } from '@electricui/components-desktop-charts'

type ServoDataProps = {
  servo_object: any
  name: string
}

const ServoData = (props: ServoDataProps) => {
  return (
    <div>
      <Grid columns={2}>
        <Cell>
          <h3>Servo {props.name}</h3>
        </Cell>
        <Cell>
          <h3> Idle</h3>
        </Cell>
        <Cell>
          <Text>Power: {props.servo_object.power.toFixed(1)} W</Text>
        </Cell>
        <Cell>
          <Text>Angle: {props.servo_object.target_angle.toFixed(2)} deg</Text>
        </Cell>
        <Cell>
          <Text>FB: {props.servo_object.feedback}</Text>
        </Cell>
        <Cell>
          <Text>En: {props.servo_object.enabled}</Text>
        </Cell>
      </Grid>
      <br />
      <Chart
        timeseriesKey="graph"
        duration={10000}
        delay={1}
        hideLegend={true}
        yMin={0}
        yMax={1500}
        height={250}
      />
    </div>
  )
}

export default ServoData
