import React, { useState } from 'react'
import { Button, Slider } from '@electricui/components-desktop-blueprint'
import { Icon, MultiSlider, NumericInput } from '@blueprintjs/core'
import { Grid, Cell } from 'styled-css-grid'
import { CALL_CALLBACK } from '@electricui/core'

const TrackPalette = () => {
  return (
    <div>
      <Grid
        columns={'70% 30%'}
        justifyContent="space-around"
        alignContent="center"
        style={{ padding: '0 50px' }}
      >
        <Cell center middle>
          <h4>X</h4>
          <Slider
            min={-100}
            max={100}
            stepSize={0.1}
            labelStepSize={25}
            writer={values => ({
              tpos: {
                x: values.target_x,
              },
            })}
          >
            <Slider.Handle accessor={state => state.tpos.x} name="target_x" />
          </Slider>
          <br />
          <h4>Y</h4>
          <Slider
            min={-100}
            max={100}
            stepSize={0.1}
            labelStepSize={25}
            writer={values => ({
              tpos: {
                y: values.target_y,
              },
            })}
          >
            <Slider.Handle accessor={state => state.tpos.y} name="target_y" />
          </Slider>
        </Cell>
        <Cell center middle>
          <div style={{ padding: '0 75px' }}>
            <h4>Z</h4>

            <Slider
              vertical
              min={0}
              max={100}
              stepSize={0.1}
              labelStepSize={25}
              writer={values => ({
                tpos: {
                  z: values.target_z,
                },
              })}
            >
              <Slider.Handle accessor={state => state.tpos.z} name="target_z" />
            </Slider>
          </div>
        </Cell>
      </Grid>
    </div>
  )
}

export default TrackPalette
