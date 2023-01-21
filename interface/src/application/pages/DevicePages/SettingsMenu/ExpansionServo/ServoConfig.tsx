import React from 'react'
import { Composition, Box } from 'atomic-layout'
import { SectionHeading } from '../SectionHeading'
import { Checkbox, NumberInput } from '@electricui/components-desktop-blueprint'
import { MSGID } from 'src/application/typedState'
import { Intent, Tag } from '@blueprintjs/core'

export const ServoConfig = () => {
  return (
    <Composition templateCols="1fr 1fr" justifyItems="center" gap={20}>
      <Box>
        <SectionHeading text="Servo Configuration" />

        <Checkbox
        checked={true}
        unchecked={false}
        accessor={state => state[MSGID.USER_CONFIG].flags.expansion_enabled}
        writer={(state, value) => {
          state[MSGID.USER_CONFIG].flags.expansion_enabled = value
        }}
      >
        Use expansion servo
      </Checkbox>

        <Checkbox
        checked={true}
        unchecked={false}
        accessor={state => state[MSGID.USER_CONFIG].flags.expansion_requires_homing}
        writer={(state, value) => {
          state[MSGID.USER_CONFIG].flags.expansion_requires_homing = value
        }}
      >
        Home on startup
      </Checkbox>

      <Checkbox
        checked={true}
        unchecked={false}
        accessor={state => state[MSGID.USER_CONFIG].flags.expansion_reverse}
        writer={(state, value) => {
          state[MSGID.USER_CONFIG].flags.expansion_reverse = value
        }}
      >
        Reverse Direction
      </Checkbox>

      <NumberInput
          accessor={state => state[MSGID.USER_CONFIG].values.expansion_resolution}
          writer={(state, value) => {
            state[MSGID.USER_CONFIG].values.expansion_resolution = value
          }}
          intent={Intent.PRIMARY}
          min={0}
          max={6400}
          rightElement={
            <Tag minimal>
              <b>STEPS/REV</b>
            </Tag>
          }
        />

      </Box>
      <Box>
        <SectionHeading text="" />
        

      </Box>
    </Composition>
  )
}
