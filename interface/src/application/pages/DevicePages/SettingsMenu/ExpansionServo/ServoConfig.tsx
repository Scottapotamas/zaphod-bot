import React from 'react'
import { Composition, Box } from 'atomic-layout'
import { SectionHeading } from '../SectionHeading'
import {
  Checkbox,
  Dropdown,
  NumberInput,
} from '@electricui/components-desktop-blueprint'
import { MSGID } from 'src/application/typedState'
import { Intent, Tag } from '@blueprintjs/core'

export const ServoConfig = () => {
  return (
    <Composition templateCols="1fr" gap={10}>
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
          accessor={state =>
            state[MSGID.USER_CONFIG].flags.expansion_requires_homing
          }
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

        <Dropdown
          placeholder={selectedOption =>
            selectedOption
              ? `Steps/Revolution: ${selectedOption.text}`
              : 'Specify Steps/Revolution'
          }
          accessor={state =>
            state[MSGID.USER_CONFIG].values.expansion_resolution
          }
          writer={(state, value) => {
            state[MSGID.USER_CONFIG].values.expansion_resolution = value
          }}
        >
          <Dropdown.Option value={0} text="None" />
          <Dropdown.Option value={1} text="100" />
          <Dropdown.Option value={2} text="200" />
          <Dropdown.Option value={4} text="400" />
          <Dropdown.Option value={8} text="800" />
          <Dropdown.Option value={16} text="1600" />
          <Dropdown.Option value={32} text="3200" />
          <Dropdown.Option value={64} text="6400" />
        </Dropdown>

        <NumberInput
        accessor={state => state[MSGID.USER_CONFIG].values.expansion_ratio}
        writer={(state, value) => {
          state[MSGID.USER_CONFIG].values.expansion_ratio = value
        }}
        min={0}
        max={1}
        intent={Intent.PRIMARY}
        rightElement={
          <Tag minimal>
            <b>RATIO</b>
          </Tag>
        }
      />
    </Composition>
  )
}
