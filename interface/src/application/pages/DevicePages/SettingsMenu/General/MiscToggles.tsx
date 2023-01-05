import React from 'react'
import { Composition, Box } from 'atomic-layout'
import { Checkbox } from '@electricui/components-desktop-blueprint'
import { MSGID } from 'src/application/typedState'
import { IconNames } from '@blueprintjs/icons'

import { SectionHeading } from '../SectionHeading'

export const MiscToggles = () => {
  return (
    <React.Fragment>
      <SectionHeading text="HANDLING" />

      <Checkbox
        checked={true}
        unchecked={false}
        accessor={state => state[MSGID.USER_CONFIG].flags.pendant_optional}
        writer={(state, value) => {
          state[MSGID.USER_CONFIG].flags.pendant_optional = value
        }}
      >
        E-STOP Pendant Optional
      </Checkbox>
      <Checkbox
        checked={true}
        unchecked={false}
        accessor={state => state[MSGID.USER_CONFIG].flags.pendant_verify_on_arm}
        writer={(state, value) => {
          state[MSGID.USER_CONFIG].flags.pendant_verify_on_arm = value
        }}
      >
        Require E-STOP verification during arming
      </Checkbox>
      <Checkbox
        checked={true}
        unchecked={false}
        accessor={state => state[MSGID.USER_CONFIG].flags.pendant_light_enabled}
        writer={(state, value) => {
          state[MSGID.USER_CONFIG].flags.pendant_light_enabled = value
        }}
      >
        E-STOP armed status light
      </Checkbox>

      <br />
      <Checkbox
        checked={true}
        unchecked={false}
        accessor={state =>
          state[MSGID.USER_CONFIG].flags.effector_as_status_led
        }
        writer={(state, value) => {
          state[MSGID.USER_CONFIG].flags.effector_as_status_led = value
        }}
      >
        Use effector light for notifications
      </Checkbox>

      <Checkbox
        checked={true}
        unchecked={false}
        accessor={state => state[MSGID.USER_CONFIG].flags.buzzer_mute}
        writer={(state, value) => {
          state[MSGID.USER_CONFIG].flags.buzzer_mute = value
        }}
      >
        Mute Buzzer
      </Checkbox>
    </React.Fragment>
  )
}
