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

export interface DurationMaterialJSON {
  type: MATERIALS.DURATION
}

export const DurationMaterialDefaultJSON: DurationMaterialJSON = {
  type: MATERIALS.DURATION,
}

export function isDurationMaterial(
  material: Material,
): material is DurationMaterial {
  return material.type === MATERIALS.DURATION
}

export function importDurationMaterial(json: DurationMaterialJSON) {
  const mat = new DurationMaterial()

  return mat
}

/**
 * This material displays the predicted duration through the lines as a colour from red, orange, blue, black, with warmer colours being shorter durations.
 */
export class DurationMaterial extends Material {
  readonly type = MATERIALS.DURATION

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
    return calculateColorFromDuration(movement.getDuration())
  }
}

const BLACK: RGBA = [0, 0, 0, 1]
const CERULEAN: RGBA = [12 / 255, 81 / 255, 116 / 255, 1] // #0C5174
const ORANGE: RGBA = [217 / 255, 130 / 255, 43 / 255, 1] // #D9822B
const RED: RGBA = [219 / 255, 55 / 255, 55 / 255, 1] // #DB3737
const PURPLE: RGBA = [86 / 255, 66 / 255, 166 / 255, 1] // #5642A6

function calculateColorFromDuration(duration: number) {
  if (duration === 0 || !Number.isInteger(duration)) {
    // should never happen
    return PURPLE
  }

  if (duration <= 1) {
    // 1ms moves should be red
    return RED
  }

  if (duration <= 10) {
    return lerpRGBA(
      RED,
      ORANGE,
      MathUtils.mapLinear(duration, 1, 10, 0, 1),
    )
  }

  if (duration <= 20) {
    return lerpRGBA(
      ORANGE,
      CERULEAN,
      MathUtils.mapLinear(duration, 10, 20, 0, 1),
    )
  }

  if (duration <= 50) {
    return lerpRGBA(
      CERULEAN,
      BLACK,
      MathUtils.mapLinear(duration, 20, 50, 0, 1),
    )
  }

  return BLACK
}

export interface DurationMaterialEditorProps {
  json: DurationMaterialJSON
  mutateJson: (writer: (json: DurationMaterialJSON) => void) => void
}

const durationGradient = {
  height: 30,
  width: '100%',
  background:
    'linear-gradient(90deg, rgba(219,55,55,1) 3.5%, rgba(217,130,43,1) 37%, rgba(12,81,116,1) 66%, rgba(0,0,0,1) 96%)',
  display: 'block',
}

const durationInfo = {
  marginTop: 10,
  display: 'grid',
  gridTemplateColumns: '1fr 1fr 1fr',
}

export function DurationMaterialEditor(props: DurationMaterialEditorProps) {
  return (
    <Composition templateCols="1fr 2fr" gap="1em" alignItems="center">
      <div>
        <div style={durationGradient}></div>
        <div style={durationInfo}>
          <div>
            <Tag intent="danger" icon={IconNames.ARROW_UP}>
              1ms
            </Tag>
          </div>
          <div>
            <Tag intent="primary" rightIcon={IconNames.ARROW_UP} style={{marginLeft: 40}}>
              20ms
            </Tag>
          </div>
          <div style={{ textAlign: 'right' }}>
            <Tag intent="none" rightIcon={IconNames.ARROW_UP} minimal>
              50ms
            </Tag>
          </div>
        </div>
      </div>
    </Composition>
  )
}
