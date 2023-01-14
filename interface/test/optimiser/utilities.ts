import { MathUtils, Vector3 } from 'three'
import {
  DenseMovements,
  Line,
  Movement,
  RGB,
  RGBA,
} from '../../src/application/pages/toolpaths/optimiser/movements'

import { ColorRampMaterial } from '../../src/application/pages/toolpaths/optimiser/materials/ColorRamp'

import {
  getDefaultCameraPosition,
  getDefaultVisualisationSettings,
  getDefaultSettings,
  hexToRgba,
} from '../materials/utilities'

import {
  toolpath,
  Toolpath,
} from '../../src/application/pages/toolpaths/optimiser/toolpath'
import {
  LightMove,
  LightMoveType,
  MovementMove,
  MovementMoveType,
} from '../../src/application/typedState'
import {
  hsiToRgb,
  lerpRGBA,
} from '../../src/application/pages/toolpaths/optimiser/materials/utilities'

import type { default as ChalkType } from 'chalk'
const chalk = require('chalk') as ChalkType.Chalk

/**
 * Generates an X axis aligned line, from `from` to `to`.
 */
export function getXAxisAlignedLine(from: number, to: number) {
  const line = new Line(
    new Vector3(from, 0, 0),
    new Vector3(to, 0, 0),
    new ColorRampMaterial(hexToRgba(`#ff0000`), hexToRgba(`#00ff00`)),
    `${from}->${to}`,
    [],
  )

  line.setMaxSpeed(1000)

  return line
}

export function generateToolpathWithDefaults(movements: DenseMovements) {
  return toolpath(
    movements,
    getDefaultSettings(),
    getDefaultVisualisationSettings(),
    getDefaultCameraPosition(),
  )
}

/**
 * A 2d zaphod renderer!
 *
 * The X axis is shrunk by the `mmPerCell` argument,
 * The Y axis represents time, with one line per move.
 * Each cell is sampled from the light moves with a binary search algorithm.
 */
export function renderMovementSequence(
  toolpath: Toolpath,
  options: {
    mmPerCell: number
  },
) {
  const lines: string[] = []
  const moves = toolpath.movementMoves

  if (moves.length === 0) {
    throw new Error(`need at least one movement`)
  }

  const lastMovement = moves.at(-1)!

  const maxTime = lastMovement.sync_offset + lastMovement.duration

  const longestTimingStrLength = movementTimingStr(lastMovement).length

  lines.push(`rendering from ${moves[0].sync_offset}ms to ${maxTime}ms`)

  for (const move of moves) {
    // Sync offset
    const timing = movementTimingStr(move, longestTimingStrLength)

    const startingX = movementGetStartX(move)
    const endingX = movementGetEndX(move)

    let rasterCellsFrom = startingX
    let rasterCellsTo = endingX
    let representation = '>' // Assume left to right

    // If they go the other way
    if (endingX < startingX) {
      rasterCellsFrom = endingX
      rasterCellsTo = startingX
      representation = '<' // It's going the other way
    } else if (startingX === endingX) {
      // for points, render an x
      representation = 'x'
    }

    // Scale everything down
    rasterCellsFrom = Math.floor(rasterCellsFrom / options.mmPerCell)
    rasterCellsTo = Math.floor(rasterCellsTo / options.mmPerCell)

    if (startingX === endingX) {
      // always render at least once cell
      rasterCellsTo += 1
    }

    // Start the line with the timing information and the blank cells
    let line = `${timing} | ${' '.repeat(rasterCellsFrom)}`

    // Render the filled cells
    for (
      let charIndex = rasterCellsFrom;
      charIndex < rasterCellsTo; // Render up to that cell
      charIndex++
    ) {
      let time = move.sync_offset

      if (rasterCellsFrom !== rasterCellsTo) {
        time = MathUtils.mapLinear(
          charIndex,
          rasterCellsFrom,
          rasterCellsTo,
          move.sync_offset,
          move.sync_offset + move.duration,
        )
      }

      switch (representation) {
        case 'x':
          line += renderCell('x', time, toolpath)

          break
        case '>':
          // Right facing lines
          if (charIndex === rasterCellsTo - 1) {
            line += renderCell('>', time, toolpath)
          } else {
            line += renderCell('-', time, toolpath)
          }
          break
        case '<':
          // Left facing lines
          if (charIndex === rasterCellsFrom) {
            line += renderCell('<', time, toolpath)
          } else {
            line += renderCell('-', time, toolpath)
          }
          break

        default:
          throw new Error(`unknown representation`)
      }
    }

    lines.push(line)
  }

  return lines.join('\n')
}

function movementTimingStr(move: MovementMove, maxLength = 0) {
  const str = `${move.sync_offset}ms -> ${move.sync_offset + move.duration}ms `

  return str.padStart(maxLength)
}

function movementGetStartX(move: MovementMove) {
  // The first X position for each move
  return move.points.at(0)![0]
}

function movementGetEndX(move: MovementMove) {
  // The last X position for each move
  return move.points[move.points.length - 1][0]
}

const blankRGBA: RGBA | RGB = [0, 0, 0, 0]

function renderCell(
  text: '<' | 'x' | '>' | '-',
  time: number,
  toolpath: Toolpath,
) {
  const fade = findLightFade(toolpath, time)

  let rgb: RGB | RGBA = blankRGBA

  // If it's an immediate light fade, render the colour at that time
  if (fade.type === LightMoveType.IMMEDIATE) {
    rgb = hsiToRgb(fade.points[0][0], fade.points[0][1], fade.points[0][2])
  } else {
    // It's a fade, interpolate it
    rgb = lerpRGBA(
      [...hsiToRgb(fade.points[0][0], fade.points[0][1], fade.points[0][2]), 1],
      [...hsiToRgb(fade.points[1][0], fade.points[1][1], fade.points[1][2]), 1],
      MathUtils.mapLinear(
        time,
        fade.timestamp,
        fade.timestamp + fade.duration,
        0,
        1,
      ),
    )
  }

  return chalk
    .bgRgb(
      Math.floor(rgb[0] * 255),
      Math.floor(rgb[1] * 255),
      Math.floor(rgb[2] * 255),
    )
    .black(text)
}

export function findLightFade(toolpath: Toolpath, time: number): LightMove {
  const lastLightFade = toolpath.lightMoves.at(-1)!
  const finishTime = lastLightFade.timestamp + lastLightFade.duration

  if (time > finishTime) {
    // Create a blank light move for times after
    return {
      timestamp: finishTime,
      duration: 9999,
      type: LightMoveType.IMMEDIATE,
      points: [[0, 0, 0]],
      num_points: 3,
    }
  }

  for (let index = toolpath.lightMoves.length - 1; index >= 0; index--) {
    const lightMove = toolpath.lightMoves[index]

    if (time >= lightMove.timestamp) {
      return lightMove
    }
  }

  throw new Error(`couldnt find light fade at time ${time}`)
}
