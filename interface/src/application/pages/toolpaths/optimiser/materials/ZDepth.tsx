import { MathUtils, Vector2, Vector3 } from 'three'
import { VisualisationSettings } from '../../interface/state'
import { Settings } from '../../optimiser/settings'
import { Movement, RGB, RGBA } from './../movements'
import { Material } from './Base'
import { NodeID } from '../../interface/RenderableTree'
import React, { useCallback, useState } from 'react'
import { lerpRGBA, MATERIALS } from './utilities'
import { ColorPicker } from '../../interface/ColorPicker'
import { NumericInput, Tag } from '@blueprintjs/core'
import { Composition, Box } from 'atomic-layout'

export interface ZDepthMaterialJSON {
  type: MATERIALS.Z_DEPTH
  color_from: RGBA
  color_to: RGBA
  depth_from: number
  depth_to: number
}

export const ZDepthMaterialDefaultJSON: ZDepthMaterialJSON = {
  type: MATERIALS.Z_DEPTH,
  color_from: [0, 0, 0, 1],
  color_to: [1, 1, 1, 1],
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
    [json.color_from[0], json.color_from[1], json.color_from[2], json.color_from[3]],
    [json.color_to[0], json.color_to[1], json.color_to[2], json.color_to[3]],
    json.depth_from,
    json.depth_to,
  )

  return mat
}

export class ZDepthMaterial extends Material {
  readonly type = MATERIALS.Z_DEPTH

  constructor(
    public start: RGBA,
    public end: RGBA,
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
  ): RGBA => {
    const depth = movement.samplePoint(t).distanceTo(cameraPosition)

    const clamped = MathUtils.clamp(
      MathUtils.mapLinear(depth, this.zFrom, this.zTo, 0, 1),
      0,
      1,
    )

    // console.log(`depth calc`, depth, this.zFrom, this.zTo, clamped)

    return lerpRGBA(this.start, this.end, clamped)
  }
}

export interface ZDepthMaterialEditorProps {
  json: ZDepthMaterialJSON
  mutateJson: (writer: (json: ZDepthMaterialJSON) => void) => void
}

export function ZDepthMaterialEditor(props: ZDepthMaterialEditorProps) {
  const [depthFrom, setDepthFrom] = useState(props.json.depth_from)
  const [depthTo, setDepthTo] = useState(props.json.depth_to)

  const handleUpdateDepthFrom = useCallback(
    depth => {
      setDepthFrom(depth)
    },
    [setDepthFrom],
  )

  const handleReleaseFrom: React.FocusEventHandler<HTMLInputElement> =
    useCallback(
      event => {
        setDepthFrom(Number(event.currentTarget.value))

        props.mutateJson(json => {
          json.depth_from = Number(event.currentTarget.value)
        })
      },
      [props.mutateJson],
    )

  const handleUpdateDepthTo = useCallback(
    depth => {
      setDepthTo(depth)
    },
    [setDepthTo],
  )

  const handleReleaseTo: React.FocusEventHandler<HTMLInputElement> =
    useCallback(
      event => {
        setDepthTo(Number(event.currentTarget.value))

        props.mutateJson(json => {
          json.depth_to = Number(event.currentTarget.value)
        })
      },
      [props.mutateJson],
    )

  return (
    <Composition templateCols="1fr 2fr" gap="1em" alignItems="center">
      From:
      <ColorPicker
        defaultColor={props.json.color_from}
        writer={col => {
          props.mutateJson(json => {
            json.color_from = col
          })
        }}
      />
      To:
      <ColorPicker
        defaultColor={props.json.color_to}
        writer={col => {
          props.mutateJson(json => {
            json.color_to = col
          })
        }}
      />
      At camera depth:
      <NumericInput
        fill
        min={0}
        max={2000}
        stepSize={25}
        majorStepSize={100}
        value={depthFrom}
        onValueChange={handleUpdateDepthFrom}
        onBlur={handleReleaseFrom}
        rightElement={<Tag>mm</Tag>}
        style={{ width: '100%' }}
      />
      To camera depth:
      <NumericInput
        fill
        min={0}
        max={2000}
        stepSize={25}
        majorStepSize={100}
        value={depthTo}
        onValueChange={handleUpdateDepthTo}
        onBlur={handleReleaseTo}
        rightElement={<Tag>mm</Tag>}
        style={{ width: '100%' }}
      />
    </Composition>
  )
}
