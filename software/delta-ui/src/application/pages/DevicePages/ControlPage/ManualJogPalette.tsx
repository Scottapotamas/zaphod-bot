import React from 'react'

import {
  IntervalRequester,
  useHardwareState,
  StateTree,
} from '@electricui/components-core'
import { Button } from '@electricui/components-desktop-blueprint'
import { Slider } from '@blueprintjs/core'
import { Grid, Cell } from 'styled-css-grid'
import { CALL_CALLBACK } from '@electricui/core'

const ManualJogPalette = () => {
  let jog_distance = 5
  let jog_duration = 300

  return (
    <div>
      <Grid columns={2}>
        <Grid columns={3}>
          <Cell />
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
                  num_points: 1,
                  points: [[jog_distance, 0, 0]],
                },
                qumv: CALL_CALLBACK,
              }}
            >
              +x
            </Button>
          </Cell>
          <Cell />
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
                  num_points: 1,
                  points: [[0, jog_distance, 0]],
                },
                qumv: CALL_CALLBACK,
              }}
            >
              +y
            </Button>
          </Cell>
          <Cell />
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
                  num_points: 1,
                  points: [[0, -jog_distance, 0]],
                },
                qumv: CALL_CALLBACK,
              }}
            >
              -y
            </Button>
          </Cell>
          <Cell />
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
                  num_points: 1,
                  points: [[-jog_distance, 0, 0]],
                },
                qumv: CALL_CALLBACK,
              }}
            >
              -x
            </Button>
          </Cell>
          <Cell />
        </Grid>
        <Grid columns={1}>
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
                qumv: CALL_CALLBACK,
              }}
            >
              Up
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
                qumv: CALL_CALLBACK,
              }}
            >
              Down
            </Button>
          </Cell>
        </Grid>
        <Slider
          min={0}
          max={10}
          stepSize={1}
          labelStepSize={2}
          value={jog_distance}
        />
        <Slider
          min={0}
          max={10}
          stepSize={1}
          labelStepSize={2}
          value={jog_distance}
        />
      </Grid>
    </div>
  )
}

export default ManualJogPalette
