import React from 'react'

import { Switch } from '@electricui/components-desktop-blueprint'
import {
  ColorPalette,
  HueSlider,
  AlphaSlider,
} from '@electricui/components-desktop'

const LEDControlPalette = () => {
  return (
    <React.Fragment>
      <div style={{ width: '200px', margin: '0 auto' }}>
        <Switch
          large
          innerLabel="OFF"
          innerLabelChecked="ON"
          unchecked={0}
          checked={1}
          accessor={state => state.manual_led.enable}
          writer={(state, value) => (state.manual_led.enable = value)}
        >
          Enable LED
        </Switch>
      </div>

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
        // vertical
        red={state => state.manual_led.red}
        green={state => state.manual_led.green}
        blue={state => state.manual_led.blue}
        writer={(state, color) => {
          state.manual_led.red = color.r
          state.manual_led.green = color.g
          state.manual_led.blue = color.b
        }}
      />
    </React.Fragment>
  )
}

export default LEDControlPalette
