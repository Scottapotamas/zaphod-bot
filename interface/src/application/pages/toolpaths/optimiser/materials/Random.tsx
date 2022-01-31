import { VisualisationSettings } from '../../interface/state'
import { Settings } from '../../optimiser/settings'
import { Movement, RGB } from './../movements'
import { Material } from './Base'
import { NodeID } from '../../interface/RenderableTree'
import React from 'react'
import { MATERIALS } from './utilities'
import { Vector3 } from 'three'

import { Composition, Box } from 'atomic-layout'
export interface RandomColorMaterialJSON {
  type: MATERIALS.RANDOM
  color: [number, number, number, number]
}

export const RandomColorMaterialDefaultJSON: RandomColorMaterialJSON = {
  type: MATERIALS.RANDOM,
  color: [1, 1, 1, 1],
}

export function isRandomColorMaterial(
  material: Material,
): material is RandomColorMaterial {
  return material.type === MATERIALS.RANDOM
}

export function importRandomColorMaterial(json: RandomColorMaterialJSON) {
  const mat = new RandomColorMaterial()

  return mat
}

function hex2rgb(hex: string): RGB {
  if (/^#/.test(hex)) {
    hex = hex.slice(1)
  }

  if (hex.length !== 3 && hex.length !== 6) {
    throw new Error('Invaild hex String')
  }

  let digit = hex.split('')

  if (digit.length === 3) {
    digit = [digit[0], digit[0], digit[1], digit[1], digit[2], digit[2]]
  }
  let r = parseInt([digit[0], digit[1]].join(''), 16) / 255
  let g = parseInt([digit[4], digit[5]].join(''), 16) / 255
  let b = parseInt([digit[2], digit[3]].join(''), 16) / 255
  return [r, g, b]
}

const colors = [
  hex2rgb('#2965CC'),
  hex2rgb('#29A634'),
  hex2rgb('#D99E0B'),
  hex2rgb('#D13913'),
  // hex2rgb('#8F398F'),
  hex2rgb('#00B3A4'),
  // hex2rgb('#DB2C6F'),
  hex2rgb('#9BBF30'),
  hex2rgb('#7157D9'),
  hex2rgb('#96622D'),
]

let counter = 0

const map = new Map()

function pickColor(id: string) {
  if (map.has(id)) return map.get(id)!

  counter++

  if (counter > colors.length - 1) {
    counter = 0
  }

  map.set(id, colors[counter])

  return colors[counter]
}

export class RandomColorMaterial extends Material {
  readonly type = MATERIALS.RANDOM

  public calculateColor = (
    movement: Movement,

    settings: Settings,
    visualisationSettings: VisualisationSettings,
    cameraPosition: Vector3,

    t: number,
  ): RGB => {
    return pickColor(movement.interFrameID)
  }
}

export interface RandomColorMaterialEditorProps {
  json: RandomColorMaterialJSON
  mutateJson: (writer: (json: RandomColorMaterialJSON) => void) => void
}

export function RandomColorMaterialEditor(
  props: RandomColorMaterialEditorProps,
) {
  return (
    <Composition templateCols="1fr 2fr" gap="1em" alignItems="center">
      Random Colors!
    </Composition>
  )
}
