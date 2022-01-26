import {
  incrementViewportFrameVersion,
  setSetting,
  VisualisationSettings,
  Store,
  getSetting,
} from './state'

import React, { useCallback } from 'react'

import { Vector3 } from 'three'
import {
  ColorPicker as ReactColorPicker,
  toColor,
  Color,
  useColor,
} from 'react-color-palette'
import { Classes, Popover2 } from '@blueprintjs/popover2'
import { Button } from '@blueprintjs/core'
import { RGB, RGBA } from '../optimiser/movements'

interface ColorPickerProps {
  defaultColor: RGB | RGBA
  writer: (rgba: RGBA) => void
  fill?: boolean
}

export function ColorPicker(props: ColorPickerProps) {
  const [col, setCol] = useColor('rgb', {
    r: Math.round(props.defaultColor[0] * 255),
    g: Math.round(props.defaultColor[1] * 255),
    b: Math.round(props.defaultColor[2] * 255),
    a: Math.round(props.defaultColor[3] ?? 1),
  })

  const updateColor = useCallback(
    (col: Color) => {
      props.writer([
        col.rgb.r / 255,
        col.rgb.g / 255,
        col.rgb.b / 255,
        col.rgb.a ?? 1,
      ])
    },
    [props.writer],
  )

  return (
    <Popover2
      minimal
      interactionKind="click"
      content={
        <div>
          <ReactColorPicker
            width={300}
            height={100}
            color={col}
            onChange={setCol}
            onChangeComplete={updateColor}
            hideHSV
            dark
          />
        </div>
      }
      renderTarget={({ isOpen, ref, ...targetProps }) => (
        <div
          {...targetProps}
          ref={ref}
          style={{
            background: `${col.hex}`,
            width: 30,
            height: 32,
            borderRadius: 5,
            boxShadow: `0 0 0 0 rgb(19 124 189 / 0%), 0 0 0 0 rgb(19 124 189 / 0%), 0 0 0 0 rgb(19 124 189 / 0%), inset 0 0 0 2px rgb(16 22 26 / 30%), inset 0 2px 2px rgb(16 22 26 / 40%)`,
          }}
        />
      )}
    />
  )
}
