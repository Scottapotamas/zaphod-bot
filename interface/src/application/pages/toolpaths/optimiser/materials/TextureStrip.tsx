import type { VisualisationSettings } from '../../interface/state'
import type { Settings } from '../../optimiser/settings'
import type { Movement, RGBA } from './../movements'
import { Material } from './Base'
import React from 'react'
import { lerpRGBA, MATERIALS } from './utilities'
import { Vector3 } from 'three'
import { ColorPicker } from '../../interface/ColorPicker'
import { Composition } from 'atomic-layout'

export interface TextureStripMaterialJSON {
  type: MATERIALS.TEXTURE_STRIP
  colors: RGBA[]
}

export const TextureStripMaterialDefaultJSON: TextureStripMaterialJSON = {
  type: MATERIALS.TEXTURE_STRIP,
  colors: [[1, 1, 1, 1]],
}

export function isTextureStripMaterial(
  material: Material,
): material is TextureStripMaterial {
  return material.type === MATERIALS.TEXTURE_STRIP
}

export class TextureStripMaterial extends Material {
  readonly type = MATERIALS.TEXTURE_STRIP

  constructor(public colors: RGBA[]) {
    super()
  }

  public calculateColor = (
    movement: Movement,
    settings: Settings,
    visualisationSettings: VisualisationSettings,
    cameraPosition: Vector3,
    t: number,
  ): RGBA => {
    // TODO: Interpolate between pixels?
    const index = Math.floor(t * (this.colors.length - 1))

    if (!this.colors[index]) {
      debugger
    }

    return this.colors[index]
  }
}

export interface TextureStripMaterialEditorProps {
  json: TextureStripMaterialJSON
  mutateJson: (writer: (json: TextureStripMaterialJSON) => void) => void
}

export function TextureStripMaterialEditor(
  props: TextureStripMaterialEditorProps,
) {
  return (
    <Composition templateCols="1fr 2fr" gap="1em" alignItems="center">
      cols: {JSON.stringify(props.json.colors)}
    </Composition>
  )
}
