import React, { useState } from 'react'

import {
  Icon,
  MultiSlider,
  NumericInput,
  Tag,
  Intent,
  Alignment,
} from '@blueprintjs/core'
import { Composition, Box } from 'atomic-layout'

import {
  Button,
  NumberInput,
  Switch,
} from '@electricui/components-desktop-blueprint'
import {
  ColorPalette,
  HueSlider,
  AlphaSlider,
} from '@electricui/components-desktop'

const ManualLEDPalette = () => {
  return (
    <Composition
      gap={10}
      templateCols="1fr auto"
      alignItems="center"
      justifyItems="center"
    >
      <Box>
        <ColorPalette
          height={200}
          width={200}
          red={state => state.manual_led.red * 255}
          green={state => state.manual_led.green * 255}
          blue={state => state.manual_led.blue * 255}
          writer={(state, color) => {
            state.manual_led.red = color.r / 255
            state.manual_led.green = color.g / 255
            state.manual_led.blue = color.b / 255
          }}
        />
        <HueSlider
          vertical
          height={200}
          red={state => state.manual_led.red * 255}
          green={state => state.manual_led.green * 255}
          blue={state => state.manual_led.blue * 255}
          writer={(state, color) => {
            state.manual_led.red = color.r / 255
            state.manual_led.green = color.g / 255
            state.manual_led.blue = color.b / 255
          }}
        />
      </Box>
      <Composition gap={10} templateCols="1fr" justifyItems="center">
        <b>LED CONTROL</b>
        <Switch
          large
          innerLabel="OFF"
          innerLabelChecked="ON"
          unchecked={0}
          checked={1}
          accessor={state => state.manual_led.enable}
          writer={(state, value) => (state.manual_led.enable = value)}
        ></Switch>
        <Button
          intent={Intent.DANGER}
          writer={state => {
            state.manual_led = {
              red: 0.5,
              green: 0,
              blue: 0,
              enable: 1,
            }
          }}
        />
        <Button
          intent={Intent.SUCCESS}
          writer={state => {
            state.manual_led = {
              red: 0,
              green: 0.5,
              blue: 0,
              enable: 1,
            }
          }}
        />
        <Button
          intent={Intent.PRIMARY}
          writer={state => {
            state.manual_led = {
              red: 0,
              green: 0,
              blue: 0.5,
              enable: 1,
            }
          }}
        />
      </Composition>
    </Composition>
  )
}

export default ManualLEDPalette
