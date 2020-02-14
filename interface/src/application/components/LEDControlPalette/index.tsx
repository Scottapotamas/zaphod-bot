import React from 'react'

import { Slider, Switch } from '@electricui/components-desktop-blueprint'

const LEDControlPalette = () => {
  return (
    <React.Fragment>
      <Slider
        showTrackFill={false}
        writer={(state, values) => (state.hsv.hue = values.hue_handle)}
        min={0}
        max={1}
        stepSize={0.025}
        labelStepSize={0.25}
        labelRenderer={val => `${Math.round(val * 360)}º`}
      >
        <Slider.Handle name="hue_handle" accessor={state => state.hsv.hue} />
      </Slider>
      <br />
      <Slider
        showTrackFill={false}
        writer={(state, values) => (state.hsv.saturation = values.sat_handle)}
        min={0}
        max={1}
        stepSize={0.025}
        labelStepSize={0.25}
        labelRenderer={val => `${Math.round(val * 100)}%`}
      >
        <Slider.Handle
          name="sat_handle"
          accessor={state => state.hsv.saturation}
        />
      </Slider>
      <br />
      <Slider
        showTrackFill={false}
        writer={(state, values) =>
          (state.hsv.lightness = values.intensity_handle)
        }
        min={0}
        max={1}
        stepSize={0.025}
        labelStepSize={0.25}
        labelRenderer={val => `${Math.round(val * 100)}%`}
      >
        <Slider.Handle
          name="intensity_handle"
          accessor={state => state.hsv.lightness}
        />
      </Slider>
      <br />
      <div style={{ width: '200px', margin: '0 auto' }}>
        <Switch
          large
          innerLabel="OFF"
          innerLabelChecked="ON"
          unchecked={0}
          checked={1}
          accessor={state => state.hsv.enable}
          writer={(state, value) => (state.hsv.enable = value)}
        >
          Enable LED
        </Switch>
      </div>
    </React.Fragment>
  )
}

export default LEDControlPalette
