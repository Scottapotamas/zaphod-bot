import React, { useState } from 'react'

import { Button, Slider } from '@electricui/components-desktop-blueprint'
import { Icon, MultiSlider, NumericInput, Callout } from '@blueprintjs/core'

import { Composition } from 'atomic-layout'

const SliderAreas = `
HorizontalXArea VerticalZArea
HorizontalYArea VerticalZArea
`

export const TrackPalette = () => {
  return (
    <div>
      {/* <Button
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
            </Button> */}
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
                    state.tpos = {
                      x: values.target_x,
                      y: state.tpos.y,
                      z: state.tpos.z,
                    }
                  }}
                >
                  <Slider.Handle
                    accessor={state => state.tpos.x}
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
                  state.tpos = {
                    x: state.tpos.x,
                    y: values.target_y,
                    z: state.tpos.z,
                  }
                }}
              >
                <Slider.Handle
                  accessor={state => state.tpos.y}
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
                  state.tpos = {
                    x: state.tpos.x,
                    y: state.tpos.y,
                    z: values.target_z,
                  }
                }}
              >
                <Slider.Handle
                  accessor={state => state.tpos.z}
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
