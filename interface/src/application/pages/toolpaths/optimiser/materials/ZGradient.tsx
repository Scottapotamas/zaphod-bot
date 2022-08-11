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

export interface ZGradientMaterialJSON {
  type: MATERIALS.Z_GRADIENT
  color_from: RGBA
  color_to: RGBA
  z_from: number
  z_to: number
}

export const ZGradientMaterialDefaultJSON: ZGradientMaterialJSON = {
  type: MATERIALS.Z_GRADIENT,
  color_from: [0, 0, 0, 1],
  color_to: [1, 1, 1, 1],
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
    [json.color_from[0], json.color_from[1], json.color_from[2], json.color_from[3]],
    [json.color_to[0], json.color_to[1], json.color_to[2], json.color_to[3]],
    json.z_from,
    json.z_to,
  )

  return mat
}

export class ZGradientMaterial extends Material {
  readonly type = MATERIALS.Z_GRADIENT

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
    const z = movement.samplePoint(t).z

    const clamped = MathUtils.clamp(
      MathUtils.mapLinear(z, this.zFrom, this.zTo, 0, 1),
      0,
      1,
    )

    return lerpRGBA(this.start, this.end, clamped)
  }
}

export interface ZGradientMaterialEditorProps {
  json: ZGradientMaterialJSON
  mutateJson: (writer: (json: ZGradientMaterialJSON) => void) => void
}

export function ZGradientMaterialEditor(props: ZGradientMaterialEditorProps) {
  const [zFrom, setZFrom] = useState(props.json.z_from)
  const [zTo, setZTo] = useState(props.json.z_to)

  const handleUpdateZFrom = useCallback(
    z => {
      setZFrom(z)
    },
    [setZFrom],
  )

  const handleReleaseFrom: React.FocusEventHandler<HTMLInputElement> =
    useCallback(
      event => {
        setZFrom(Number(event.currentTarget.value))

        props.mutateJson(json => {
          json.z_from = Number(event.currentTarget.value)
        })
      },
      [props.mutateJson],
    )

  const handleUpdateZTo = useCallback(
    z => {
      setZTo(z)
    },
    [setZTo],
  )

  const handleReleaseTo: React.FocusEventHandler<HTMLInputElement> =
    useCallback(
      event => {
        setZTo(Number(event.currentTarget.value))

        props.mutateJson(json => {
          json.z_to = Number(event.currentTarget.value)
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
      At Z:
      <NumericInput
        fill
        min={0}
        max={2000}
        stepSize={25}
        majorStepSize={100}
        value={zFrom}
        onValueChange={handleUpdateZFrom}
        onBlur={handleReleaseFrom}
        rightElement={<Tag>mm</Tag>}
        style={{ width: '100%' }}
      />
      To Z:
      <NumericInput
        fill
        min={0}
        max={2000}
        stepSize={25}
        majorStepSize={100}
        value={zTo}
        onValueChange={handleUpdateZTo}
        onBlur={handleReleaseTo}
        rightElement={<Tag>mm</Tag>}
        style={{ width: '100%' }}
      />
    </Composition>
  )
}
