import React, { useState } from 'react'

import { Button, Slider } from '@electricui/components-desktop-blueprint'
import { Icon, MultiSlider, NumericInput, Callout } from '@blueprintjs/core'

import { Composition } from 'atomic-layout'
import { MSGID } from 'src/application/typedState'

const SliderAreas = `
HorizontalXArea VerticalZArea
HorizontalYArea VerticalZArea
`

export const TrackPalette = () => {
  return (
    <div>
      <Composition
        areas={SliderAreas}
        gap={20}
        templateCols="auto auto"
      >
        {({ HorizontalXArea, HorizontalYArea, VerticalZArea }) => (
          <React.Fragment>
            <HorizontalXArea>
              <Callout title="X" style={{minWidth: '400px', padding: '2em'}}>
                <Slider
                  min={-100}
                  max={100}
                  stepSize={0.1}
                  labelStepSize={50}
                  throttleDuration={10}
                  writer={(state, values) => {
                    state[MSGID.POSITION_TARGET] = {
                      x: values.target_x,
                      y: state[MSGID.POSITION_TARGET].y,
                      z: state[MSGID.POSITION_TARGET].z,
                    }
                  }}
                >
                  <Slider.Handle
                    accessor={state => state[MSGID.POSITION_TARGET].x}
                    name="target_x"
                  />
                </Slider>
              </Callout>
            </HorizontalXArea>
            <HorizontalYArea>
            <Callout title="Y" style={{minWidth: '400px', padding: '2em'}}>
            <Slider
                min={-100}
                max={100}
                stepSize={0.1}
                labelStepSize={25}
                throttleDuration={10}
                writer={(state, values) => {
                  state[MSGID.POSITION_TARGET] = {
                    x: state[MSGID.POSITION_TARGET].x,
                    y: values.target_y,
                    z: state[MSGID.POSITION_TARGET].z,
                  }
                }}
              >
                <Slider.Handle
                  accessor={state => state[MSGID.POSITION_TARGET].y}
                  name="target_y"
                />
              </Slider>
</Callout>

            </HorizontalYArea>
            <VerticalZArea>
            <Callout title="Z" style={{minWidth: '100px', padding: '2em'}}>
            <Slider
                vertical
                min={0}
                max={100}
                stepSize={0.1}
                labelStepSize={25}
                throttleDuration={10}
                writer={(state, values) => {
                  state[MSGID.POSITION_TARGET] = {
                    x: state[MSGID.POSITION_TARGET].x,
                    y: state[MSGID.POSITION_TARGET].y,
                    z: values.target_z,
                  }
                }}
              >
                <Slider.Handle
                  accessor={state => state[MSGID.POSITION_TARGET].z}
                  name="target_z"
                />
              </Slider>

              </Callout>


            </VerticalZArea>
          </React.Fragment>
        )}
      </Composition>
    </div>
  )
}
