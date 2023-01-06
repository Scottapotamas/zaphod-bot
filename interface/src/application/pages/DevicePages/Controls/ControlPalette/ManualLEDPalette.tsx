import React, { useState } from 'react'

import {
  Icon,
  MultiSlider,
  NumericInput,
  Tag,
  Intent,
  Alignment,
  Colors,
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
import { IconNames } from '@blueprintjs/icons'

type SwatchProps = {
  name: string
  red?: number
  green?: number
  blue?: number
  hex?: string
}

const colorPresets: SwatchProps[] = [
  // { name: 'Blue', red: 0, green: 0, blue: 1.0 },
  // { name: 'Red', red: 1, green: 0, blue: 0 },
  // { name: 'Green', red: 0, green: 1, blue: 0 },

  { name: 'Grey', hex: Colors.GRAY1 },
  { name: 'Light Grey', hex: Colors.LIGHT_GRAY1 },
  // { name: 'White', hex: Colors.WHITE },
  { name: 'Vermilion', hex: Colors.VERMILION1 },
  { name: 'Red', hex: Colors.RED1 },
  { name: 'Rose', hex: Colors.ROSE1 },
  { name: 'Violet', hex: Colors.VIOLET1 },
  { name: 'Indigo', hex: Colors.INDIGO1 },
  { name: 'Cerulean', hex: Colors.CERULEAN1 },
  { name: 'Blue', hex: Colors.BLUE1 },
  { name: 'Turquoise', hex: Colors.TURQUOISE1 },
  { name: 'Forest', hex: Colors.FOREST1 },
  { name: 'Green', hex: Colors.GREEN1 },
  { name: 'Lime', hex: Colors.LIME1 },
  { name: 'Gold', hex: Colors.GOLD1 },
  { name: 'Orange', hex: Colors.ORANGE1 },
  { name: 'Sepia', hex: Colors.SEPIA1 },
]

function hexToRGB(hex: string) {
  return hex
    .replace(
      /^#?([a-f\d])([a-f\d])([a-f\d])$/i,
      (m, r, g, b) => '#' + r + r + g + g + b + b,
    )!
    .substring(1)!
    .match(/.{2}/g)!
    .map(x => parseInt(x, 16))
}

const ManualLEDPalette = () => {
  return (
    <Composition templateCols="repeat(4, 1fr)" gap={10} padding={10}>
      <Button
        key="led_off"
        // active={selected_demo == attractor.seqNum}
        outlined
        fill
        // intent={Intent.NONE}
        icon={IconNames.CROSS}
        writer={state => {
          state[MSGID.LED_MANUAL_REQUEST].enable = false
        }}
      >
        OFF
      </Button>

      <Box
        style={{
          gridColumnEnd: 'span 3',
        }}
      >
        <HueSlider
          height={30}
          width={325}
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
      
      {colorPresets.map((swatch, index) => (
        <Button
          key={'led' + index}
          // active={selected_demo == attractor.seqNum}
          outlined
          fill
          small
          writer={state => {
            let rgb: number[] = [swatch.red!, swatch.green!, swatch.blue!]

            if (swatch.hex) {
              rgb = hexToRGB(swatch.hex)

              // TODO: cleanup/remap colours to look better by eye?
              rgb[0] = rgb[0] / 255
              rgb[1] = rgb[1] / 255
              rgb[2] = rgb[2] / 255
            }

            state[MSGID.LED_MANUAL_REQUEST] = {
              red: rgb[0],
              green: rgb[1],
              blue: rgb[2],
              enable: true,
            }
          }}
          style={{
            borderColor: swatch.hex ? `${swatch.hex}` : '',
            color: swatch.hex ? `${swatch.hex}` : '',
            // background: (swatch.hex)?`${swatch.hex}`:"",
          }}
        >
          <b>{swatch.name.toUpperCase()}</b>
        </Button>
      ))}
    </Composition>
  )
}

export default ManualLEDPalette
