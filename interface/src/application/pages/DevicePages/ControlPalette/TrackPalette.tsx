import React, { useState } from 'react'

import { Button, Slider } from '@electricui/components-desktop-blueprint'
import { Icon, MultiSlider, NumericInput } from '@blueprintjs/core'

import { Composition } from 'atomic-layout'

const SliderAreas = `
HorizontalXArea VerticalZArea
HorizontalYArea VerticalZArea
`

const TrackPalette = () => {
  return (
    <Composition
      areas={SliderAreas}
      padding={30}
      gap={50}
      templateCols="1fr auto"
    >
      {({ HorizontalXArea, HorizontalYArea, VerticalZArea }) => (
        <React.Fragment>
          <HorizontalXArea>
            <Button
              writer={state => {
                state.tpos = {
                  x: 0,
                  y: 0,
                  z: 40,
                }
              }}
            >
              Pos 1
            </Button>
            <Button
              writer={state => {
                state.tpos = {
                  x: 10,
                  y: 10,
                  z: 40,
                }
              }}
            >
              Pos 2
            </Button>
            <h4>X</h4>
            <Slider
              min={-100}
              max={100}
              stepSize={0.1}
              labelStepSize={25}
              throttleDuration={10}
              writer={(state, values) => {
                state.tpos = {
                  x: values.target_x,
                  y: state.tpos.y,
                  z: state.tpos.z,
                }
              }}
            >
              <Slider.Handle accessor={state => state.tpos.x} name="target_x" />
            </Slider>
          </HorizontalXArea>
          <HorizontalYArea>
            <h4>Y</h4>
            <Slider
              min={-100}
              max={100}
              stepSize={0.1}
              labelStepSize={25}
              throttleDuration={10}
              writer={(state, values) => {
                state.tpos = {
                  x: state.tpos.x,
                  y: values.target_y,
                  z: state.tpos.z,
                }
              }}
            >
              <Slider.Handle accessor={state => state.tpos.y} name="target_y" />
            </Slider>
          </HorizontalYArea>
          <VerticalZArea>
            <h4>Z</h4>

            <Slider
              vertical
              min={0}
              max={100}
              stepSize={0.1}
              labelStepSize={25}
              throttleDuration={10}
              writer={(state, values) => {
                state.tpos = {
                  x: state.tpos.x,
                  y: state.tpos.y,
                  z: values.target_z,
                }
              }}
            >
              <Slider.Handle accessor={state => state.tpos.z} name="target_z" />
            </Slider>
          </VerticalZArea>
        </React.Fragment>
      )}
    </Composition>
  )
}

export default TrackPalette
