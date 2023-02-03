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
} from '@electricui/components-desktop-blueprint'
import { Printer } from '@electricui/components-desktop'
import { MSGID } from 'src/application/typedState'
import { SectionHeading } from './SectionHeading'

export const LEDCalibrationBlock = () => {
  return (
    <Composition templateCols="1fr 1fr" gap={20}>
      <Box>
        <Checkbox
          checked={true}
          unchecked={false}
          accessor={state => state[MSGID.LED_CALIBRATION].correct_luma}
          writer={(state, value) => {
            state[MSGID.LED_CALIBRATION].correct_luma = value
          }}
        >
          Luminance Curve Correction
        </Checkbox>
        <Checkbox
          checked={true}
          unchecked={false}
          accessor={state => state[MSGID.LED_CALIBRATION].correct_whitebalance}
          writer={(state, value) => {
            state[MSGID.LED_CALIBRATION].correct_whitebalance = value
          }}
        >
          Whitebalance Correction
        </Checkbox>
      </Box>
      <Box>
        {/* Something goes here? */}
      </Box>

      <Box>
        <SectionHeading text="Whitebalance Offset" />
        <Composition gap={10}>
          <NumberInput
            accessor={state => state[MSGID.LED_CALIBRATION].offset_red * -100}
            writer={(state, value) => {
              state[MSGID.LED_CALIBRATION].offset_red = value / -100
            }}
            throttleDuration={100}
            min={-100}
            max={0}
            fill
            intent={Intent.DANGER}
            rightElement={
              <Tag minimal>
                % RED
              </Tag>
            }
          />
          <NumberInput
            accessor={state => state[MSGID.LED_CALIBRATION].offset_green * -100}
            writer={(state, value) => {
              state[MSGID.LED_CALIBRATION].offset_green = value / -100
            }}
            throttleDuration={100}
            min={-100}
            max={0}
            fill
            intent={Intent.SUCCESS}
            rightElement={
              <Tag minimal>
                % GREEN
              </Tag>
            }
          />
          <NumberInput
            accessor={state => state[MSGID.LED_CALIBRATION].offset_blue * -100}
            writer={(state, value) => {
              state[MSGID.LED_CALIBRATION].offset_blue = value / -100
            }}
            throttleDuration={100}
            min={-100}
            max={0}
            fill
            intent={Intent.PRIMARY}
            rightElement={
              <Tag minimal>
                % BLUE
              </Tag>
            }
          />
        </Composition>
      </Box>

      <Box>
        <SectionHeading text="Whitebalance Check" />
        <ButtonGroup fill>
          <Button
            outlined
            writer={state => {
              state[MSGID.LED_MANUAL_REQUEST] = {
                red: 0,
                green: 0,
                blue: 0,
                enable: true,
              }
            }}
          >
            OFF
          </Button>

          <Button
            outlined
            writer={state => {
              state[MSGID.LED_MANUAL_REQUEST] = {
                red: 0.1,
                green: 0.1,
                blue: 0.1,
                enable: true,
              }
            }}
          >
            10%
          </Button>
          <Button
            outlined
            writer={state => {
              state[MSGID.LED_MANUAL_REQUEST] = {
                red: 0.5,
                green: 0.5,
                blue: 0.5,
                enable: true,
              }
            }}
          >
            50%
          </Button>
          <Button
            outlined
            writer={state => {
              state[MSGID.LED_MANUAL_REQUEST] = {
                red: 1.0,
                green: 1.0,
                blue: 1.0,
                enable: true,
              }
            }}
          >
            100%
          </Button>
        </ButtonGroup>
        <br />
        <SectionHeading text="Colour Check" />
        <ButtonGroup fill>
          <Button
            outlined
            writer={state => {
              state[MSGID.LED_MANUAL_REQUEST] = {
                red: 0,
                green: 0,
                blue: 0,
                enable: true,
              }
            }}
          >
            OFF
          </Button>

          <Button
            outlined
            intent={Intent.DANGER}
            writer={state => {
              state[MSGID.LED_MANUAL_REQUEST] = {
                red: 0.5,
                green: 0,
                blue: 0,
                enable: true,
              }
            }}
          >
            RED
          </Button>
          <Button
            outlined
            intent={Intent.SUCCESS}
            writer={state => {
              state[MSGID.LED_MANUAL_REQUEST] = {
                red: 0,
                green: 0.5,
                blue: 0,
                enable: true,
              }
            }}
          >
            GREEN
          </Button>
          <Button
            outlined
            intent={Intent.PRIMARY}
            writer={state => {
              state[MSGID.LED_MANUAL_REQUEST] = {
                red: 0,
                green: 0,
                blue: 0.5,
                enable: true,
              }
            }}
          >
            BLUE
          </Button>
        </ButtonGroup>
      </Box>
    </Composition>
  )
}
