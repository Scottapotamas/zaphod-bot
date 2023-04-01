import { MathUtils, Vector3 } from 'three'
import type { VisualisationSettings } from '../../interface/state'
import type { Settings } from '../settings'
import { Movement, RGBA } from '../movements'
import { Material } from './Base'
import { Tag } from '@blueprintjs/core'
import React from 'react'
import { lerpRGBA, MATERIALS } from './utilities'
import { IconNames } from '@blueprintjs/icons'
import { Composition } from 'atomic-layout'

export interface FlippedMaterialJSON {
  type: MATERIALS.FLIPPED
}

export const FlippedMaterialDefaultJSON: FlippedMaterialJSON = {
  type: MATERIALS.FLIPPED,
}

export function isFlippedMaterial(
  material: Material,
): material is FlippedMaterial {
  return material.type === MATERIALS.FLIPPED
}

export function importFlippedMaterial(json: FlippedMaterialJSON) {
  const mat = new FlippedMaterial()

  return mat
}

/**
 * This material displays the predicted flipped through the lines as a colour from red, orange, blue, black, with warmer colours being shorter flippeds.
 */
export class FlippedMaterial extends Material {
  readonly type = MATERIALS.FLIPPED

  constructor() {
    super()
  }

  public calculateColor = (
    movement: Movement,
    settings: Settings,
    visualisationSettings: VisualisationSettings,
    cameraPosition: Vector3,
    t: number,
  ): RGBA => {
    return calculateColorFromFlipped(movement.isFlipped)
  }
}

const CERULEAN: RGBA = [12 / 255, 81 / 255, 116 / 255, 1] // #0C5174
const ORANGE: RGBA = [217 / 255, 130 / 255, 43 / 255, 1] // #D9822B

function calculateColorFromFlipped(flipped: boolean) {
  return flipped ? ORANGE : CERULEAN
}

export interface FlippedMaterialEditorProps {
  json: FlippedMaterialJSON
  mutateJson: (writer: (json: FlippedMaterialJSON) => void) => void
}

const flippedGradient = {
  height: 30,
  width: '100%',
  background:
    'linear-gradient(90deg, rgba(12,81,116,1) 1%, rgba(12,81,116,1) 49%, rgba(217,130,43,1) 51%, rgba(217,130,43,1) 99%)',
  display: 'block',
}

const flippedInfo = {
  marginTop: 10,
  display: 'grid',
  gridTemplateColumns: '1fr 1fr 1fr',
}

export function FlippedMaterialEditor(props: FlippedMaterialEditorProps) {
  return (
    <Composition templateCols="1fr 2fr" gap="1em" alignItems="center">
      <div>
        <div style={flippedGradient}></div>
        <div style={flippedInfo}>
          <div>
            <Tag intent="primary" icon={IconNames.DOT}>
              Not flipped
            </Tag>
          </div>
          <div style={{ textAlign: 'right' }}>
          <Tag intent="danger" rightIcon={IconNames.SWAP_HORIZONTAL}>
              Flipped
            </Tag>
          </div>
        </div>
      </div>
    </Composition>
  )
}
