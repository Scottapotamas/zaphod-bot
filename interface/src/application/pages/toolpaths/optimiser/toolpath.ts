import { DenseMovements } from './movement_utilities'
import { GLOBAL_OVERRIDE_OBJECT_ID } from './movements'
import {
  LightMove,
  LightMoveType,
  LightPoint,
  MovementMove,
  MovementMoveReference,
  MovementMoveType,
  MovementPoint,
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
  for (
    let movementIndex = 0;
    movementIndex < denseMovements.length;
    movementIndex++
  ) {
    const movement = denseMovements[movementIndex]

    const moves = movement.generateToolpath()

    const movementMovesTotalDuration = moves.reduce(
      (acc, fade) => acc + fade.duration,
      0,
    )

    if (movement.getDuration() !== movementMovesTotalDuration) {
      console.warn(
        `move ${movementIndex} has mismatching durations, getDuration: ${movement.getDuration()}, movementMovesTotalDuration: ${movementMovesTotalDuration}`,
      )
    }

    // If the movement has no duration overall, skip it entirely
    if (movement.getDuration() === 0) {
      continue
    }

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
        id: 0 // Will be replaced after the deduplication 
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

    const movementLightFades = material.generateLightpath(
      movement,
      settings,
      visualisationSettings,
      cameraPosition,
      matStartT,
      matendT,
    )

    const totalLightFadeDuration = movementLightFades.reduce(
      (acc, fade) => acc + fade.duration,
      0,
    )

    // Accumulate the light fades

    for (
      let lightMoveIndex = 0;
      lightMoveIndex < movementLightFades.length;
      lightMoveIndex++
    ) {
      const lightMove = movementLightFades[lightMoveIndex]

      if (lightMove.duration !== Math.floor(lightMove.duration)) {
        console.log(`precision error at time stamp ${movementTimestamp}`)
      }

      if (lightMove.duration === 0) {
        // zero duration fades are dumped
        console.log(
          `zero duration light move dumped at time stamp ${movementTimestamp} as part of movement index ${movementIndex} (with duration ${movement.getDuration()}), material #${lightMoveIndex}/${
            movementLightFades.length - 1
          } fade type ${
            material.type
          }, total duration of movement's light fades: ${totalLightFadeDuration} `,
        )
        continue
      }

      lightFades.push({
        ...lightMove,
        timestamp: lightFadeTimestamp,
        id: 0, // Will be replaced after the deduplication
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
        id: 0 // Will be replaced after the deduplication, but will likely stay 0
      })
    } else {
      const delay = settings.lightFadeOffset * -1

      // Delay all movements by this amount
      for (const movement of movementMoves) {
        movement.sync_offset += delay
      }

      // Add a blank move to the beginning
      movementMoves.unshift({
        duration: delay,
        type: MovementMoveType.POINT_TRANSIT, // Despite being a point, draw a line
        reference: MovementMoveReference.RELATIVE,
        points: [[0, 0, 0]],
        num_points: 1,
        sync_offset: 0,
        id: 0 // Will be replaced after the deduplication, but will likely stay 0
      })
    }
  }

  const prePostProcessLightFadeCount = lightFades.length

  // Deduplicate light fades in a single backward pass
  for (let index = lightFades.length - 1; index >= 1; index--) {
    const previousFade = lightFades[index - 1]
    const currentFade = lightFades[index]

    if (
      lightFadePointsMatch(
        lightFadeEndingColor(previousFade),
        lightFadeStartingColor(previousFade), // If the previous fade doesn't change colour
      ) &&
      lightFadePointsMatch(
        lightFadeEndingColor(currentFade),
        lightFadeStartingColor(currentFade), // If the current fade doesn't change colour
      ) &&
      lightFadePointsMatch(
        lightFadeEndingColor(previousFade),
        lightFadeStartingColor(currentFade), // If the fades are the same colour
      )
    ) {
      // add the current fade duration to the previous fade
      previousFade.duration += currentFade.duration

      // delete the current fade
      lightFades.splice(index, 1)
    }
  }

  const postPostProcessLightFadeCount = lightFades.length

  if (postPostProcessLightFadeCount !== prePostProcessLightFadeCount) {
    console.log(
      `Reduced light fade count from ${prePostProcessLightFadeCount} to ${postPostProcessLightFadeCount}`,
    )
  }

  const prePostProcessMovementMoveCount = movementMoves.length

  // Deduplicate movement moves in a single backward pass
  for (let index = movementMoves.length - 1; index >= 1; index--) {
    const previousMovementMove = movementMoves[index - 1]
    const currentMovementMove = movementMoves[index]

    if (
      previousMovementMove.type === currentMovementMove.type && // If lines are the same types
      movementMovePointsMatch(
        movementMoveEndingPosition(previousMovementMove),
        movementMoveStartingPosition(previousMovementMove), // If the previous line is stationary
      ) &&
      movementMovePointsMatch(
        movementMoveEndingPosition(currentMovementMove),
        movementMoveStartingPosition(currentMovementMove), // If the current line is stationary
      ) &&
      movementMovePointsMatch(
        movementMoveEndingPosition(previousMovementMove),
        movementMoveStartingPosition(currentMovementMove), // If the lines are the same point
      )
    ) {
      // add the current movement move duration to the previous movement
      previousMovementMove.duration += currentMovementMove.duration

      // delete the current movement move
      movementMoves.splice(index, 1)
    }
  }

  const postPostProcessMovementMoveCount = movementMoves.length

  if (postPostProcessMovementMoveCount !== prePostProcessMovementMoveCount) {
    console.log(
      `Reduced movement count from ${prePostProcessMovementMoveCount} to ${postPostProcessMovementMoveCount}`,
    )
  }

  // Give every movement and fade a globally unique ID, just use the index,
  // The codec should wrap if it isn't big enough.
  for (let index = 0; index < movementMoves.length; index++) {
    const move = movementMoves[index];
    move.id = index
  }

  for (let index = 0; index < lightFades.length; index++) {
    const fade = lightFades[index];
    fade.id = index
  }

  return {
    movementMoves,
    lightMoves: lightFades,
    triggers,
  }
}

function lightFadeEndingColor(fade: LightMove): LightPoint {
  switch (fade.settings.type) {
    case LightMoveType.IMMEDIATE:
      return fade.points[0]
    case LightMoveType.RAMP:
      return fade.points[1]

    default:
      throw new Error(`Unkown LightMove type`)
  }
}

function lightFadeStartingColor(fade: LightMove): LightPoint {
  switch (fade.settings.type) {
    case LightMoveType.IMMEDIATE:
      return fade.points[0]
    case LightMoveType.RAMP:
      return fade.points[0]

    default:
      throw new Error(`Unkown LightMove type`)
  }
}

// Allow 10 bits of precision
const LIGHT_EPSILON = Math.pow(2, -10)

function lightFadePointsMatch(a: LightPoint, b: LightPoint) {
  return (
    Math.abs(a[0] - b[0]) < LIGHT_EPSILON &&
    Math.abs(a[1] - b[1]) < LIGHT_EPSILON &&
    Math.abs(a[2] - b[2]) < LIGHT_EPSILON
  )
}

function movementMoveEndingPosition(move: MovementMove): MovementPoint {
  switch (move.type) {
    case MovementMoveType.LINE:
      return move.points[1]

    case MovementMoveType.CATMULL_SPLINE:
    case MovementMoveType.CATMULL_SPLINE_LINEARISED:
      return move.points[2]

    case MovementMoveType.BEZIER_CUBIC:
    case MovementMoveType.BEZIER_CUBIC_LINEARISED:
      return move.points[3]

    case MovementMoveType.POINT_TRANSIT:
    case MovementMoveType.BEZIER_QUADRATIC:
    case MovementMoveType.BEZIER_QUADRATIC_LINEARISED:
    default:
      throw new Error(
        `Unimplemented post processing for movement type ${move.type}`,
      )
  }
}

function movementMoveStartingPosition(move: MovementMove): MovementPoint {
  switch (move.type) {
    case MovementMoveType.LINE:
      return move.points[0]

    case MovementMoveType.CATMULL_SPLINE:
    case MovementMoveType.CATMULL_SPLINE_LINEARISED:
      return move.points[1]

    case MovementMoveType.BEZIER_CUBIC:
    case MovementMoveType.BEZIER_CUBIC_LINEARISED:
      return move.points[0]

    case MovementMoveType.POINT_TRANSIT:
    case MovementMoveType.BEZIER_QUADRATIC:
    case MovementMoveType.BEZIER_QUADRATIC_LINEARISED:
    default:
      throw new Error(
        `Unimplemented post processing for movement type ${move.type}`,
      )
  }
}

// Allow precision to the micron
const MOVEMENT_EPSILON = 0.001 // mm

function movementMovePointsMatch(a: MovementPoint, b: MovementPoint) {
  return (
    Math.abs(a[0] - b[0]) < MOVEMENT_EPSILON &&
    Math.abs(a[1] - b[1]) < MOVEMENT_EPSILON &&
    Math.abs(a[2] - b[2]) < MOVEMENT_EPSILON
  )
}
