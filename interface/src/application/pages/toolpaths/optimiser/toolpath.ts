import { DenseMovements, GLOBAL_OVERRIDE_OBJECT_ID } from './movements'
import {
  LightMove,
  LightMoveType,
  MovementMove,
  MovementMoveReference,
  MovementMoveType,
} from '../../../../application/typedState'
import { Settings } from './settings'
import { getMaterialOverride, VisualisationSettings } from '../interface/state'
import { Vector3 } from 'three'
import { importMaterial } from './material'

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
  const lightFades: LightMove[] = []

  let movementTimestamp = 0
  let lightFadeTimestamp = 0
  // each movement should have a generateToolpath method
  for (const movement of denseMovements) {
    const moves = movement.generateToolpath()

    // The light fades start at the offset of the movement time stamp
    lightFadeTimestamp = movementTimestamp

    // Accumulate the movement moves
    for (const move of moves) {
      if (move.duration === 0) {
        // zero duration moves are dumped
        continue
      }

      // Add the movement
      movementMoves.push({
        ...move,
        sync_offset: movementTimestamp,
      })

      movementTimestamp += move.duration
    }

    // Get the material override
    const globalMaterialOverride = visualisationSettings.objectMaterialOverrides[GLOBAL_OVERRIDE_OBJECT_ID]
      ? importMaterial(visualisationSettings.objectMaterialOverrides[GLOBAL_OVERRIDE_OBJECT_ID])
      : null

    let material = globalMaterialOverride ? globalMaterialOverride : movement.material
    material = getMaterialOverride(visualisationSettings, movement.material, movement.overrideKeys)

    // Accumulate the light fades
    for (const lightMove of material.generateLightpath(
      movement,
      settings,
      visualisationSettings,
      cameraPosition,
      0, // from start
      1, // to end
    )) {
      if (lightMove.duration === 0) {
        // zero duration fades are dumped
        continue
      }

      lightFades.push({
        ...lightMove,
        timestamp: lightFadeTimestamp,
      })

      // Increment the timestamp
      lightFadeTimestamp += lightMove.duration
    }

    if (lightFadeTimestamp !== movementTimestamp) {
      console.warn(
        `The light and movement timestamps not adding up? They're ${Math.abs(
          lightFadeTimestamp - movementTimestamp,
        )}ms off at timestamp ${movementTimestamp}`,
      )
    }
  }

  // Add our global offset
  if (settings.lightFadeOffset !== 0) {
    if (settings.lightFadeOffset > 0) {
      const delay = settings.lightFadeOffset

      // Delay all fades by this amount
      for (const fade of lightFades) {
        fade.timestamp += delay
      }

      // Add a blank fade to the beginning
      lightFades.unshift({
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
    lightMoves: lightFades,
  }
}
