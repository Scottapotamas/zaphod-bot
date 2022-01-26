import {
  incrementViewportFrameVersion,
  setSetting,
  VisualisationSettings,
} from '../../interface/state'
import { Settings } from '../../optimiser/settings'
import { Movement, RGB, RGBA } from './../movements'
import { Material } from './Base'
import { NodeID } from '../../interface/RenderableTree'
import React from 'react'
import { lerpRGB, MATERIALS } from './utilities'
import { Vector3 } from 'three'
import { ColorPicker } from '../../interface/ColorPicker'
import { Composition, Box } from 'atomic-layout'

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
