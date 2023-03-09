import { MathUtils, Vector3 } from 'three'
import type { Settings } from '../../optimiser/settings'
import { PlannerLightMove, LightMoveType } from './../hardware'
import type {
  AddComponentCallback,
  AddLineCallback,
  Movement,
  RGBA,
} from './../movements'
import { MOVEMENT_TYPE } from './../movement_types'
import { Material } from './Base'
import React from 'react'
import { annotateDrawOrder, MATERIALS } from './utilities'
import type { VisualisationSettings } from '../../interface/state'
import { Composition } from 'atomic-layout'

import { ColorPicker } from '../../interface/ColorPicker'
export interface InvisibleMaterialJSON {
  type: MATERIALS.INVISIBLE
  // For the color in the UI
  color: RGBA
}

export const InvisibleMaterialDefaultJSON: InvisibleMaterialJSON = {
  type: MATERIALS.INVISIBLE,
  color: [0.3, 0.3, 0.3, 1],
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
    json.color[3],
  ])

  return mat
}

export function invisibleLightFade(duration: number) {
  const fade: PlannerLightMove = {
    duration: duration,
    settings: { type: LightMoveType.IMMEDIATE },
    points: [[0, 0, 0]], // Set a light move of the LED being off
  }
  return fade
}

export class InvisibleMaterial extends Material {
  readonly type = MATERIALS.INVISIBLE

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
    return [this.color[0], this.color[1], this.color[2], 0]
  }
}

export interface InvisibleMaterialEditorProps {
  json: InvisibleMaterialJSON
  mutateJson: (writer: (json: InvisibleMaterialJSON) => void) => void
}

export function InvisibleMaterialEditor(props: InvisibleMaterialEditorProps) {
  return (
    <Composition templateCols="1fr 2fr" gap="1em" alignItems="center">
      Viewer Color
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
