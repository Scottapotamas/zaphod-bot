import { VisualisationSettings } from '../../interface/state'
import { Settings } from '../../optimiser/settings'
import { Movement, RGB, RGBA } from './../movements'
import { Material } from './Base'
import { NodeID } from '../../interface/RenderableTree'
import React from 'react'
import { MATERIALS } from './utilities'
import { Vector3 } from 'three'

export interface ColorMaterialJSON {
  type: MATERIALS.COLOR
  color: RGBA
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

  public calculateColor = (
    movement: Movement,
    settings: Settings,
    visualisationSettings: VisualisationSettings,
    cameraPosition: Vector3,
    t: number,
  ): RGB => {
    return this.color
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
