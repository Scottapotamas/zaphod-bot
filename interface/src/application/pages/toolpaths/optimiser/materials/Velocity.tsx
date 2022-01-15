import { MathUtils, Vector3 } from 'three'
import { VisualisationSettings } from '../../interface/state'
import { Settings } from '../../optimiser/settings'
import { PlannerLightMove, LightMoveType } from './../hardware'
import {
  AddComponentCallback,
  AddLineCallback,
  isPointTransition,
  isTransition,
  Movement,
  MOVEMENT_TYPE,
  RGB,
} from './../movements'
import { Material } from './Base'
import { Html } from '@react-three/drei'
import { Intent, Tag } from '@blueprintjs/core'
import { NodeID } from '../../interface/RenderableTree'
import React from 'react'
import { annotateDrawOrder, lerpRGB, MATERIALS, rgbToHsi } from './utilities'
import { IconNames } from '@blueprintjs/icons'

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

  public generateLightpath = (movement: Movement) => {
    // TODO: Have the Delta have a material that does this with its own information

    const fade: PlannerLightMove = {
      duration: movement.getDuration(),
      type: LightMoveType.IMMEDIATE,
      points: [rgbToHsi(1, 1, 1)],
      num_points: 1,
    }

    return [fade]
  }

  public generateThreeJSRepresentation = (
    movementIndex: number,
    movement: Movement,
    settings: Settings,
    visualisationSettings: VisualisationSettings,
    addColouredLine: AddLineCallback,
    addDottedLine: AddLineCallback,
    addReactComponent: AddComponentCallback,
    fromT: number,
    toT: number,
  ) => {
    // Annotate draw order
    annotateDrawOrder(
      movementIndex,
      movement,
      visualisationSettings,
      addReactComponent,
    )

    // A simple color material draws the line segment(s) from the start to the end with a single color
    const numSegments =
      movement.type === MOVEMENT_TYPE.LINE ||
      movement.type === MOVEMENT_TYPE.POINT
        ? 1
        : visualisationSettings.curveSegments

    // Calculate the bounds of the colors used
    const intendedSpeed =
      isTransition(movement) || isPointTransition(movement)
        ? settings.optimisation.transitionMaxSpeed
        : settings.optimisation.maxSpeed

    const movementDuration = movement.getDuration()

    const tRange = 1 / numSegments

    // For the number of segments,
    for (let index = 0; index < numSegments; index++) {
      const startT = MathUtils.mapLinear(index / numSegments, 0, 1, fromT, toT)
      const endT = MathUtils.mapLinear(
        (index + 1) / numSegments,
        0,
        1,
        fromT,
        toT,
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
        movement.objectID,
      )
    }
  }
}

const LIMIT_SPEED = 350 // mm/s, the hardware limit, don't go over this under any circumstances

const BLACK: RGB = [0, 0, 0]
const GREEN: RGB = [15 / 255, 153 / 255, 96 / 255] // #0F9960
const ORANGE: RGB = [217 / 255, 130 / 255, 43 / 255] // #D9822B
const RED: RGB = [219 / 255, 55 / 255, 55 / 255] // #DB3737

function calculateColorFromSpeed(speed: number, intendedSpeed: number) {
  if (speed < intendedSpeed) {
    // Speeds from 0 to the intended speed are varying degrees of green
    return lerpRGB(
      BLACK,
      GREEN,
      MathUtils.mapLinear(speed, 0, intendedSpeed, 0, 1),
    )
  } else if (speed < LIMIT_SPEED) {
    // Speeds above the intended speed but below the hardware limit become more orange
    return lerpRGB(
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
  objectID: NodeID
  json: VelocityMaterialJSON
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
  )
}
