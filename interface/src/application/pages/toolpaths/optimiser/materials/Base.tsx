import { MathUtils, Vector3 } from 'three'
import type { VisualisationSettings } from '../../interface/state'
import type { Settings } from '../../optimiser/settings'
import { LightMoveType, PlannerLightMove } from './../hardware'
import { MOVEMENT_TYPE } from './../movement_types'
import {
  AddComponentCallback,
  AddLineCallback,
  isTransition,
  Movement,
  RGBA,
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
  ): RGBA => {
    return [0, 0, 0, 1]
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

    const startHSI = rgbToHsi(startColor[0], startColor[1], startColor[2])
    const endHSI = rgbToHsi(endColor[0], endColor[1], endColor[2])

    const preI = startHSI[2]

    // Use the alpha value to multiply out the intensity??
    // TODO: do a better colour model
    startHSI[2] = startHSI[2] * startColor[3]
    endHSI[2] = endHSI[2] * endColor[3]

    const fade: PlannerLightMove = {
      duration: movement.getDuration(),
      settings: { type: LightMoveType.RAMP },
      points: [
        startHSI,
        endHSI, //
      ],
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
    spatialRenderFrom: number, // 0-1 spatial render up from this point
    spatialRenderTo: number, // 0-1 spatial render up to this point
  ) => {
    // Annotate draw order
    annotateDrawOrder(
      movementIndex,
      movement,
      visualisationSettings,
      addReactComponent,
    )

    // Lines and points only need a single segment, everything else create up to 10.
    const numSegments =
      movement.type === MOVEMENT_TYPE.LINE ||
      movement.type === MOVEMENT_TYPE.POINT
        ? 1
        : Math.max(Math.ceil(movement.getLength() / 2), 10)

    // For the number of segments,
    for (let index = 0; index < numSegments; index++) {
      const startTSpatial = MathUtils.mapLinear(
        index / numSegments,
        0,
        1,
        spatialRenderFrom,
        spatialRenderTo,
      )
      const endTSpatial = MathUtils.mapLinear(
        (index + 1) / numSegments,
        0,
        1,
        spatialRenderFrom,
        spatialRenderTo,
      )

      const startTMat = MathUtils.mapLinear(
        index / numSegments,
        0,
        1,
        fromT,
        toT,
      )
      const endTMat = MathUtils.mapLinear(
        (index + 1) / numSegments,
        0,
        1,
        fromT,
        toT,
      )

      // Sample points along the movement
      const start = movement.samplePoint(startTSpatial)
      const end = movement.samplePoint(endTSpatial)

      // Sample along the gradient between the two colours
      const startCol = this.calculateColor(
        movement,
        settings,
        visualisationSettings,
        cameraPosition,
        startTMat,
      )
      const endCol = this.calculateColor(
        movement,
        settings,
        visualisationSettings,
        cameraPosition,
        Math.min(endTMat, endTSpatial), // If the spatial end is less than the mat end, just clamp at the spatial end?
      )

      // Add the line
      if (isTransition(movement)) {
        addDottedLine(
          start,
          end,
          startCol,
          endCol,
          movementIndex,
          movement.objectID,
        )
      } else {
        addColouredLine(
          start,
          end,
          startCol,
          endCol,
          movementIndex,
          movement.objectID,
        )
      }
    }
  }
}
