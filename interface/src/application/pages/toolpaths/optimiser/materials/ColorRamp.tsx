import { MathUtils, Vector3 } from 'three'
import { VisualisationSettings } from '../../interface/state'
import { Settings } from '../../optimiser/settings'
import { PlannerLightMove, LightMoveType } from './../hardware'
import {
  AddComponentCallback,
  AddLineCallback,
  isPointTransition,
  isTransition,
  Movement,
  MOVEMENT_TYPE,
  RGB,
} from './../movements'
import { Material } from './Base'
import { Html } from '@react-three/drei'
import { Intent, Tag } from '@blueprintjs/core'
import { NodeID } from '../../interface/RenderableTree'
import React from 'react'
import { annotateDrawOrder, lerpRGB, MATERIALS, rgbToHsi } from './utilities'

export interface ColorRampMaterialJSON {
  type: MATERIALS.COLOR_RAMP
  colorFrom: [number, number, number]
  colorTo: [number, number, number]
}

export const ColorRampMaterialDefaultJSON: ColorRampMaterialJSON = {
  type: MATERIALS.COLOR_RAMP,
  colorFrom: [1, 1, 1],
  colorTo: [1, 1, 1],
}

export function isColorRampMaterial(
  material: Material,
): material is ColorRampMaterial {
  return material.type === MATERIALS.COLOR_RAMP
}

export function importColorRampMaterial(json: ColorRampMaterialJSON) {
  const mat = new ColorRampMaterial(
    [json.colorFrom[0], json.colorFrom[1], json.colorFrom[2]],
    [json.colorTo[0], json.colorTo[1], json.colorTo[2]],
  )

  return mat
}

export class ColorRampMaterial extends Material {
  readonly type = MATERIALS.COLOR_RAMP

  constructor(public start: RGB, public end: RGB) {
    super()
  }

  public generateLightpath = (movement: Movement) => {
    const fade: PlannerLightMove = {
      duration: movement.getDuration(),
      type: LightMoveType.RAMP,
      points: [
        rgbToHsi(this.start[0], this.start[1], this.start[2]),
        rgbToHsi(this.end[0], this.end[1], this.end[2]),
      ],
      num_points: 2,
    }

    return [fade]
  }

  public generateThreeJSRepresentation = (
    movementIndex: number,
    movement: Movement,
    settings: Settings,
    visualisationSettings: VisualisationSettings,
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
      const startCol = lerpRGB(this.start, this.end, startT)
      const endCol = lerpRGB(this.start, this.end, endT)

      // Add the line
      addColouredLine(start, end, startCol, endCol, movement.objectID)
    }
  }
}

export interface ColorRampMaterialEditorProps {
  objectID: NodeID
  json: ColorRampMaterialJSON
}

export function ColorRampMaterialEditor(props: ColorRampMaterialEditorProps) {
  return (
    <>
      ColorRampMaterialEditorProps for {props.objectID} with color from [
      {props.json.colorFrom.join(', ')}] to [{props.json.colorTo.join(', ')}]
    </>
  )
}
