import { VisualisationSettings } from '../../interface/state'
import { Settings } from '../../optimiser/settings'
import { Movement, RGB, RGBA } from './../movements'
import { Material } from './Base'
import { NodeID } from '../../interface/RenderableTree'
import React from 'react'
import { lerpRGB, MATERIALS } from './utilities'
import { Vector3 } from 'three'

export interface ColorRampMaterialJSON {
  type: MATERIALS.COLOR_RAMP
  color_from: RGBA
  color_to: RGBA
}

export const ColorRampMaterialDefaultJSON: ColorRampMaterialJSON = {
  type: MATERIALS.COLOR_RAMP,
  color_from: [1, 1, 1, 1],
  color_to: [1, 1, 1, 1],
}

export function isColorRampMaterial(
  material: Material,
): material is ColorRampMaterial {
  return material.type === MATERIALS.COLOR_RAMP
}

export function importColorRampMaterial(json: ColorRampMaterialJSON) {
  const mat = new ColorRampMaterial(
    [json.color_from[0], json.color_from[1], json.color_from[2]],
    [json.color_to[0], json.color_to[1], json.color_to[2]],
  )

  return mat
}

export class ColorRampMaterial extends Material {
  readonly type = MATERIALS.COLOR_RAMP

  constructor(public start: RGB, public end: RGB) {
    super()
  }

  public calculateColor = (
    movement: Movement,
    settings: Settings,
    visualisationSettings: VisualisationSettings,
    cameraPosition: Vector3,
    t: number,
  ): RGB => {
    return lerpRGB(this.start, this.end, t)
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
      {props.json.color_from.join(', ')}] to [{props.json.color_to.join(', ')}]
    </>
  )
}
