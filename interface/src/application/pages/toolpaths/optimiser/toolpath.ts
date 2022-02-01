import { DenseMovements } from './movements'
import {
  LightMove,
  LightMoveType,
  MovementMove,
  MovementMoveReference,
  MovementMoveType,
} from '../../../../application/typedState'
import { Settings } from './settings'
import { VisualisationSettings } from '../interface/state'
import { Vector3 } from 'three'

export interface Toolpath {
  movementMoves: MovementMove[]
  lightMoves: LightMove[]
}

/**
 * Take a densely ordered set of movements and convert them to a toolpath.
 *
 * Done just before rendering on the UI thread.
 */
export function toolpath(
  denseMovements: DenseMovements,
  settings: Settings,
  visualisationSettings: VisualisationSettings,
  cameraPosition: Vector3,
): Toolpath {
  const movementMoves: MovementMove[] = []
  const lightMoves: LightMove[] = []

  let movementTimestamp = 0
  let lightFadeTimestamp = 0
  // each movement should have a generateToolpath method
  for (const movement of denseMovements) {
    const move = movement.generateToolpath()

    if (move.duration === 0) {
      // zero duration moves are dumped
      continue
    }

    // Add the movement
    movementMoves.push({
      ...move,
      sync_offset: movementTimestamp,
    })

    if (lightFadeTimestamp !== movementTimestamp) {
      console.warn(
        `The light and movement timestamps not adding up? They're ${Math.abs(
          lightFadeTimestamp - movementTimestamp,
        )}ms off at timestamp ${movementTimestamp}`,
      )
    }

    // These should be the same anyway.
    lightFadeTimestamp = movementTimestamp

    // Build the light moves
    for (const lightMove of movement.material.generateLightpath(
      movement,
      settings,
      visualisationSettings,
      cameraPosition,
      0, // from start
      1, // to end
    )) {
      lightMoves.push({
        ...lightMove,
        timestamp: lightFadeTimestamp,
      })

      // Increment the timestamp
      lightFadeTimestamp += lightMove.duration
    }

    // Increment the timestamp
    movementTimestamp += move.duration
  }

  // Add our global offset
  if (settings.lightFadeOffset !== 0) {
    if (settings.lightFadeOffset > 0) {
      const delay = settings.lightFadeOffset

      // Delay all fades by this amount
      for (const fade of lightMoves) {
        fade.timestamp += delay
      }

      // Add a blank fade to the beginning
      lightMoves.unshift({
        timestamp: 0,
        duration: delay,
        type: LightMoveType.IMMEDIATE,
        points: [[0, 0, 0]],
        num_points: 1,
      })
    } else {
      const delay = settings.lightFadeOffset * -1

      // Delay all movements by this amount
      for (const movement of movementMoves) {
        movement.sync_offset += delay
      }

      // Add a blank move to the beginning
      movementMoves.unshift({
        sync_offset: 0,
        duration: delay,
        type: MovementMoveType.POINT_TRANSIT, // Despite being a point, draw a line
        reference: MovementMoveReference.RELATIVE,
        points: [[0, 0, 0]],
        num_points: 1,
      })
    }
  }

  return {
    movementMoves,
    lightMoves,
  }
}
