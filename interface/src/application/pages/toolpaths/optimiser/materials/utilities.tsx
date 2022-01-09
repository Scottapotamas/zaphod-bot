import { MathUtils, Vector3 } from 'three'
import { NodeID } from '../../interface/RenderableTree'
import { useStore, VisualisationSettings } from '../../interface/state'
import { AddComponentCallback, Movement, MOVEMENT_TYPE } from '../movements'
import { Intent, Tag } from '@blueprintjs/core'
import { Html } from '@react-three/drei'
import React from 'react'

export enum MATERIALS {
  DEFAULT = 'default', // used for overrides to indicate no override
  COLOR = 'color',
  COLOR_RAMP = 'color-ramp',
  VELOCITY = 'velocity',
  MIX = 'mix',
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

/**
 * Linearly lerp between to colours, converting to HSI space first.
 *
 * TODO: Does this actually match the deltas implementation?
 */
export function lerpRGB(
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

export function annotateDrawOrder(
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

function TagThatHidesWhenNotInList(props: {
  objectID: NodeID
  intent: Intent
  text: string
}) {
  const isHidden = useStore(state => {
    // If nothing is hovered, display everything
    if (state.treeStore.hoveredObjectIDs.length === 0) {
      return false
    }

    // If something is hovered, only display if it matches
    return !state.treeStore.hoveredObjectIDs.includes(props.objectID)
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
