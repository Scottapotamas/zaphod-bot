import React from 'react'

import {
  IntervalRequester,
  useHardwareState,
  StateTree,
} from '@electricui/components-core'
import {
  Button,
  NumberInput,
  Slider,
} from '@electricui/components-desktop-blueprint'
import { Icon } from '@blueprintjs/core'
import { Grid, Cell } from 'styled-css-grid'
import { CALL_CALLBACK } from '@electricui/core'

const ManualJogPalette = () => {
  let jog_distance = 5
  let jog_duration = 300

  return (
    <div>
      <Grid columns={'80px 80px 160px 100px'} justifyContent="space-around">
        <Cell center>
          Speed
          <br />
          <br />
          <Slider
            min={-1.5}
            max={1.5}
            stepSize={0.25}
            vertical
            labelRenderer={val => `${Math.round(val * 100)}%`}
            labelStepSize={0.75}
          >
            <Slider.Handle accessor="jog_duration" />
          </Slider>
          <br />
          <NumberInput
            accessor="jog_duration"
            debounceDuration={250}
            fill
            intent="warning"
            min={0}
            max={10}
            stepSize={1}
          />
        </Cell>
        <Cell center>
          Distance
          <br />
          <br />
          <Slider
            min={0}
            max={10}
            stepSize={1}
            vertical
            labelRenderer={val => `${val}mm`}
            labelStepSize={5}
          >
            <Slider.Handle accessor="jog_distance" intentBefore="success" />
          </Slider>
          <br />
          <NumberInput
            accessor="jog_distance"
            debounceDuration={250}
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
                  qumv: CALL_CALLBACK,
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
                  qumv: CALL_CALLBACK,
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
                  qumv: CALL_CALLBACK,
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
                  qumv: CALL_CALLBACK,
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
                  qumv: CALL_CALLBACK,
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
                  qumv: CALL_CALLBACK,
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
