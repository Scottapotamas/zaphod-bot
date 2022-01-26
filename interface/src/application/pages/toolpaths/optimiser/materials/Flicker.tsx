import { RGB } from './../movements'
import { Material } from './Base'
import { NodeID } from '../../interface/RenderableTree'
import React from 'react'
import { lerpRGB, MATERIALS } from './utilities'

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
  }

  public calculateColor = () => {
    return lerpRGB(this.start, this.end, Math.random())
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
