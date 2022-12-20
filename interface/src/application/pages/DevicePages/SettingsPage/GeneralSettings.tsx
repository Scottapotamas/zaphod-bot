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
import { IconNames } from '@blueprintjs/icons'

export const GeneralSettings = () => {
  return (
    <Composition templateCols="1fr 1fr" justifyItems="start" gap={10}>
      <Box>
        <h2>Operations</h2>
        <Checkbox
          checked={true}
          unchecked={false}
          accessor={state => state[MSGID.USER_CONFIG].mute_buzzer}
          writer={(state, value) => {
            state[MSGID.USER_CONFIG].mute_buzzer = value
          }}
        >
          Aerial mounting
        </Checkbox>

        {/* Dropdown for geometry warning behaviours */}
        <Dropdown
          accessor={state => state[MSGID.USER_CONFIG].mute_buzzer}
          writer={(state, value) => {
            state[MSGID.USER_CONFIG].mute_buzzer = value
          }}
          placeholder={() => 'Boundary Checks'}
        >
          <Dropdown.Option value={0} text="Disabled" icon={IconNames.CROSS} />
          <Dropdown.Option
            value={1}
            text="Notify"
            icon={IconNames.NOTIFICATIONS_UPDATED}
            intent={Intent.PRIMARY}
          />
          <Dropdown.Option
            value={2}
            text="Halt"
            icon={IconNames.WALK}
            intent={Intent.WARNING}
          />
          <Dropdown.Option
            value={3}
            text="Disarm"
            icon={IconNames.ERROR}
            intent={Intent.DANGER}
          />
        </Dropdown>
      </Box>

      <Box>
        <h2>Handling</h2>
        <Checkbox
          checked={true}
          unchecked={false}
          accessor={state => state[MSGID.USER_CONFIG].mute_buzzer}
          writer={(state, value) => {
            state[MSGID.USER_CONFIG].mute_buzzer = value
          }}
        >
          E-STOP Pendant Optional
        </Checkbox>
        <Checkbox
          checked={true}
          unchecked={false}
          accessor={state => state[MSGID.USER_CONFIG].mute_buzzer}
          writer={(state, value) => {
            state[MSGID.USER_CONFIG].mute_buzzer = value
          }}
        >
          Require E-STOP verification during arming
        </Checkbox>
        <Checkbox
          checked={true}
          unchecked={false}
          accessor={state => state[MSGID.USER_CONFIG].mute_buzzer}
          writer={(state, value) => {
            state[MSGID.USER_CONFIG].mute_buzzer = value
          }}
        >
          E-STOP armed status light
        </Checkbox>

        <br />
        <Checkbox
          checked={true}
          unchecked={false}
          accessor={state => state[MSGID.USER_CONFIG].mute_buzzer}
          writer={(state, value) => {
            state[MSGID.USER_CONFIG].mute_buzzer = value
          }}
        >
          Use effector light for notifications
        </Checkbox>

        <Checkbox
          checked={true}
          unchecked={false}
          accessor={state => state[MSGID.USER_CONFIG].mute_buzzer}
          writer={(state, value) => {
            state[MSGID.USER_CONFIG].mute_buzzer = value
          }}
        >
          Mute Buzzer
        </Checkbox>

      </Box>
      
      <Box>
        <h2>Speed Limits</h2>

      <NumberInput
            accessor={state => state[MSGID.USER_CONFIG].restrict.y}
            writer={(state, value) => {
              state[MSGID.USER_CONFIG].restrict.y = value
            }}
            min={0}
            max={300}
            intent={Intent.NONE}
            rightElement={
              <Tag minimal>
                <b>mm/sec</b>
              </Tag>
            }
            style={{ width: '150px' }}
          />
      </Box>

      <Box>
        <h2>Z-axis rotation offset</h2>
        <Slider
          min={0}
          max={360}
          labelStepSize={90}
          stepSize={15}
          sendOnlyOnRelease
          labelRenderer={val => `${val}º`}
        >
          <Slider.Handle accessor="rotZ" />
        </Slider>
      </Box>

     

      <Box>
        <h2>Restrict Volume</h2>
        <Composition templateCols="1fr" gap={5}>
          <NumberInput
            accessor={state => state[MSGID.USER_CONFIG].restrict.x}
            writer={(state, value) => {
              state[MSGID.USER_CONFIG].restrict.x = value
            }}
            min={0}
            max={300}
            intent={Intent.DANGER}
            rightElement={
              <Tag minimal>
                <b>X</b>
              </Tag>
            }
          />

          <NumberInput
            accessor={state => state[MSGID.USER_CONFIG].restrict.y}
            writer={(state, value) => {
              state[MSGID.USER_CONFIG].restrict.y = value
            }}
            min={0}
            max={300}
            intent={Intent.SUCCESS}
            rightElement={
              <Tag minimal>
                <b>Y</b>
              </Tag>
            }
          />
          <Box>
            <NumberInput
              accessor={state => state[MSGID.USER_CONFIG].restrict.z}
              writer={(state, value) => {
                state[MSGID.USER_CONFIG].restrict.z = value
              }}
              min={0}
              max={300}
              intent={Intent.PRIMARY}
              rightElement={
                <Tag minimal>
                  <b>Z</b>
                </Tag>
              }
            />
          </Box>

        </Composition>
      </Box>




      
    </Composition>
  )
}
