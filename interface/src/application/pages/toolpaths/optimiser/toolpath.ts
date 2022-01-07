import { LightMove, MovementMove } from './hardware'
import { DenseMovements } from './movements'

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

  let id = 0
  // each movement should have a generateToolpath method
  for (const movement of denseMovements) {
    // Increment the ID
    id++

    // Build the hardware moves
    for (const movementMove of movement.generateToolpath(id)) {
      movementMoves.push(movementMove)
    }

    // Build the light moves
    for (const lightMove of movement.material.generateLightpath(id, movement)) {
      lightMoves.push(lightMove)
    }
  }

  return {
    movementMoves,
    lightMoves,
  }
}
