import { MathUtils, Vector3 } from 'three'
import { NodeID } from '../../interface/RenderableTree'
import { changeState, useStore, VisualisationSettings } from '../../interface/state'
import type { AddComponentCallback, Movement, RGBA, RGB, Bezier, ConstantSpeedBezier } from '../movements'

import { MOVEMENT_TYPE } from './../movement_types'

import { Intent, Tag } from '@blueprintjs/core'
import { Html } from '@react-three/drei'
import React, { useCallback } from 'react'
import {
  findHighestApproximateSpeedAndT,
  isBezier,
  isConstantSpeedBezier,
  isTransition,
  predictSpeedAtT,
} from '../movement_utilities'

export const enum MATERIALS {
  DEFAULT = 'default', // used for overrides to indicate no override
  COLOR = 'color',
  COLOR_RAMP = 'color_ramp',
  TEXTURE_STRIP = 'texture_strip',
  VELOCITY = 'velocity',
  MIX = 'mix',
  INVISIBLE = 'invisible',
  DELAY = 'delay',
  RANDOM = 'random',
  FLICKER = 'flicker',
  Z_GRADIENT = 'z_gradient',
  Z_DEPTH = 'z_depth',
  BLEND = 'blend',
  REMAP = 'remap',
  DURATION = 'duration',
  FLIPPED = 'flipped',
}

// Our angles are stored as floats in the range 0-1
const RadiansToUnitFloat = 1 / (2 * Math.PI)
const UnitFloatToRadians = 2 * Math.PI

// RGB, HSI all 0-1 floats
export function rgbToHsi(
  red: number,
  green: number,
  blue: number,
): [hue: number, saturation: number, intensity: number] {
  const intensity = (red + green + blue) / 3

  if (red == green && green == blue) {
    return [0, 0, intensity]
  } else {
    const alpha = (1 / 2) * (2 * red - green - blue)
    const beta = (Math.sqrt(3) / 2) * (green - blue)
    const hRad = Math.atan2(beta, alpha)
    const min = Math.min(red, green, blue)
    const saturation = 1 - min / intensity
    return [(hRad < 0 ? 2 * Math.PI + hRad : hRad) * RadiansToUnitFloat, saturation, intensity]
  }
}

// RGB, HSI all 0-1 floats
export function hsiToRgb(
  hue: number,
  saturation: number,
  intensity: number,
): [red: number, green: number, blue: number] {
  let r = 0
  let g = 0
  let b = 0
  let hRad = hue * UnitFloatToRadians

  if (hRad < (2 * Math.PI) / 3) {
    b = (1 - saturation) / 3
    r = (1 + (saturation * Math.cos(hRad)) / Math.cos(Math.PI / 3 - hRad)) / 3
    g = 1 - (b + r)
  } else if (hRad < (4 * Math.PI) / 3) {
    hRad -= (2 * Math.PI) / 3
    r = (1 - saturation) / 3
    g = (1 + (saturation * Math.cos(hRad)) / Math.cos(Math.PI / 3 - hRad)) / 3
    b = 1 - (r + g)
  } else {
    hRad -= (4 * Math.PI) / 3
    g = (1 - saturation) / 3
    b = (1 + (saturation * Math.cos(hRad)) / Math.cos(Math.PI / 3 - hRad)) / 3
    r = 1 - (g + b)
  }
  return [3 * intensity * r, 3 * intensity * g, 3 * intensity * b]
}

/**
 * Linearly lerp between to colours in the LAB colour space
 */
export function lerpRGBA(
  a: [r: number, g: number, b: number, a: number],
  b: [r: number, g: number, b: number, a: number],
  t: number,
): [r: number, g: number, b: number, a: number] {
  // Convert RGBA to LAB
  const labA = RGBToLAB(a.slice(0, 3) as RGB)
  const labB = RGBToLAB(b.slice(0, 3) as RGB)

  // Lerp in LAB space
  const lerpedLab = [
    labA[0] + (labB[0] - labA[0]) * t,
    labA[1] + (labB[1] - labA[1]) * t,
    labA[2] + (labB[2] - labA[2]) * t,
  ] as LAB

  const lerpedRGB = XYZToRGB(LABToXYZ(lerpedLab))

  // Lerp alpha separately
  const lerpedAlpha = a[3] + (b[3] - a[3]) * t

  // Return the result as RGBA
  return [...lerpedRGB, lerpedAlpha] as RGBA
}

export function movementTypeToLetter(movement: Movement) {
  let type = 'U'

  switch (movement.type) {
    case MOVEMENT_TYPE.LINE:
      type = 'L'
      break
    case MOVEMENT_TYPE.POINT:
      type = 'P'
      break
    case MOVEMENT_TYPE.BEZIER:
      type = 'B'
      break
    case MOVEMENT_TYPE.BEZIER_CONSTANT_SPEED:
      type = 'CB'
      break
    case MOVEMENT_TYPE.CATMULL_ROM:
      type = 'ROM'
      break
    case MOVEMENT_TYPE.CATMULL_ROM_CONSTANT_SPEED:
      type = 'CROM'
      break
    case MOVEMENT_TYPE.INTER_LINE_TRANSITION:
      type = 'IT'
      break

    default:
      break
  }

  return type
}

export function movementTypeToIntent(movement: Movement) {
  let intent: Intent = 'none'

  if (isTransition(movement)) {
    intent = 'primary'
  }

  return intent
}

export function annotateDrawOrder(movementIndex: number, movement: Movement, addReactComponent: AddComponentCallback) {
  const intent = movementTypeToIntent(movement)
  const type = movementTypeToLetter(movement)

  let extra = ``

  // if (isBezier(movement)) {
  //   const transition = movement as Bezier

  //   try {
  //     transition.entryVelocity.length()
  //   } catch (e) {
  //     debugger
  //   }

  //   extra = `in:${
  //     Math.round(transition.entryVelocity.length() * 100) / 100
  //   } (${Math.round(predictSpeedAtT(movement, 0) * 100) / 100}) out:${
  //     Math.round(transition.exitVelocity.length() * 100) / 100
  //   } (${Math.round(predictSpeedAtT(movement, 1) * 100) / 100}) max:${
  //     Math.round(transition.maxSpeed * 100) / 100
  //   } (${
  //     Math.round(findHighestApproximateSpeedAndT(movement).maxSpeed * 100) /
  //     100
  //   }) priority: ${
  //     transition.getEntrySpeedExact() ? 'entry' : 'exit'
  //   } flipped: ${transition.isFlipped ? 'flipped' : 'normal'}`
  // }

  // if (isConstantSpeedBezier(movement)) {
  //   const transition = movement as ConstantSpeedBezier
  //   extra = `in:${Math.round(transition.maxSpeed * 100) / 100} (${
  //     Math.round(predictSpeedAtT(movement, 0) * 100) / 100
  //   }) out:${Math.round(transition.maxSpeed * 100) / 100} (${
  //     Math.round(predictSpeedAtT(movement, 1) * 100) / 100
  //   }) max:${Math.round(transition.maxSpeed * 100) / 100} (${
  //     Math.round(findHighestApproximateSpeedAndT(movement).maxSpeed * 100) /
  //     100
  //   }) flipped: ${transition.isFlipped ? 'flipped' : 'normal'}`
  // }

  addReactComponent(
    generateHtmlTagFromAveragePosition(
      movementIndex,
      movement.objectID,
      movement.getApproximateCentroid(),
      intent,
      `${type} #${movementIndex} (${movement.getDuration()}ms) ${extra}`,
    ),
  )
}

// Map them from near is 4096, far is 0, with the canvas div having a zIndex set, DOM precedence means it'll always be on top
const TagZIndexRange = [4096, 0]

export function generateHtmlTagFromAveragePosition(
  movementIndex: number,
  objectID: string,
  centroid: Vector3,
  intent: Intent,
  text: string,
) {
  // Convert the centroid from Blender units to ThreeJS units
  const threeCentroid = new Vector3(centroid.x, centroid.z, -centroid.y)

  const component = (
    <Html position={threeCentroid} key={text} zIndexRange={TagZIndexRange}>
      <TagThatHidesWhenNotInList movementIndex={movementIndex} objectID={objectID} intent={intent} text={text} />
    </Html>
  )

  return component
}

function TagThatHidesWhenNotInList(props: { movementIndex: number; objectID: string; intent: Intent; text: string }) {
  const isHidden = useStore(state => {
    // If nothing is hovered, display everything
    if (state.treeStore.hoveredItems.length === 0) {
      return false
    }

    // If something is hovered, only display if it matches a hovered object ID or movement index
    return !(
      state.treeStore.hoveredItems.includes(props.movementIndex) ||
      state.treeStore.hoveredItems.includes(props.objectID)
    )
  })

  const onMouseEnter = useCallback(() => {
    changeState(state => {
      state.treeStore.hoveredItems = [props.movementIndex]
    })
  }, [props.movementIndex])

  const onMouseLeave = useCallback(() => {
    changeState(state => {
      state.treeStore.hoveredItems = []
    })
  }, [props.movementIndex])

  return (
    <Tag
      intent={props.intent}
      minimal={isHidden}
      style={{ opacity: isHidden ? 0.2 : 1, cursor: 'pointer' }}
      onMouseEnter={onMouseEnter}
      onMouseLeave={onMouseLeave}
    >
      {props.text}
    </Tag>
  )
}

/**
 * Based on https://github.com/Qix-/color-convert/tree/master
 * 
 * Copyright (c) 2011-2016 Heather Arthur <fayearthur@gmail.com>.
 * Copyright (c) 2016-2021 Josh Junon <josh@junon.me>.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

/** RGB 0-1 floats to XYZ 0-100 floats */
function RGBToXYZ(col: RGB) {
  let r = col[0]
  let g = col[1]
  let b = col[2]

  r = r > 0.04045 ? ((r + 0.055) / 1.055) ** 2.4 : r / 12.92
  g = g > 0.04045 ? ((g + 0.055) / 1.055) ** 2.4 : g / 12.92
  b = b > 0.04045 ? ((b + 0.055) / 1.055) ** 2.4 : b / 12.92

  const x = r * 0.4124564 + g * 0.3575761 + b * 0.1804375
  const y = r * 0.2126729 + g * 0.7151522 + b * 0.072175
  const z = r * 0.0193339 + g * 0.119192 + b * 0.9503041

  return [x * 100, y * 100, z * 100]
}

// LAB f(t) constant
const LAB_FT = Math.pow(6 / 29, 3)

function RGBToLAB(col: RGB) {
  const xyz = RGBToXYZ(col)
  let x = xyz[0]
  let y = xyz[1]
  let z = xyz[2]

  x /= 95.047
  y /= 100
  z /= 108.883

  x = x > LAB_FT ? x ** (1 / 3) : 7.787 * x + 16 / 116
  y = y > LAB_FT ? y ** (1 / 3) : 7.787 * y + 16 / 116
  z = z > LAB_FT ? z ** (1 / 3) : 7.787 * z + 16 / 116

  const l = 116 * y - 16
  const a = 500 * (x - y)
  const b = 200 * (y - z)

  return [l, a, b]
}

type XYZ = [x: number, y: number, z: number]
type LAB = [l: number, a: number, b: number]

function LABToXYZ(col: LAB): XYZ {
  const l = col[0]
  const a = col[1]
  const b = col[2]
  let y = (l + 16) / 116
  let x = a / 500 + y
  let z = y - b / 200

  const y2 = y ** 3
  const x2 = x ** 3
  const z2 = z ** 3
  y = y2 > LAB_FT ? y2 : (y - 16 / 116) / 7.787
  x = x2 > LAB_FT ? x2 : (x - 16 / 116) / 7.787
  z = z2 > LAB_FT ? z2 : (z - 16 / 116) / 7.787

  // Illuminant D65 XYZ Tristrimulus Values
  // https://en.wikipedia.org/wiki/CIE_1931_color_space
  x *= 95.047
  y *= 100
  z *= 108.883

  return [x, y, z]
}

function XYZToRGB(col: XYZ): RGB {
  const x = col[0] / 100
  const y = col[1] / 100
  const z = col[2] / 100
  let r = x * 3.2404542 + y * -1.5371385 + z * -0.4985314
  let g = x * -0.969266 + y * 1.8760108 + z * 0.041556
  let b = x * 0.0556434 + y * -0.2040259 + z * 1.0572252

  r = r > 0.0031308 ? 1.055 * r ** (1.0 / 2.4) - 0.055 : r * 12.92
  g = g > 0.0031308 ? 1.055 * g ** (1.0 / 2.4) - 0.055 : g * 12.92
  b = b > 0.0031308 ? 1.055 * b ** (1.0 / 2.4) - 0.055 : b * 12.92

  r = Math.min(Math.max(0, r), 1)
  g = Math.min(Math.max(0, g), 1)
  b = Math.min(Math.max(0, b), 1)

  return [r, g, b]
}
