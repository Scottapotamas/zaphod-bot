import { MathUtils, Vector3 } from 'three'
import type { VisualisationSettings } from '../../interface/state'
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

  // public calculateColor = (
  //   movement: Movement,

  //   settings: Settings,
  //   visualisationSettings: VisualisationSettings,

  //   t: number,
  // ): RGBA => {
  //   return [0, 0, 0]
  // }

  public generateLightpath = (
    movement: Movement,
    settings: Settings,
    visualisationSettings: VisualisationSettings,
    cameraPosition: Vector3,
    fromT: number,
    toT: number,
  ) => {
    // TODO: Have the Delta have a material that does this with its own information

    const fade: PlannerLightMove = {
      duration: movement.getDuration(),
      settings: { type: LightMoveType.IMMEDIATE },
      points: [rgbToHsi(1, 1, 1)],
    }

    return [fade]
  }

  public generateThreeJSRepresentation = (
    movementIndex: number,
    movement: Movement,
    settings: Settings,
    visualisationSettings: VisualisationSettings,
    cameraPosition: Vector3,
    addColouredLine: AddLineCallback,
    addDottedLine: AddLineCallback,
    addReactComponent: AddComponentCallback,
    fromT: number,
    toT: number,
    spatialRenderFrom: number,
    spatialRenderTo: number,
  ) => {
    if (visualisationSettings.annotateDrawOrder) {
      addReactComponent(
        generateHtmlTagFromAveragePosition(
          movementIndex,
          movement.getApproximateCentroid(),
          movementTypeToIntent(movement),
          `${movementTypeToLetter(
            movement,
          )} #${movementIndex} (${movement.getDuration()}ms, ${
            Math.round(
              (movement.getLength() / movement.getDuration()) * 1000 * 10,
            ) / 10
          }mm/s)`,
        ),
      )
    }

    // A simple color material draws the line segment(s) from the start to the end with a single color
    const numSegments =
      movement.type === MOVEMENT_TYPE.LINE ||
      movement.type === MOVEMENT_TYPE.POINT
        ? 1
        : Math.ceil(movement.getLength() / 5)

    // Calculate the bounds of the colors used
    const intendedSpeed = settings.optimisation.maxSpeed

    const movementDuration = movement.getDuration()

    const tRange = 1 / numSegments

    // For the number of segments,
    for (let index = 0; index < numSegments; index++) {
      const startT = MathUtils.mapLinear(
        index / numSegments,
        0,
        1,
        spatialRenderFrom,
        spatialRenderTo,
      )
      const endT = MathUtils.mapLinear(
        (index + 1) / numSegments,
        0,
        1,
        spatialRenderFrom,
        spatialRenderTo,
      )

      // Sample points along the movement, these will define the drawn line segment
      const start = movement.samplePoint(startT)
      const end = movement.samplePoint(endT)

      // Sample points before and after to get speeds at the start and end points
      // which define the line segment. Since we have vertex colour control,
      // we want that information embedded in every available vertex.
      const tBeforeStart = Math.max(0, startT - tRange / 2)
      const tMidpoint = (startT + endT) / 2
      const tAfterEnd = Math.min(1, endT + tRange / 2)

      // Calculate points before and after
      const pBeforeStart = movement.samplePoint(tBeforeStart)
      const pMidpoint = movement.samplePoint(tMidpoint)
      const pAfterEnd = movement.samplePoint(tAfterEnd)

      const timeStart = tMidpoint - tBeforeStart
      const timeEnd = tAfterEnd - tMidpoint

      const distanceStart = pBeforeStart.distanceTo(pMidpoint)
      const distanceEnd = pMidpoint.distanceTo(pAfterEnd)

      const startSpeed = distanceStart / timeStart
      const endSpeed = distanceEnd / timeEnd

      // Add the line
      addColouredLine(
        start,
        end,
        calculateColorFromSpeed(startSpeed, intendedSpeed),
        calculateColorFromSpeed(endSpeed, intendedSpeed),
        movementIndex, movement.objectID
      )
    }
  }
}

const LIMIT_SPEED = 350 // mm/s, the hardware limit, don't go over this under any circumstances

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
