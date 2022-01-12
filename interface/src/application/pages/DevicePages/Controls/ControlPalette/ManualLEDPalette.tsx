import React, { useState } from 'react'

import {
  Icon,
  MultiSlider,
  NumericInput,
  Tag,
  Intent,
  Alignment,
} from '@blueprintjs/core'
import { Composition, Box } from 'atomic-layout'

import {
  Button,
  NumberInput,
  Switch,
} from '@electricui/components-desktop-blueprint'
import {
  ColorPalette,
  HueSlider,
  AlphaSlider,
} from '@electricui/components-desktop'

import { MSGID } from '../../../../typedState'

const ManualLEDPalette = () => {
  return (
    <Composition
      gap={10}
      templateCols="1fr auto"
      alignItems="center"
      justifyItems="center"
    >
      <Box>
        <ColorPalette
          height={200}
          width={200}
          red={state => state[MSGID.LED_MANUAL_REQUEST].red * 255}
          green={state => state[MSGID.LED_MANUAL_REQUEST].green * 255}
          blue={state => state[MSGID.LED_MANUAL_REQUEST].blue * 255}
          writer={(state, color) => {
            state[MSGID.LED_MANUAL_REQUEST].red = color.r / 255
            state[MSGID.LED_MANUAL_REQUEST].green = color.g / 255
            state[MSGID.LED_MANUAL_REQUEST].blue = color.b / 255
          }}
        />
        <HueSlider
          vertical
          height={200}
          red={state => state[MSGID.LED_MANUAL_REQUEST].red * 255}
          green={state => state[MSGID.LED_MANUAL_REQUEST].green * 255}
          blue={state => state[MSGID.LED_MANUAL_REQUEST].blue * 255}
          writer={(state, color) => {
            state[MSGID.LED_MANUAL_REQUEST].red = color.r / 255
            state[MSGID.LED_MANUAL_REQUEST].green = color.g / 255
            state[MSGID.LED_MANUAL_REQUEST].blue = color.b / 255
          }}
        />
      </Box>
      <Composition gap={10} templateCols="1fr" justifyItems="center">
        <b>LED CONTROL</b>
        <Switch
          large
          innerLabel="OFF"
          innerLabelChecked="ON"
          unchecked={false}
          checked={true}
          accessor={state => state[MSGID.LED_MANUAL_REQUEST].enable}
          writer={(state, value) => {
            state[MSGID.LED_MANUAL_REQUEST].enable = value
          }}
        ></Switch>
        <Button
          intent={Intent.DANGER}
          writer={state => {
            state[MSGID.LED_MANUAL_REQUEST] = {
              red: 0.5,
              green: 0,
              blue: 0,
              enable: true,
            }
          }}
        />
        <Button
          intent={Intent.SUCCESS}
          writer={state => {
            state[MSGID.LED_MANUAL_REQUEST] = {
              red: 0,
              green: 0.5,
              blue: 0,
              enable: true,
            }
          }}
        />
        <Button
          intent={Intent.PRIMARY}
          writer={state => {
            state[MSGID.LED_MANUAL_REQUEST] = {
              red: 0,
              green: 0,
              blue: 0.5,
              enable: true,
            }
          }}
        />
      </Composition>
    </Composition>
  )
}

export default ManualLEDPalette
