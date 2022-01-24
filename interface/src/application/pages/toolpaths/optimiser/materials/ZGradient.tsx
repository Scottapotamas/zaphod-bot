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

export interface ZGradientMaterialJSON {
  type: MATERIALS.Z_GRADIENT
  color_from: RGB
  color_to: RGB
  z_from: number
  z_to: number
}

export const ZGradientMaterialDefaultJSON: ZGradientMaterialJSON = {
  type: MATERIALS.Z_GRADIENT,
  color_from: [0, 0, 0],
  color_to: [1, 1, 1],
  z_from: 0,
  z_to: 1,
}

export function isZGradientMaterial(
  material: Material,
): material is ZGradientMaterial {
  return material.type === MATERIALS.Z_GRADIENT
}

export function importZGradientMaterial(json: ZGradientMaterialJSON) {
  const mat = new ZGradientMaterial(
    [json.color_from[0], json.color_from[1], json.color_from[2]],
    [json.color_to[0], json.color_to[1], json.color_to[2]],
    json.z_from,
    json.z_to,
  )

  return mat
}

export class ZGradientMaterial extends Material {
  readonly type = MATERIALS.Z_GRADIENT

  constructor(
    public start: RGB,
    public end: RGB,
    public zFrom: number,
    public zTo: number,
  ) {
    super()
    this.calculateColor = this.calculateColor.bind(this)
  }

  calculateColor(z: number) {
    const clamped = MathUtils.clamp(
      MathUtils.mapLinear(z, this.zFrom, this.zTo, 0, 1),
      0,
      1,
    )

    return lerpRGB(this.start, this.end, clamped)
  }

  public generateLightpath = (movement: Movement) => {
    const col1 = this.calculateColor(movement.getStart().z)
    const col2 = this.calculateColor(movement.getEnd().z)

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
      const startCol = this.calculateColor(start.z)
      const endCol = this.calculateColor(end.z)

      // Add the line
      addColouredLine(start, end, startCol, endCol, movement.objectID)
    }
  }
}

export interface ZGradientMaterialEditorProps {
  objectID: NodeID
  json: ZGradientMaterialJSON
}

export function ZGradientMaterialEditor(props: ZGradientMaterialEditorProps) {
  return (
    <>
      ZGradientMaterialEditorProps for {props.objectID} with color from [
      {props.json.color_from.join(', ')}] to [{props.json.color_to.join(', ')}]
      over z{props.json.z_from} to z{props.json.z_to}
    </>
  )
}
