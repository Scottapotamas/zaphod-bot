import {
  Alignment,
  Button as BlueprintButton,
  ButtonGroup,
  Navbar,
  Popover,
  Position,
  Tooltip,
  HTMLTable,
  Intent,
  Tag,
} from '@blueprintjs/core'
import {
  IntervalRequester,
  useDeviceConnect,
  useDeviceConnectionRequested,
  useDeviceDisconnect,
  useHardwareState,
} from '@electricui/components-core'

import React from 'react'
import { Composition, Box } from 'atomic-layout'
import {
  Statistic,
  Button,
  NumberInput,
  Slider,
  Switch,
  Checkbox,
  Dropdown,
  RadioGroup,
} from '@electricui/components-desktop-blueprint'
import { Printer } from '@electricui/components-desktop'
import { MSGID, ServoInfo } from 'src/application/typedState'
import { IconNames, IconName } from '@blueprintjs/icons'

type BoundaryButtonProps = {
  text: string
  icon: IconName
  value: number
}

const boundarySettings: BoundaryButtonProps[] = [
  {
    value: 0,
    text: 'Disabled',
    icon: IconNames.CROSS,
  },
  {
    value: 1,
    text: 'Notify',
    icon: IconNames.NOTIFICATIONS_UPDATED,
  },
  {
    value: 2,
    text: 'Halt',
    icon: IconNames.WALK,
  },
  {
    value: 3,
    text: 'Disarm',
    icon: IconNames.ERROR,
  },
]

export const GeneralSettings = () => {
  const boundary_setting = useHardwareState(
    state => state[MSGID.USER_CONFIG].flags.boundary_violation_mode,
  )

  const inverted_setting = useHardwareState(
    state => state[MSGID.USER_CONFIG].flags.inverted,
  )!

  return (
    <Composition templateCols="auto 1fr" justifyItems="center" gap={20}>
      <Box>
        <h2
          style={{ textAlign: 'center', fontWeight: 'initial', opacity: '0.8' }}
        >
          ORIENTATION
        </h2>
        <ButtonGroup>
          <Button
            active={!inverted_setting}
            outlined
            fill
            intent={inverted_setting ? Intent.NONE : Intent.SUCCESS}
            writer={state => {
              state[MSGID.USER_CONFIG].flags.inverted = false
            }}
            icon={IconNames.CARET_UP}
          >
            NORMAL
          </Button>
          <Button
            active={inverted_setting}
            outlined
            fill
            intent={inverted_setting ? Intent.SUCCESS : Intent.NONE}
            writer={state => {
              state[MSGID.USER_CONFIG].flags.inverted = true
            }}
            icon={IconNames.CARET_DOWN}
          >
            INVERTED
          </Button>
        </ButtonGroup>
      </Box>

      {/* Settings for geometry warning behaviours */}
      <Box>
        <h2
          style={{ textAlign: 'center', fontWeight: 'initial', opacity: '0.8' }}
        >
          BOUNDARY VIOLATIONS
        </h2>

        <ButtonGroup>
          {boundarySettings.map((option, index) => (
            <Button
              key={index}
              active={boundary_setting == option.value}
              outlined
              fill
              intent={
                boundary_setting == option.value ? Intent.WARNING : Intent.NONE
              }
              writer={state => {
                state[MSGID.USER_CONFIG].flags.boundary_violation_mode =
                  option.value
              }}
              icon={option.icon}
            >
              {option.text.toLocaleUpperCase()}
            </Button>
          ))}
        </ButtonGroup>
      </Box>

      <Box>
        <h2
          style={{ textAlign: 'center', fontWeight: 'initial', opacity: '0.8' }}
        >
          HANDLING
        </h2>

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
          accessor={state =>
            state[MSGID.USER_CONFIG].flags.pendant_verify_on_arm
          }
          writer={(state, value) => {
            state[MSGID.USER_CONFIG].flags.pendant_verify_on_arm = value
          }}
        >
          Require E-STOP verification during arming
        </Checkbox>
        <Checkbox
          checked={true}
          unchecked={false}
          accessor={state =>
            state[MSGID.USER_CONFIG].flags.pendant_light_enabled
          }
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
      </Box>

      <Box>
        <h2
          style={{ textAlign: 'center', fontWeight: 'initial', opacity: '0.8' }}
        >
          SPEED LIMIT
        </h2>
        <NumberInput
          accessor={state => state[MSGID.USER_CONFIG].values.speed_limit}
          writer={(state, value) => {
            state[MSGID.USER_CONFIG].values.speed_limit = value
          }}
          min={0}
          max={750}
          intent={Intent.DANGER}
          rightElement={
            <Tag minimal>
              <b>mm/sec</b>
            </Tag>
          }
        />
        <br />
        <h2
          style={{ textAlign: 'center', fontWeight: 'initial', opacity: '0.8' }}
        >
          RESTRICT VOLUME
        </h2>
        <Composition templateCols="1fr" gap={5}>
          <NumberInput
            accessor={state => state[MSGID.USER_CONFIG].values.volume_radius}
            writer={(state, value) => {
              state[MSGID.USER_CONFIG].values.volume_radius = value
            }}
            min={0}
            max={300}
            intent={Intent.WARNING}
            rightElement={
              <Tag minimal>
                <b>RADIUS</b>
              </Tag>
            }
          />

          <NumberInput
            accessor={state => state[MSGID.USER_CONFIG].values.volume_z}
            writer={(state, value) => {
              state[MSGID.USER_CONFIG].values.volume_z = value
            }}
            min={0}
            max={300}
            intent={Intent.PRIMARY}
            rightElement={
              <Tag minimal>
                <b>HEIGHT</b>
              </Tag>
            }
          />
        </Composition>
      </Box>

      <Box>
        {/* Something else goes here? */}
      </Box>

      <Box style={{ minWidth: '250px' }}>
        <h2
          style={{ textAlign: 'center', fontWeight: 'initial', opacity: '0.8' }}
        >
          Z-AXIS OFFSET
        </h2>
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
      </Box>
    </Composition>
  )
}
