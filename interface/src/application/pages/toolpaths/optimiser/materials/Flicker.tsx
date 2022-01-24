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

export interface FlickerMaterialJSON {
  type: MATERIALS.FLICKER
  color_from: RGB
  color_to: RGB
}

export const FlickerMaterialDefaultJSON: FlickerMaterialJSON = {
  type: MATERIALS.FLICKER,
  color_from: [0, 0, 0],
  color_to: [1, 1, 1],
}

export function isFlickerMaterial(
  material: Material,
): material is FlickerMaterial {
  return material.type === MATERIALS.FLICKER
}

export function importFlickerMaterial(json: FlickerMaterialJSON) {
  const mat = new FlickerMaterial(
    [json.color_from[0], json.color_from[1], json.color_from[2]],
    [json.color_to[0], json.color_to[1], json.color_to[2]],
  )

  return mat
}

export class FlickerMaterial extends Material {
  readonly type = MATERIALS.FLICKER

  constructor(public start: RGB, public end: RGB) {
    super()
    this.pickColor = this.pickColor.bind(this)
  }

  pickColor() {
    return lerpRGB(this.start, this.end, Math.random())
  }

  public generateLightpath = (movement: Movement) => {
    const col1 = this.pickColor()
    const col2 = this.pickColor()

    const fade: PlannerLightMove = {
      duration: movement.getDuration(),
      type: LightMoveType.RAMP,
      points: [
        rgbToHsi(col1[0], col1[1], col1[2]),
        rgbToHsi(col2[0], col2[1], col2[2]),
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

      // Pick random colours

      const startCol = this.pickColor()
      const endCol = this.pickColor()

      // Add the line
      addColouredLine(start, end, startCol, endCol, movement.objectID)
    }
  }
}

export interface FlickerMaterialEditorProps {
  objectID: NodeID
  json: FlickerMaterialJSON
}

export function FlickerMaterialEditor(props: FlickerMaterialEditorProps) {
  return (
    <>
      FlickerMaterialEditorProps for {props.objectID} with color from [
      {props.json.color_from.join(', ')}] to [{props.json.color_to.join(', ')}]
    </>
  )
}
