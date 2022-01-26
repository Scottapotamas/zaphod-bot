import { MathUtils, Vector2, Vector3 } from 'three'
import { VisualisationSettings } from '../../interface/state'
import { Settings } from '../../optimiser/settings'
import { Movement, RGB } from './../movements'
import { Material } from './Base'
import { NodeID } from '../../interface/RenderableTree'
import React from 'react'
import { lerpRGB, MATERIALS } from './utilities'

export interface ZDepthMaterialJSON {
  type: MATERIALS.Z_DEPTH
  color_from: RGB
  color_to: RGB
  depth_from: number
  depth_to: number
}

export const ZDepthMaterialDefaultJSON: ZDepthMaterialJSON = {
  type: MATERIALS.Z_DEPTH,
  color_from: [0, 0, 0],
  color_to: [1, 1, 1],
  depth_from: 0,
  depth_to: 1,
}

export function isZDepthMaterial(
  material: Material,
): material is ZDepthMaterial {
  return material.type === MATERIALS.Z_DEPTH
}

export function importZDepthMaterial(json: ZDepthMaterialJSON) {
  const mat = new ZDepthMaterial(
    [json.color_from[0], json.color_from[1], json.color_from[2]],
    [json.color_to[0], json.color_to[1], json.color_to[2]],
    json.depth_from,
    json.depth_to,
  )

  return mat
}

export class ZDepthMaterial extends Material {
  readonly type = MATERIALS.Z_DEPTH

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
    const depth = movement.samplePoint(t).distanceTo(cameraPosition)

    const clamped = MathUtils.clamp(
      MathUtils.mapLinear(depth, this.zFrom, this.zTo, 0, 1),
      0,
      1,
    )

    // console.log(`depth calc`, depth, this.zFrom, this.zTo, clamped)

    return lerpRGB(this.start, this.end, clamped)
  }
}

export interface ZDepthMaterialEditorProps {
  objectID: NodeID
  json: ZDepthMaterialJSON
}

export function ZDepthMaterialEditor(props: ZDepthMaterialEditorProps) {
  return (
    <>
      ZDepthMaterialEditorProps for {props.objectID} with color from [
      {props.json.color_from.join(', ')}] to [{props.json.color_to.join(', ')}]
      over z{props.json.depth_from} to z{props.json.depth_to}
    </>
  )
}
