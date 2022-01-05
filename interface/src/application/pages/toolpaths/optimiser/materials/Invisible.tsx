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

export class InvisibleMaterial extends Material {
  readonly type = MATERIALS.INVISIBLE

  constructor(public color: RGB) {
    super()
  }

  public generateLightpath = (id: number, movement: Movement) => {
    const fade: LightMove = {
      id,
      duration: movement.getDuration(),
      type: LightMoveType.IMMEDIATE,
      points: [[0, 0, 0]], // Set a light move of the LED being off
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
    addTransitionLine: AddLineCallback,
    addReactComponent: AddComponentCallback,
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
      const startT = index / numSegments
      const endT = (index + 1) / numSegments

      // Sample points along the movement
      const start = movement.samplePoint(startT)
      const end = movement.samplePoint(endT)

      // Add the line
      addTransitionLine(start, end, this.color, this.color, movement.objectID)
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
