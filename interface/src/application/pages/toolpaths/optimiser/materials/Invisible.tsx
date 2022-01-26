import { MathUtils, Vector3 } from 'three'
import { VisualisationSettings } from '../../interface/state'
import { Settings } from '../../optimiser/settings'
import { PlannerLightMove, LightMoveType } from './../hardware'
import {
  AddComponentCallback,
  AddLineCallback,
  Movement,
  MOVEMENT_TYPE,
  RGB,
} from './../movements'
import { Material } from './Base'
import { NodeID } from '../../interface/RenderableTree'
import React from 'react'
import { annotateDrawOrder, MATERIALS } from './utilities'

export interface InvisibleMaterialJSON {
  type: MATERIALS.INVISIBLE
  // For the color in the UI
  color: [number, number, number]
}

export const InvisibleMaterialDefaultJSON: InvisibleMaterialJSON = {
  type: MATERIALS.INVISIBLE,
  color: [0.3, 0.3, 0.3],
}

export function isInvisibleMaterial(
  material: Material,
): material is InvisibleMaterial {
  return material.type === MATERIALS.INVISIBLE
}

export function importInvisibleMaterial(json: InvisibleMaterialJSON) {
  const mat = new InvisibleMaterial([
    json.color[0],
    json.color[1],
    json.color[2],
  ])

  return mat
}

export function invisibleLightFade(duration: number) {
  const fade: PlannerLightMove = {
    duration: duration,
    type: LightMoveType.IMMEDIATE,
    points: [[0, 0, 0]], // Set a light move of the LED being off
    num_points: 1,
  }
  return fade
}

export class InvisibleMaterial extends Material {
  readonly type = MATERIALS.INVISIBLE

  constructor(public color: RGB) {
    super()
  }

  public generateLightpath = (
    movement: Movement,
    settings: Settings,
    visualisationSettings: VisualisationSettings,
    cameraPosition: Vector3,
    fromT: number,
    toT: number,
  ) => {
    const fade = invisibleLightFade(movement.getDuration())

    return [fade]
  }

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

    // Despite being invisible in hardware, we still want to draw this in the UI
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

      // Add the line
      addDottedLine(start, end, this.color, this.color, movement.objectID)
    }
  }
}

export interface InvisibleMaterialEditorProps {
  objectID: NodeID
  json: InvisibleMaterialJSON
}

export function InvisibleMaterialEditor(props: InvisibleMaterialEditorProps) {
  return (
    <>
      InvisibleMaterialEditorProps for {props.objectID} with visualisation color
      [{props.json.color.join(', ')}]
    </>
  )
}
