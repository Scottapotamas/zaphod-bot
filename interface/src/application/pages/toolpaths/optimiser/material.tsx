import { MathUtils, Vector3 } from 'three'
import { VisualisationSettings } from '../interface/state'
import { Settings } from '../optimiser/settings'
import { LightMove, LightMoveType } from './hardware'
import {
  AddComponentCallback,
  AddLineCallback,
  isPointTransition,
  isTransition,
  Movement,
  MOVEMENT_TYPE,
  RGB,
} from './movements'
import { Html } from '@react-three/drei'
import { Intent, Tag } from '@blueprintjs/core'
import { NodeID, useTreeStore } from '../interface/RenderableTree'
import React from 'react'

export enum MATERIALS {
  COLOR = 'color',
  COLOR_RAMP = 'color-ramp',
  VELOCITY = 'velocity',
  INVISIBLE = 'invisible',
}

// Our angles are stored as floats in the range 0-1
const RadiansToUnitFloat = 1 / (2 * Math.PI)
const UnitFloatToRadians = 2 * Math.PI

// RGB, HSI all 0-1 floats
export function rgbToHsi(
  red: number,
  green: number,
  blue: number,
): [hue: number, saturation: number, intensity: number] {
  const intensity = (red + green + blue) / 3

  if (red == green && green == blue) {
    return [0, 0, intensity]
  } else {
    const alpha = (1 / 2) * (2 * red - green - blue)
    const beta = (Math.sqrt(3) / 2) * (green - blue)
    const hRad = Math.atan2(beta, alpha)
    const min = Math.min(red, green, blue)
    const saturation = 1 - min / intensity
    return [
      (hRad < 0 ? 2 * Math.PI + hRad : hRad) * RadiansToUnitFloat,
      saturation,
      intensity,
    ]
  }
}

// RGB, HSI all 0-1 floats
export function hsiToRgb(
  hue: number,
  saturation: number,
  intensity: number,
): [red: number, green: number, blue: number] {
  let r = 0
  let g = 0
  let b = 0
  let hRad = hue * UnitFloatToRadians

  if (hRad < (2 * Math.PI) / 3) {
    b = (1 - saturation) / 3
    r = (1 + (saturation * Math.cos(hRad)) / Math.cos(Math.PI / 3 - hRad)) / 3
    g = 1 - (b + r)
  } else if (hRad < (4 * Math.PI) / 3) {
    hRad -= (2 * Math.PI) / 3
    r = (1 - saturation) / 3
    g = (1 + (saturation * Math.cos(hRad)) / Math.cos(Math.PI / 3 - hRad)) / 3
    b = 1 - (r + g)
  } else {
    hRad -= (4 * Math.PI) / 3
    g = (1 - saturation) / 3
    b = (1 + (saturation * Math.cos(hRad)) / Math.cos(Math.PI / 3 - hRad)) / 3
    r = 1 - (g + b)
  }
  return [3 * intensity * r, 3 * intensity * g, 3 * intensity * b]
}

export type MaterialJSON =
  | ColorMaterialJSON
  | ColorRampMaterialJSON
  | VelocityMaterialJSON
  | InvisibleMaterialJSON

/**
 * Materials are currently stateless over the movements they're a part of
 */
export abstract class Material {
  abstract type: string

  /**
   * Given a movement ID, generate the light moves for this material.
   */
  abstract generateLightpath: (id: number, movement: Movement) => LightMove[]

  /**
   * Given a movement, sample some points and generate line segments, react components
   * for display in the 3D environment.
   */
  abstract generateThreeJSRepresentation: (
    movementIndex: number,
    movement: Movement,
    settings: Settings,
    visualisationSettings: VisualisationSettings,
    addColouredLine: AddLineCallback,
    addTransitionLine: AddLineCallback,
    addReactComponent: AddComponentCallback,
  ) => void
}

export interface ColorMaterialJSON {
  type: MATERIALS.COLOR
  color: [number, number, number, number]
}

export function isSimpleColorMaterial(
  material: Material,
): material is SimpleColorMaterial {
  return material.type === MATERIALS.COLOR
}

export function importColorMaterial(json: ColorMaterialJSON) {
  const mat = new SimpleColorMaterial([
    json.color[0],
    json.color[1],
    json.color[2],
  ])

  return mat
}

export class SimpleColorMaterial extends Material {
  readonly type = MATERIALS.COLOR

  constructor(public color: RGB) {
    super()
  }

  public generateLightpath = (id: number, movement: Movement) => {
    const fade: LightMove = {
      id,
      duration: movement.getDuration(),
      type: LightMoveType.IMMEDIATE,
      points: [rgbToHsi(this.color[0], this.color[1], this.color[2])],
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
    addTransitionLine: AddLineCallback,
    addReactComponent: AddComponentCallback,
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

    // For the number of segments,
    for (let index = 0; index < numSegments; index++) {
      const startT = index / numSegments
      const endT = (index + 1) / numSegments

      // Sample points along the movement
      const start = movement.samplePoint(startT)
      const end = movement.samplePoint(endT)

      // Add the line
      addColouredLine(start, end, this.color, this.color, movement.objectID)
    }
  }
}

function TagThatHidesWhenNotInList(props: {
  objectID: NodeID
  intent: Intent
  text: string
}) {
  const isHidden = useTreeStore(state => {
    // If nothing is hovered, display everything
    if (state.hoveredObjectIDs.length === 0) {
      return false
    }

    // If something is hovered, only display if it matches
    return !state.hoveredObjectIDs.includes(props.objectID)
  })

  return (
    <Tag
      intent={props.intent}
      minimal={isHidden}
      style={{ opacity: isHidden ? 0.5 : 1, cursor: 'pointer' }}
    >
      {props.text}
    </Tag>
  )
}

function generateHtmlTagFromAveragePosition(
  objectID: NodeID,
  centroid: Vector3,
  intent: Intent,
  text: string,
) {
  // Convert the centroid from Blender units to ThreeJS units
  const threeCentroid = new Vector3(centroid.x, centroid.z, -centroid.y)

  const component = (
    <Html position={threeCentroid} key={text}>
      <TagThatHidesWhenNotInList
        objectID={objectID}
        intent={intent}
        text={text}
      />
    </Html>
  )

  return component
}

function annotateDrawOrder(
  movementIndex: number,
  movement: Movement,
  visualisationSettings: VisualisationSettings,
  addReactComponent: AddComponentCallback,
) {
  if (visualisationSettings.annotateDrawOrder) {
    let intent: Intent = 'none'

    if (
      movement.type === 'transition' ||
      movement.type === 'point-transition'
    ) {
      intent = 'primary'
    }

    let type = 'U'

    switch (movement.type) {
      case MOVEMENT_TYPE.LINE:
        type = 'L'
        break
      case MOVEMENT_TYPE.POINT:
        type = 'P'
        break
      case MOVEMENT_TYPE.TRANSITION:
        type = 'T'
        break
      case MOVEMENT_TYPE.POINT_TRANSITION:
        type = 'PT'
        break

      default:
        break
    }

    addReactComponent(
      generateHtmlTagFromAveragePosition(
        movement.objectID,
        movement.getApproximateCentroid(),
        intent,
        `${type} #${movementIndex}`,
      ),
    )
  }
}

export interface ColorRampMaterialJSON {
  type: MATERIALS.COLOR_RAMP
  colorFrom: [number, number, number, number]
  colorTo: [number, number, number, number]
}

export function isColorRampMaterial(
  material: Material,
): material is SimpleColorMaterial {
  return material.type === MATERIALS.COLOR_RAMP
}

export function importColorRampMaterial(json: ColorRampMaterialJSON) {
  const mat = new ColorRampMaterial(
    [json.colorFrom[0], json.colorFrom[1], json.colorFrom[2]],
    [json.colorTo[0], json.colorTo[1], json.colorTo[2]],
  )

  return mat
}

/**
 * Linearly lerp between to colours, converting to HSI space first.
 *
 * TODO: Does this actually match the deltas implementation?
 */
function lerpRGB(
  a: [r: number, g: number, b: number],
  b: [r: number, g: number, b: number],
  t: number,
) {
  const tClamped = MathUtils.clamp(t, 0, 1)
  const [hA, sA, iA] = rgbToHsi(a[0], a[1], a[2])
  const [hB, sB, iB] = rgbToHsi(b[0], b[1], b[2])

  const h = MathUtils.lerp(hA, hB, tClamped)
  const s = MathUtils.lerp(sA, sB, tClamped)
  const i = MathUtils.lerp(iA, iB, tClamped)

  return hsiToRgb(h, s, i)
}

export class ColorRampMaterial extends Material {
  readonly type = MATERIALS.COLOR_RAMP

  constructor(public start: RGB, public end: RGB) {
    super()
  }

  public generateLightpath = (id: number, movement: Movement) => {
    const fade: LightMove = {
      id,
      duration: movement.getDuration(),
      type: LightMoveType.RAMP,
      points: [
        rgbToHsi(this.start[0], this.start[1], this.start[2]),
        rgbToHsi(this.end[0], this.end[1], this.end[2]),
      ],
      num_points: 2,
    }

    return [fade]
  }

  public generateThreeJSRepresentation = (
    movementIndex: number,
    movement: Movement,
    settings: Settings,
    visualisationSettings: VisualisationSettings,
    addColouredLine: AddLineCallback,
    addTransitionLine: AddLineCallback,
    addReactComponent: AddComponentCallback,
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

    // For the number of segments,
    for (let index = 0; index < numSegments; index++) {
      const startT = index / numSegments
      const endT = (index + 1) / numSegments

      // Sample points along the movement
      const start = movement.samplePoint(startT)
      const end = movement.samplePoint(endT)

      // Sample along the gradient between the two colours
      const startCol = lerpRGB(this.start, this.end, startT)
      const endCol = lerpRGB(this.start, this.end, endT)

      // Add the line
      addColouredLine(start, end, startCol, endCol, movement.objectID)
    }
  }
}

export interface VelocityMaterialJSON {
  type: MATERIALS.VELOCITY
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

  public generateLightpath = (id: number, movement: Movement) => {
    // TODO: Have the Delta have a material that does this with its own information

    const fade: LightMove = {
      id,
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
    addTransitionLine: AddLineCallback,
    addReactComponent: AddComponentCallback,
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
      const startT = index / numSegments
      const endT = (index + 1) / numSegments

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

export interface InvisibleMaterialJSON {
  type: MATERIALS.INVISIBLE
  // For the color in the UI
  color: [number, number, number]
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
  ])

  return mat
}

export class InvisibleMaterial extends Material {
  readonly type = MATERIALS.INVISIBLE

  constructor(public color: RGB) {
    super()
  }

  public generateLightpath = (id: number, movement: Movement) => {
    const fade: LightMove = {
      id,
      duration: movement.getDuration(),
      type: LightMoveType.IMMEDIATE,
      points: [[0, 0, 0]], // Set a light move of the LED being off
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
    addTransitionLine: AddLineCallback,
    addReactComponent: AddComponentCallback,
  ) => {
    // Annotate draw order
    annotateDrawOrder(
      movementIndex,
      movement,
      visualisationSettings,
      addReactComponent,
    )

    // Despite being invisible in hardware, we still want to draw this in the UI
    const numSegments =
      movement.type === MOVEMENT_TYPE.LINE ||
      movement.type === MOVEMENT_TYPE.POINT
        ? 1
        : visualisationSettings.curveSegments

    // For the number of segments,
    for (let index = 0; index < numSegments; index++) {
      const startT = index / numSegments
      const endT = (index + 1) / numSegments

      // Sample points along the movement
      const start = movement.samplePoint(startT)
      const end = movement.samplePoint(endT)

      // Add the line
      addTransitionLine(start, end, this.color, this.color, movement.objectID)
    }
  }
}

export const defaultTransitionMaterial = new InvisibleMaterial([
  19 / 255,
  124 / 255,
  189 / 255,
])

export function importMaterial(json: MaterialJSON) {
  switch (json.type) {
    case MATERIALS.COLOR:
      return importColorMaterial(json)
    case MATERIALS.COLOR_RAMP:
      return importColorRampMaterial(json)
    case MATERIALS.VELOCITY:
      return importVelocityMaterial(json)
    case MATERIALS.INVISIBLE:
      return importInvisibleMaterial(json)

    default:
      throw new Error(`Error importing material, unknown type ${json['type']}`)
  }
}
