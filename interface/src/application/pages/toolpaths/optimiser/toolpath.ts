import { DenseMovements } from './movements'
import { LightMove, MovementMove } from '../../../../application/typedState'

export interface Toolpath {
  movementMoves: MovementMove[]
  lightMoves: LightMove[]
}

/**
 * Take a densely ordered set of movements and convert them to a toolpath.
 *
 * Done just before rendering on the UI thread.
 */
export function toolpath(denseMovements: DenseMovements): Toolpath {
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
      timestamp: movementTimestamp,
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
    for (const lightMove of movement.material.generateLightpath(movement)) {
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

  return {
    movementMoves,
    lightMoves,
  }
}
