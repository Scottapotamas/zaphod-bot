import { MathUtils, Vector3 } from 'three'
import { VisualisationSettings } from '../../interface/state'
import { Settings } from '../../optimiser/settings'
import { Movement, RGB } from './../movements'
import { Material } from './Base'
import { NodeID } from '../../interface/RenderableTree'
import React from 'react'
import { lerpRGB, MATERIALS } from './utilities'

export interface ZGradientMaterialJSON {
  type: MATERIALS.Z_GRADIENT
  color_from: RGB
  color_to: RGB
  z_from: number
  z_to: number
}

export const ZGradientMaterialDefaultJSON: ZGradientMaterialJSON = {
  type: MATERIALS.Z_GRADIENT,
  color_from: [0, 0, 0],
  color_to: [1, 1, 1],
  z_from: 0,
  z_to: 1,
}

export function isZGradientMaterial(
  material: Material,
): material is ZGradientMaterial {
  return material.type === MATERIALS.Z_GRADIENT
}

export function importZGradientMaterial(json: ZGradientMaterialJSON) {
  const mat = new ZGradientMaterial(
    [json.color_from[0], json.color_from[1], json.color_from[2]],
    [json.color_to[0], json.color_to[1], json.color_to[2]],
    json.z_from,
    json.z_to,
  )

  return mat
}

export class ZGradientMaterial extends Material {
  readonly type = MATERIALS.Z_GRADIENT

  constructor(
    public start: RGB,
    public end: RGB,
    public zFrom: number,
    public zTo: number,
  ) {
    super()
  }

  public calculateColor = (
    movement: Movement,
    settings: Settings,
    visualisationSettings: VisualisationSettings,
    cameraPosition: Vector3,
    t: number,
  ): RGB => {
    const z = movement.samplePoint(t).z

    const clamped = MathUtils.clamp(
      MathUtils.mapLinear(z, this.zFrom, this.zTo, 0, 1),
      0,
      1,
    )

    return lerpRGB(this.start, this.end, clamped)
  }
}

export interface ZGradientMaterialEditorProps {
  objectID: NodeID
  json: ZGradientMaterialJSON
}

export function ZGradientMaterialEditor(props: ZGradientMaterialEditorProps) {
  return (
    <>
      ZGradientMaterialEditorProps for {props.objectID} with color from [
      {props.json.color_from.join(', ')}] to [{props.json.color_to.join(', ')}]
      over z{props.json.z_from} to z{props.json.z_to}
    </>
  )
}
