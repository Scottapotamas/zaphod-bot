import { initialState } from '../../src/application/pages/toolpaths/interface/state'
import { Vector3 } from 'three'
import {
  Line,
  Movement,
} from '../../src/application/pages/toolpaths/optimiser/movements'
import { Material } from '../../src/application/pages/toolpaths/optimiser/materials/Base'
import { PlannerLightMove } from '../../src/application/pages/toolpaths/optimiser/hardware'

import type { default as ChalkType } from 'chalk'

const chalk = require('chalk') as ChalkType.Chalk

import {
  hsiToRgb,
  lerpRGBA,
} from '../../src/application/pages/toolpaths/optimiser/materials/utilities'

export function hexToRgba(
  hex: string,
): [r: number, g: number, b: number, a: number] {
  const result = /^#?([a-f\d]{2})([a-f\d]{2})([a-f\d]{2})$/i.exec(hex)

  if (!result) {
    throw new Error(`Couldn't parse hex colour: ${hex}`)
  }

  return [
    parseInt(result[1], 16) / 255, //
    parseInt(result[2], 16) / 255,
    parseInt(result[3], 16) / 255,
    1,
  ]
}

function componentToHex(c: number) {
  var hex = Math.floor(c).toString(16)
  return hex.length == 1 ? '0' + hex : hex
}

export function rgb255ToHex(r: number, g: number, b: number) {
  return '#' + componentToHex(r) + componentToHex(g) + componentToHex(b)
}

export function getDefaultSettings() {
  // Remove that 7ms light fade offset
  return Object.assign(initialState.settings, { lightFadeOffset: 0 })
}

export function getDefaultVisualisationSettings() {
  return initialState.visualisationSettings
}

export function getDefaultCameraPosition() {
  return new Vector3(0, 0, 0)
}

/**
 * Generates a line as long as the duration, with a max speed of 1mm/ms
 */
export function getNominalLine(material: Material, duration: number = 100) {
  const line = new Line(
    new Vector3(0, 0, 0),
    new Vector3(duration, 0, 0),
    material,
    'objectId',
    [],
  )

  line.setMaxSpeed(1000)

  return line
}

export function generateLightPathFromMovement(
  movement: Movement,
  fromT = 0,
  toT = 1,
) {
  const settings = getDefaultSettings()
  const visualisationSettings = getDefaultVisualisationSettings()
  const cameraPosition = getDefaultCameraPosition()

  return movement.material.generateLightpath(
    movement,
    settings,
    visualisationSettings,
    cameraPosition,
    fromT,
    toT,
  )
}

export function prettyPrintLightPath(lightPath: PlannerLightMove[]) {
  const str = lightPath.map(p => {
    const startCol = hsiToRgb(
      p.points[0][0],
      p.points[0][1],
      p.points[0][2],
    ).map(n => Math.floor(n * 255))

    const endCol = hsiToRgb(p.points[1][0], p.points[1][1], p.points[1][2]).map(
      n => Math.floor(n * 255),
    )

    return `[${chalk
      .rgb(startCol[0], startCol[1], startCol[2])
      .bold(rgb255ToHex(startCol[0], startCol[1], startCol[2]))}] to [${chalk
      .rgb(endCol[0], endCol[1], endCol[2])
      .bold(rgb255ToHex(endCol[0], endCol[1], endCol[2]))}] over ${
      p.duration
    }ms `
  })

  console.log(str.join('\n'))
}

export function formatLightPathColourSequence(
  lightPath: PlannerLightMove[],
  millisecondsPerSquare: number,
) {
  const str = lightPath.map(p => {
    let acc = ''

    const steps = p.duration / millisecondsPerSquare

    for (let index = 0; index < steps; index++) {
      const rgb = lerpRGBA(
        [...hsiToRgb(p.points[0][0], p.points[0][1], p.points[0][2]), 1],
        [...hsiToRgb(p.points[1][0], p.points[1][1], p.points[1][2]), 1],
        index / steps,
      )

      acc += chalk
        .bgRgb(
          Math.floor(rgb[0] * 255),
          Math.floor(rgb[1] * 255),
          Math.floor(rgb[2] * 255),
        )
        .bold(' ')
    }

    return acc
  })

  return str.join('')
}

/**
 * Standardises the length
 */
export function matsToSeq(materials: Material[], millisecondsPerSquare = 5) {
  return materials
    .map(mat => {
      const line = getNominalLine(mat)

      const lightPath = generateLightPathFromMovement(line)

      return formatLightPathColourSequence(
        lightPath,
        millisecondsPerSquare * materials.length,
      )
    })
    .join('')
}
