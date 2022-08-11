import { RGB, RGBA } from './../movements'
import { Material } from './Base'
import { NodeID } from '../../interface/RenderableTree'
import React from 'react'
import { lerpRGBA, MATERIALS } from './utilities'
import { ColorPicker } from '../../interface/ColorPicker'
import { Composition, Box } from 'atomic-layout'

export interface FlickerMaterialJSON {
  type: MATERIALS.FLICKER
  color_from: RGBA
  color_to: RGBA
}

export const FlickerMaterialDefaultJSON: FlickerMaterialJSON = {
  type: MATERIALS.FLICKER,
  color_from: [0, 0, 0, 1],
  color_to: [1, 1, 1, 1],
}

export function isFlickerMaterial(
  material: Material,
): material is FlickerMaterial {
  return material.type === MATERIALS.FLICKER
}

export function importFlickerMaterial(json: FlickerMaterialJSON) {
  const mat = new FlickerMaterial(
    [json.color_from[0], json.color_from[1], json.color_from[2], json.color_from[3]],
    [json.color_to[0], json.color_to[1], json.color_to[2], json.color_to[3]],
  )

  return mat
}

export class FlickerMaterial extends Material {
  readonly type = MATERIALS.FLICKER

  constructor(public start: RGBA, public end: RGBA) {
    super()
  }

  public calculateColor = () => {
    return lerpRGBA(this.start, this.end, Math.random())
  }
}

export interface FlickerMaterialEditorProps {
  json: FlickerMaterialJSON
  mutateJson: (writer: (json: FlickerMaterialJSON) => void) => void
}

export function FlickerMaterialEditor(props: FlickerMaterialEditorProps) {
  return (
    <Composition templateCols="1fr 2fr" gap="1em" alignItems="center">
      From:
      <ColorPicker
        defaultColor={props.json.color_from}
        writer={col => {
          props.mutateJson(json => {
            json.color_from = col
          })
        }}
      />
      To:
      <ColorPicker
        defaultColor={props.json.color_to}
        writer={col => {
          props.mutateJson(json => {
            json.color_to = col
          })
        }}
      />
    </Composition>
  )
}
