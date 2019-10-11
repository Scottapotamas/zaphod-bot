import React, { useState } from 'react'

import { Button, Slider } from '@electricui/components-desktop-blueprint'
import { Icon, MultiSlider, NumericInput } from '@blueprintjs/core'
import { CALL_CALLBACK } from '@electricui/core'

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
          </HorizontalXArea>
          <HorizontalYArea>
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
          </HorizontalYArea>
          <VerticalZArea>
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
          </VerticalZArea>
        </React.Fragment>
      )}
    </Composition>
  )
}

export default TrackPalette
