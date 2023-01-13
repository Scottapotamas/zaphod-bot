import type { VisualisationSettings } from '../../interface/state'
import type { Settings } from '../../optimiser/settings'
import type { Movement, RGBA } from './../movements'
import { Material } from './Base'
import React from 'react'
import { lerpRGBA, MATERIALS } from './utilities'
import { Vector3 } from 'three'
import { ColorPicker } from '../../interface/ColorPicker'
import { Composition } from 'atomic-layout'

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

export function isColorRampMaterial(material: Material): material is ColorRampMaterial {
  return material.type === MATERIALS.COLOR_RAMP
}

export function importColorRampMaterial(json: ColorRampMaterialJSON) {
  const mat = new ColorRampMaterial(
    [json.color_from[0], json.color_from[1], json.color_from[2], json.color_from[3]],
    [json.color_to[0], json.color_to[1], json.color_to[2], json.color_to[3]],
  )

  return mat
}

export class ColorRampMaterial extends Material {
  readonly type = MATERIALS.COLOR_RAMP

  constructor(public fromCol: RGBA, public toCol: RGBA) {
    super()
  }

  public calculateColor = (
    movement: Movement,
    settings: Settings,
    visualisationSettings: VisualisationSettings,
    cameraPosition: Vector3,
    t: number,
  ): RGBA => {
    return lerpRGBA(this.fromCol, this.toCol, t)
  }
}

export interface ColorRampMaterialEditorProps {
  json: ColorRampMaterialJSON
  mutateJson: (writer: (json: ColorRampMaterialJSON) => void) => void
}

export function ColorRampMaterialEditor(props: ColorRampMaterialEditorProps) {
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
