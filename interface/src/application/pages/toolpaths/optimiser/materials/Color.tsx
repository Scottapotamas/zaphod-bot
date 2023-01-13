import type { VisualisationSettings } from '../../interface/state'
import type { Settings } from '../../optimiser/settings'
import type { Movement, RGBA } from './../movements'
import { Material } from './Base'
import React from 'react'
import { MATERIALS } from './utilities'
import { Vector3 } from 'three'
import { ColorPicker } from '../../interface/ColorPicker'
import { Composition } from 'atomic-layout'

export interface ColorMaterialJSON {
  type: MATERIALS.COLOR
  color: RGBA
}

export const ColorMaterialDefaultJSON: ColorMaterialJSON = {
  type: MATERIALS.COLOR,
  color: [1, 1, 1, 1],
}

export function isSimpleColorMaterial(material: Material): material is SimpleColorMaterial {
  return material.type === MATERIALS.COLOR
}

export function importColorMaterial(json: ColorMaterialJSON) {
  const mat = new SimpleColorMaterial([json.color[0], json.color[1], json.color[2], json.color[3]])

  return mat
}

export class SimpleColorMaterial extends Material {
  readonly type = MATERIALS.COLOR

  constructor(public color: RGBA) {
    super()
  }

  public calculateColor = (
    movement: Movement,
    settings: Settings,
    visualisationSettings: VisualisationSettings,
    cameraPosition: Vector3,
    t: number,
  ): RGBA => {
    return this.color
  }
}

export interface ColorMaterialEditorProps {
  json: ColorMaterialJSON
  mutateJson: (writer: (json: ColorMaterialJSON) => void) => void
}

export function ColorMaterialEditor(props: ColorMaterialEditorProps) {
  return (
    <Composition templateCols="1fr 2fr" gap="1em" alignItems="center">
      Color
      <ColorPicker
        defaultColor={props.json.color}
        writer={col => {
          props.mutateJson(json => {
            json.color = col
          })
        }}
      />
    </Composition>
  )
}
