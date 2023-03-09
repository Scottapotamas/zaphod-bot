import { MathUtils, Vector3 } from 'three'
import type { VisualisationSettings } from '../../interface/state'
import type { Settings } from '../../optimiser/settings'
import { PlannerLightMove, LightMoveType } from './../hardware'
import {
  AddComponentCallback,
  AddLineCallback,
  isLine,
  isTransition,
  Movement,
  predictVelocityAtT,
  RGBA,
} from './../movements'
import { MOVEMENT_TYPE } from './../movement_types'
import { Material } from './Base'
import { Tag } from '@blueprintjs/core'
import React from 'react'
import {
  generateHtmlTagFromAveragePosition,
  lerpRGBA,
  MATERIALS,
  movementTypeToIntent,
  movementTypeToLetter,
  rgbToHsi,
} from './utilities'
import { IconNames } from '@blueprintjs/icons'
import { Composition, Box } from 'atomic-layout'

export interface VelocityMaterialJSON {
  type: MATERIALS.VELOCITY
}

export const VelocityMaterialDefaultJSON: VelocityMaterialJSON = {
  type: MATERIALS.VELOCITY,
}

export function isVelocityMaterial(
  material: Material,
): material is VelocityMaterial {
  return material.type === MATERIALS.VELOCITY
}

export function importVelocityMaterial(json: VelocityMaterialJSON) {
  const mat = new VelocityMaterial()

  return mat
}

/**
 * This material displays the predicted velocity through the lines as a colour from black, green, orange, red.
 */
export class VelocityMaterial extends Material {
  readonly type = MATERIALS.VELOCITY

  constructor() {
    super()
  }

  // TODO: Have the Delta have a material that does this with its own information
  // public generateLightpath = (
  //   movement: Movement,
  //   settings: Settings,
  //   visualisationSettings: VisualisationSettings,
  //   cameraPosition: Vector3,
  //   fromT: number,
  //   toT: number,
  // ) => {

  //   const fade: PlannerLightMove = {
  //     duration: movement.getDuration(),
  //     settings: { type: LightMoveType.IMMEDIATE },
  //     points: [rgbToHsi(1, 1, 1)],
  //   }

  //   return [fade]
  // }

  public calculateColor = (
    movement: Movement,
    settings: Settings,
    visualisationSettings: VisualisationSettings,
    cameraPosition: Vector3,
    t: number,
  ): RGBA => {
    const intendedSpeed = settings.optimisation.maxSpeed

    const speed = predictVelocityAtT(movement, t)
    const col = calculateColorFromSpeed(speed, intendedSpeed)

    return col
  }
}

const LIMIT_SPEED = 500 // mm/s, the hardware limit, don't go over this under any circumstances

const BLACK: RGBA = [0, 0, 0, 1]
const GREEN: RGBA = [15 / 255, 153 / 255, 96 / 255, 1] // #0F9960
const ORANGE: RGBA = [217 / 255, 130 / 255, 43 / 255, 1] // #D9822B
const RED: RGBA = [219 / 255, 55 / 255, 55 / 255, 1] // #DB3737

function calculateColorFromSpeed(speed: number, intendedSpeed: number) {
  if (speed < intendedSpeed) {
    // Speeds from 0 to the intended speed are varying degrees of green
    return lerpRGBA(
      BLACK,
      GREEN,
      MathUtils.mapLinear(speed, 0, intendedSpeed, 0, 1),
    )
  } else if (speed < LIMIT_SPEED) {
    // Speeds above the intended speed but below the hardware limit become more orange
    return lerpRGBA(
      GREEN,
      ORANGE,
      MathUtils.mapLinear(speed, intendedSpeed, LIMIT_SPEED, 0, 1),
    )
  } else {
    // Any speed above the limit is red
    return RED
  }
}

export interface VelocityMaterialEditorProps {
  json: VelocityMaterialJSON
  mutateJson: (writer: (json: VelocityMaterialJSON) => void) => void
}

const velocityGradient = {
  height: 30,
  width: '100%',
  background:
    'linear-gradient(90deg, rgba(0,0,0,1) 3.5%, rgba(15,153,96,1) 37%, rgba(217,130,43,1) 66%, rgba(219,55,55,1) 96%)',
  display: 'block',
}

const velocityInfo = {
  marginTop: 10,
  display: 'grid',
  gridTemplateColumns: '1fr 1fr 1fr',
}

export function VelocityMaterialEditor(props: VelocityMaterialEditorProps) {
  return (
    <Composition templateCols="1fr 2fr" gap="1em" alignItems="center">
      <div>
        <div style={velocityGradient}></div>
        <div style={velocityInfo}>
          <div>
            <Tag intent="none" icon={IconNames.ARROW_UP} minimal>
              Stationary
            </Tag>
          </div>
          <div>
            <Tag intent="success" icon={IconNames.ARROW_UP}>
              Intended Speed
            </Tag>
          </div>
          <div style={{ textAlign: 'right' }}>
            <Tag intent="danger" rightIcon={IconNames.ARROW_UP}>
              Hardware Limit
            </Tag>
          </div>
        </div>
      </div>
    </Composition>
  )
}
