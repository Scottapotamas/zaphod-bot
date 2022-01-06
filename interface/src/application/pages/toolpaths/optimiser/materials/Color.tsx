import { MathUtils, Vector3 } from 'three'
import { VisualisationSettings } from '../../interface/state'
import { Settings } from '../../optimiser/settings'
import { LightMove, LightMoveType } from './../hardware'
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

export interface ColorMaterialJSON {
  type: MATERIALS.COLOR
  color: [number, number, number, number]
}

export const ColorMaterialDefaultJSON: ColorMaterialJSON = {
  type: MATERIALS.COLOR,
  color: [1, 1, 1, 1],
}

export function isSimpleColorMaterial(
  material: Material,
): material is SimpleColorMaterial {
  return material.type === MATERIALS.COLOR
}

export function importColorMaterial(json: ColorMaterialJSON) {
  const mat = new SimpleColorMaterial([
    json.color[0],
    json.color[1],
    json.color[2],
  ])

  return mat
}

export class SimpleColorMaterial extends Material {
  readonly type = MATERIALS.COLOR

  constructor(public color: RGB) {
    super()
  }

  public generateLightpath = (id: number, movement: Movement) => {
    const fade: LightMove = {
      id,
      duration: movement.getDuration(),
      type: LightMoveType.IMMEDIATE,
      points: [rgbToHsi(this.color[0], this.color[1], this.color[2])],
      num_points: 1,
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
      const startT = index / numSegments
      const endT = (index + 1) / numSegments

      // Sample points along the movement
      const start = movement.samplePoint(startT)
      const end = movement.samplePoint(endT)

      // Add the line
      addColouredLine(start, end, this.color, this.color, movement.objectID)
    }
  }
}

export interface ColorMaterialEditorProps {
  objectID: NodeID
  json: ColorMaterialJSON
}

export function ColorMaterialEditor(props: ColorMaterialEditorProps) {
  return (
    <>
      ColorMaterialEditor for {props.objectID} with color [
      {props.json.color.join(', ')}]
    </>
  )
}
