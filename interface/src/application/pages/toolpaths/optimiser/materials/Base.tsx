import { MathUtils, Vector3 } from 'three'
import { VisualisationSettings } from '../../interface/state'
import { Settings } from '../../optimiser/settings'
import { LightMoveType, PlannerLightMove } from './../hardware'
import {
  AddComponentCallback,
  AddLineCallback,
  Movement,
  MOVEMENT_TYPE,
  RGB,
} from './../movements'
import { annotateDrawOrder, rgbToHsi } from './utilities'

/**
 * Materials are currently stateless over the movements they're a part of
 */
export abstract class Material {
  abstract type: string

  /**
   * Calculates the color at a certain point T
   */
  public calculateColor = (
    movement: Movement,

    settings: Settings,
    visualisationSettings: VisualisationSettings,
    cameraPosition: Vector3,

    t: number,
  ): RGB => {
    return [0, 0, 0]
  }

  /**
   * Given a movement ID, generate the light moves for this material.
   */
  public generateLightpath = (
    movement: Movement,
    settings: Settings,
    visualisationSettings: VisualisationSettings,
    cameraPosition: Vector3,
    fromT: number,
    toT: number,
  ): PlannerLightMove[] => {
    const startColor = this.calculateColor(
      movement,
      settings,
      visualisationSettings,
      cameraPosition,
      fromT,
    )
    const endColor = this.calculateColor(
      movement,
      settings,
      visualisationSettings,
      cameraPosition,
      toT,
    )

    const fade: PlannerLightMove = {
      duration: movement.getDuration(),
      type: LightMoveType.RAMP,
      points: [
        rgbToHsi(startColor[0], startColor[1], startColor[2]),
        rgbToHsi(endColor[0], endColor[1], endColor[2]),
      ],
      num_points: 2,
    }

    return [fade]
  }

  /**
   * Given a movement, sample some points and generate line segments, react components
   * for display in the 3D environment.
   */
  public generateThreeJSRepresentation = (
    movementIndex: number,
    movement: Movement,
    settings: Settings,
    visualisationSettings: VisualisationSettings,
    cameraPosition: Vector3,
    addColouredLine: AddLineCallback,
    addDottedLine: AddLineCallback,
    addReactComponent: AddComponentCallback,
    fromT: number,
    toT: number,
  ) => {
    // Annotate draw order
    annotateDrawOrder(
      movementIndex,
      movement,
      visualisationSettings,
      addReactComponent,
    )

    // A simple color material draws the line segment(s) from the start to the end with a single color
    const numSegments =
      movement.type === MOVEMENT_TYPE.LINE ||
      movement.type === MOVEMENT_TYPE.POINT
        ? 1
        : visualisationSettings.curveSegments

    // For the number of segments,
    for (let index = 0; index < numSegments; index++) {
      const startT = MathUtils.mapLinear(index / numSegments, 0, 1, fromT, toT)
      const endT = MathUtils.mapLinear(
        (index + 1) / numSegments,
        0,
        1,
        fromT,
        toT,
      )

      // Sample points along the movement
      const start = movement.samplePoint(startT)
      const end = movement.samplePoint(endT)

      // Sample along the gradient between the two colours
      const startCol = this.calculateColor(
        movement,
        settings,
        visualisationSettings,
        cameraPosition,
        fromT,
      )
      const endCol = this.calculateColor(
        movement,
        settings,
        visualisationSettings,
        cameraPosition,
        endT,
      )

      // Add the line
      addColouredLine(start, end, startCol, endCol, movement.objectID)
    }
  }
}
