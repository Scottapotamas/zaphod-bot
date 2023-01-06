import React from 'react'
import { Composition, Box } from 'atomic-layout'
import { SectionHeading } from '../SectionHeading'
import { IconNames } from '@blueprintjs/icons'
import { ButtonGroup, Intent, Tag } from '@blueprintjs/core'
import { useHardwareState } from '@electricui/components-core'
import { EXPANSION_MOTION_TYPES, MSGID } from 'src/application/typedState'
import { Button, NumberInput } from '@electricui/components-desktop-blueprint'

const MotionTypeSelector = () => {
  const motion_setting = useHardwareState(
    state => state[MSGID.USER_CONFIG].flags.expansion_type,
  )!

  return (
    <React.Fragment>
      <ButtonGroup>
        <Button
          active={motion_setting==EXPANSION_MOTION_TYPES.ROTARY_LIMITED}
          outlined
          fill
          intent={motion_setting ? Intent.NONE : Intent.SUCCESS}
          writer={state => {
            state[MSGID.USER_CONFIG].flags.expansion_type = EXPANSION_MOTION_TYPES.ROTARY_LIMITED
          }}
          icon={IconNames.SOCIAL_MEDIA}
        >
          ROTARY RESTRICTED
        </Button>
        <Button
          active={motion_setting==EXPANSION_MOTION_TYPES.ROTARY_UNLIMITED}
          outlined
          fill
          intent={motion_setting ? Intent.NONE : Intent.SUCCESS}
          writer={state => {
            state[MSGID.USER_CONFIG].flags.expansion_type = EXPANSION_MOTION_TYPES.ROTARY_UNLIMITED
          }}
          icon={IconNames.REFRESH}
        >
          ROTARY
        </Button>
        <Button
          active={motion_setting==EXPANSION_MOTION_TYPES.LINEAR}
          outlined
          fill
          intent={motion_setting ? Intent.SUCCESS : Intent.NONE}
          writer={state => {
            state[MSGID.USER_CONFIG].flags.expansion_type = EXPANSION_MOTION_TYPES.LINEAR
          }}
          icon={IconNames.SWAP_HORIZONTAL}
        >
          LINEAR
        </Button>
      </ButtonGroup>
    </React.Fragment>
  )
}

export const ApplicationConfig = () => {
  return (
    <Composition templateCols="1fr 1fr" justifyItems="center" gap={20}>
      <Box>
        <SectionHeading text="Motion Type" />
        <MotionTypeSelector />
      </Box>

      <Box>
        <SectionHeading text="Ratio" />

        {/* TODO: use correct setting variable */}
        <NumberInput
        accessor={state => state[MSGID.USER_CONFIG].values.speed_limit}
        writer={(state, value) => {
          state[MSGID.USER_CONFIG].values.speed_limit = value
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

      {/* Ability to define a minimum and maximum value for the range of motion, or unlimited */}
      
      {/* Flip the direction of travel */}

      

      </Box>
    </Composition>
  )
}
