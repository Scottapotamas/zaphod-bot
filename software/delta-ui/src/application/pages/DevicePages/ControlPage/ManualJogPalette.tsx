import { Cell, Grid } from 'styled-css-grid'
import { Icon, MultiSlider, NumericInput } from '@blueprintjs/core'
import React, { useState } from 'react'

import { Button } from '@electricui/components-desktop-blueprint'
import { CALL_CALLBACK } from '@electricui/core'

const ManualJogPalette = () => {
  let [jog_speed, setJogSpeed] = useState(0)
  let [jog_distance, setJogDistance] = useState(5.0)

  let jog_duration = 600 * (1 / (1 + jog_speed))

  return (
    <div>
      <Grid columns={'80px 80px 160px 100px'} justifyContent="space-around">
        <Cell center>
          <label>Speed</label>
          <br />
          <br />
          <MultiSlider
            min={-0.75}
            max={1}
            stepSize={0.25}
            vertical
            labelRenderer={val => `${Math.round((val + 1) * 100)}%`}
            labelStepSize={0.5}
            showTrackFill={jog_speed !== 0}
          >
            <MultiSlider.Handle
              value={jog_speed}
              intentAfter={jog_speed < 0 ? 'warning' : undefined}
              intentBefore={jog_speed > 0 ? 'warning' : undefined}
              onChange={value => setJogSpeed(value)}
            />
            <MultiSlider.Handle value={0} interactionKind="none" />
          </MultiSlider>
          <br />
          <NumericInput
            value={jog_speed}
            onValueChange={value => setJogSpeed(value)}
            fill
            intent="warning"
            min={-1}
            max={1}
            stepSize={0.25}
          />
        </Cell>
        <Cell center>
          <label>Distance</label>
          <br />
          <br />
          <MultiSlider
            onChange={values => setJogDistance(values[0])}
            min={0}
            max={10}
            stepSize={1}
            vertical
            labelRenderer={val => `${val}mm`}
            labelStepSize={5}
          >
            <MultiSlider.Handle value={jog_distance} intentBefore="success" />
          </MultiSlider>
          <br />
          <NumericInput
            value={jog_distance}
            onValueChange={value => setJogDistance(value)}
            fill
            intent="success"
            min={0}
            max={10}
            stepSize={1}
          />
        </Cell>
        <Cell middle>
          <Grid columns={'50px 50px 50px'} rows={'50px 50px 50px'}>
            <Cell left={2}>
              <Button
                large
                fill
                writer={{
                  inmv: {
                    type: 0,
                    reference: 1,
                    id: 0,
                    duration: jog_duration,
                    num_points: 1,
                    points: [[jog_distance, 0, 0]],
                  },
                }}
              >
                <Icon icon="arrow-up" iconSize={40} />
              </Button>
            </Cell>
            <Cell left={1} top={2}>
              <Button
                large
                fill
                writer={{
                  inmv: {
                    type: 0,
                    reference: 1,
                    id: 0,
                    duration: jog_duration,
                    num_points: 1,
                    points: [[0, jog_distance, 0]],
                  },
                }}
              >
                <Icon icon="arrow-left" iconSize={40} />
              </Button>
            </Cell>
            <Cell left={3} top={2}>
              <Button
                large
                fill
                writer={{
                  inmv: {
                    type: 0,
                    reference: 1,
                    id: 0,
                    duration: jog_duration,
                    num_points: 1,
                    points: [[0, -jog_distance, 0]],
                  },
                }}
              >
                <Icon icon="arrow-right" iconSize={40} />
              </Button>
            </Cell>
            <Cell left={2} top={3}>
              <Button
                large
                fill
                writer={{
                  inmv: {
                    type: 0,
                    reference: 1,
                    id: 0,
                    duration: jog_duration,
                    num_points: 1,
                    points: [[-jog_distance, 0, 0]],
                  },
                }}
              >
                <Icon icon="arrow-down" iconSize={40} />
              </Button>
            </Cell>
          </Grid>
        </Cell>
        <Cell middle>
          <Grid columns={'100px'} rows={'50px'} alignContent="space-evenly">
            <Cell>
              <Button
                large
                fill
                writer={{
                  inmv: {
                    type: 0,
                    reference: 1,
                    id: 0,
                    duration: jog_duration,
                    points: [[0, 0, jog_distance]],
                  },
                }}
              >
                <Icon icon="arrow-up" iconSize={40} />
              </Button>
            </Cell>

            <Cell>
              <Button
                large
                fill
                writer={{
                  inmv: {
                    type: 0,
                    reference: 1,
                    id: 0,
                    duration: jog_duration,
                    points: [[0, 0, -jog_distance]],
                  },
                }}
              >
                <Icon icon="arrow-down" iconSize={40} />
              </Button>
            </Cell>
          </Grid>
        </Cell>
      </Grid>
    </div>
  )
}

export default ManualJogPalette
