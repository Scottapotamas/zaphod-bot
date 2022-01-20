import { MathUtils, Vector3 } from 'three'
import { VisualisationSettings } from '../../interface/state'
import { Settings } from '../../optimiser/settings'
import { PlannerLightMove, LightMoveType } from './../hardware'
import {
  AddComponentCallback,
  AddLineCallback,
  isPointTransition,
  isTransition,
  Movement,
  MOVEMENT_TYPE,
  RGB,
} from './../movements'
import { Material } from './Base'
import { Html } from '@react-three/drei'
import { Colors, Intent, Tag } from '@blueprintjs/core'
import { NodeID } from '../../interface/RenderableTree'
import React from 'react'
import { annotateDrawOrder, lerpRGB, MATERIALS, rgbToHsi } from './utilities'

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
  hex2rgb('#DB2C6F'),
  hex2rgb('#9BBF30'),
  hex2rgb('#96622D'),
]

let counter = 0

function pickColor() {
  counter++

  if (counter > colors.length - 1) {
    counter = 0
  }

  return colors[counter]
}

export class RandomColorMaterial extends Material {
  readonly type = MATERIALS.RANDOM

  public color: RGB

  constructor() {
    super()
    this.color = pickColor()
  }

  public generateLightpath = (movement: Movement) => {
    const fade: PlannerLightMove = {
      duration: movement.getDuration(),
      type: LightMoveType.IMMEDIATE,
      points: [rgbToHsi(this.color[0], this.color[1], this.color[2])],
      num_points: 1,
    }

    return [fade]
  }

  public generateThreeJSRepresentation = (
    movementIndex: number,
    movement: Movement,
    settings: Settings,
    visualisationSettings: VisualisationSettings,
    addColouredLine: AddLineCallback,
    addDottedLine: AddLineCallback,
    addReactComponent: AddComponentCallback,
    fromT: number,
    toT: number,
  ) => {
    // Annotate draw order
    annotateDrawOrder(
      movementIndex,
      movement,
      visualisationSettings,
      addReactComponent,
    )

    // A random color material draws the line segment(s) from the start to the end with a single color
    const numSegments =
      movement.type === MOVEMENT_TYPE.LINE ||
      movement.type === MOVEMENT_TYPE.POINT
        ? 1
        : visualisationSettings.curveSegments

    // For the number of segments,
    for (let index = 0; index < numSegments; index++) {
      const startT = MathUtils.mapLinear(index / numSegments, 0, 1, fromT, toT)
      const endT = MathUtils.mapLinear(
        (index + 1) / numSegments,
        0,
        1,
        fromT,
        toT,
      )

      // Sample points along the movement
      const start = movement.samplePoint(startT)
      const end = movement.samplePoint(endT)

      // Add the line
      addColouredLine(start, end, this.color, this.color, movement.objectID)
    }
  }
}

export interface RandomColorMaterialEditorProps {
  objectID: NodeID
  json: RandomColorMaterialJSON
}

export function RandomColorMaterialEditor(
  props: RandomColorMaterialEditorProps,
) {
  return <>RandomColorMaterialEditor for {props.objectID}</>
}
