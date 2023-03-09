import { DenseMovements } from './movement_utilities'
import { GLOBAL_OVERRIDE_OBJECT_ID } from './movements'
import {
  LightMove,
  LightMoveType,
  MovementMove,
  MovementMoveReference,
  MovementMoveType,
} from '../../../../application/typedState'
import type { Settings } from './settings'
import { VisualisationSettings } from '../interface/state'
import { Vector3 } from 'three'
import { getMaterialOverride, importMaterial } from './material'
import type { TimedTrigger, TriggerAlignment } from './triggers'

export interface Toolpath {
  movementMoves: MovementMove[]
  lightMoves: LightMove[]
  triggers: TimedTrigger[]
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
  const triggers: TimedTrigger[] = []

  let movementTimestamp = 0
  let lightFadeTimestamp = 0
  // each movement should have a generateToolpath method
  for (const movement of denseMovements) {
    const moves = movement.generateToolpath()

    // The light fades start at the offset of the movement time stamp
    lightFadeTimestamp = movementTimestamp

    // Grab the triggers
    const movementTriggers = movement.getTriggers()
    const movementStartTime = movementTimestamp

    // Accumulate 'start' triggers
    const movementTriggersOnStart = movementTriggers.filter(
      trigger => trigger.align === (`start` as TriggerAlignment.START),
    )

    for (const trigger of movementTriggersOnStart) {
      triggers.push({
        ...trigger,
        timestamp: movementTimestamp,
      })
    }

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

    // Accumulate middle and end triggers, apply timestamp
    const movementTriggersOnMiddle = movementTriggers.filter(
      trigger => trigger.align === (`middle` as TriggerAlignment.MIDDLE),
    )

    for (const trigger of movementTriggersOnMiddle) {
      triggers.push({
        ...trigger,
        timestamp: (movementStartTime + movementTimestamp) / 2, // this is the middle
      })
    }

    const movementTriggersOnEnd = movementTriggers.filter(
      trigger => trigger.align === (`end` as TriggerAlignment.END),
    )

    for (const trigger of movementTriggersOnEnd) {
      triggers.push({
        ...trigger,
        timestamp: movementTimestamp, // now the end
      })
    }

    // Get the material override
    const globalMaterialOverride = visualisationSettings
      .objectMaterialOverrides[GLOBAL_OVERRIDE_OBJECT_ID]
      ? importMaterial(
          visualisationSettings.objectMaterialOverrides[
            GLOBAL_OVERRIDE_OBJECT_ID
          ],
        )
      : null

    let material = globalMaterialOverride
      ? globalMaterialOverride
      : movement.material
    material = getMaterialOverride(
      visualisationSettings,
      movement.material,
      movement.overrideKeys,
    )

    // If the movement is flipped, reverse the ordering of the material so it stays consistent
    const matStartT = movement.isFlipped ? 1 : 0
    const matendT = movement.isFlipped ? 0 : 1

    // Accumulate the light fades
    for (const lightMove of material.generateLightpath(
      movement,
      settings,
      visualisationSettings,
      cameraPosition,
      matStartT,
      matendT,
    )) {
      if (lightMove.duration !== Math.floor(lightMove.duration)) {
        console.log(`precision error at time stamp ${movementTimestamp}`)
      }

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

    if (lightFadeTimestamp > movementTimestamp) {
      // console.warn(
      //   `${
      //     lightFadeTimestamp - movementTimestamp
      //   }ms surplus light fades at timestamp ${movementTimestamp}ms`,
      // )
    } else if (lightFadeTimestamp < movementTimestamp) {
      console.warn(
        `Not enough light fades to complete this movement at movement timestamp ${movementTimestamp}ms, missing ${
          movementTimestamp - lightFadeTimestamp
        }ms of light fades`,
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
        settings: { 
          num_points: 1, 
          type: LightMoveType.IMMEDIATE, 
        },
        points: [[0, 0, 0]],
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
    triggers,
  }
}
