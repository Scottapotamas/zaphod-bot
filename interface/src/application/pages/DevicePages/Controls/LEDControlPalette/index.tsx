import React from 'react'

import { Switch } from '@electricui/components-desktop-blueprint'
import {
  ColorPalette,
  HueSlider,
  AlphaSlider,
} from '@electricui/components-desktop'
import { Composition, Box } from 'atomic-layout'

export const LEDControlPalette = () => {
  return (
    <React.Fragment>
      <Composition templateCols="auto auto">
        <Switch
          large
          innerLabel="OFF"
          innerLabelChecked="ON"
          unchecked={0}
          checked={1}
          accessor={state => state.manual_led.enable}
          writer={(state, value) => (state.manual_led.enable = value)}
        >
        </Switch>
        <Box>
          <ColorPalette
            red={state => state.manual_led.red}
            green={state => state.manual_led.green}
            blue={state => state.manual_led.blue}
            writer={(state, color) => {
              state.manual_led.red = color.r
              state.manual_led.green = color.g
              state.manual_led.blue = color.b
            }}
          />
          <HueSlider
            red={state => state.manual_led.red}
            green={state => state.manual_led.green}
            blue={state => state.manual_led.blue}
            writer={(state, color) => {
              state.manual_led.red = color.r
              state.manual_led.green = color.g
              state.manual_led.blue = color.b
            }}
          />
        </Box>
      </Composition>
    </React.Fragment>
  )
}
