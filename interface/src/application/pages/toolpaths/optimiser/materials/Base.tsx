import { NodeID } from '../../interface/RenderableTree'
import { VisualisationSettings } from '../../interface/state'
import { Settings } from '../../optimiser/settings'
import { LightMove } from './../hardware'
import { AddComponentCallback, AddLineCallback, Movement } from './../movements'

/**
 * Materials are currently stateless over the movements they're a part of
 */
export abstract class Material {
  abstract type: string

  /**
   * Given a movement ID, generate the light moves for this material.
   */
  abstract generateLightpath: (id: number, movement: Movement) => LightMove[]

  /**
   * Given a movement, sample some points and generate line segments, react components
   * for display in the 3D environment.
   */
  abstract generateThreeJSRepresentation: (
    movementIndex: number,
    movement: Movement,
    settings: Settings,
    visualisationSettings: VisualisationSettings,
    addColouredLine: AddLineCallback,
    addDottedLine: AddLineCallback,
    addReactComponent: AddComponentCallback,
  ) => void
}
