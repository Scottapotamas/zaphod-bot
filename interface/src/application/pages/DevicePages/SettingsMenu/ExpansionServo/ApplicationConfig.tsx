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
          intent={(motion_setting==EXPANSION_MOTION_TYPES.ROTARY_LIMITED) ? Intent.SUCCESS : Intent.NONE}
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
          intent={(motion_setting==EXPANSION_MOTION_TYPES.ROTARY_UNLIMITED) ? Intent.SUCCESS : Intent.NONE}
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
          intent={(motion_setting==EXPANSION_MOTION_TYPES.LINEAR) ? Intent.SUCCESS : Intent.NONE}
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
        <br/>
        <br/>
        {/* Ability to define a minimum and maximum value for the range of motion, or unlimited */}
        <Composition templateCols="1fr 1fr" gapCol={20} >
          <NumberInput
            accessor={state => state[MSGID.USER_CONFIG].values.expansion_range_min}
            writer={(state, value) => {
              state[MSGID.USER_CONFIG].values.expansion_range_min = value
            }}
            intent={Intent.PRIMARY}
            min={-400}
            max={400}
            rightElement={
              <Tag minimal>
                <b>MIN</b>
              </Tag>
            }
            style={{maxWidth: '125px'}}
          />
          <NumberInput
            accessor={state => state[MSGID.USER_CONFIG].values.expansion_range_max}
            writer={(state, value) => {
              state[MSGID.USER_CONFIG].values.expansion_range_max = value
            }}
            intent={Intent.PRIMARY}
            min={-400}
            max={400}
            rightElement={
              <Tag minimal>
                <b>MAX</b>
              </Tag>
            }
            style={{maxWidth: '125px'}}
          />
        </Composition>

        <br/>

        {/* Maximum speed of output stage */}
        {/* TODO: modify label based on type of mode selected */}
        <NumberInput
          accessor={state => state[MSGID.USER_CONFIG].values.expansion_speed_limit}
          writer={(state, value) => {
            state[MSGID.USER_CONFIG].values.expansion_speed_limit = value
          }}
          fill
          intent={Intent.PRIMARY}
          min={0}
          max={400}
          rightElement={
            <Tag minimal>
              <b>units/second</b>
            </Tag>
          }
        />

      </Box>

      <Box>
        <SectionHeading text="Ratio" />

        {/* TODO: use correct setting variable */}
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

      </Box>
    </Composition>
  )
}
