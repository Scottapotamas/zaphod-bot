import React from 'react'
import { Slider } from '@electricui/components-desktop-blueprint'
import { MSGID } from 'src/application/typedState'
import { SectionHeading } from '../SectionHeading'

export const ZAxisOffset = () => {
  return (
    <React.Fragment>
      <SectionHeading text="Z-AXIS OFFSET" />
      <Slider
        writer={(state, values) => {
          state[MSGID.USER_CONFIG].values.z_rotation =
            values.slider_handle_z_offset
        }}
        min={0}
        max={360}
        labelStepSize={90}
        stepSize={15}
        sendOnlyOnRelease
        labelRenderer={val => `${val}º`}
      >
        <Slider.Handle
          name="slider_handle_z_offset"
          accessor={state => state[MSGID.USER_CONFIG].values.z_rotation}
        />
      </Slider>
    </React.Fragment>
  )
}
