import React, { useCallback } from 'react'

import { MATERIALS } from './utilities'
import { MaterialJSON } from '../material'

import { MathUtils, Vector3 } from 'three'
import type { Settings } from '../../optimiser/settings'
import { importMaterial } from '../material'
import { Movement, RGB, RGBA } from './../movements'
import { Material } from './Base'

import type { VisualisationSettings } from '../../interface/state'
import {
  MaterialSelector,
  MaterialEditor,
  MaterialOption,
  calculateInitialMaterialJSON,
  Spacer,
} from '../../interface/MaterialSelector'
import { Composition } from 'atomic-layout'

export enum BlendMode {
  NORMAL = 'normal',
  MULTIPLY = 'multiply',
}

function multiplySeperable(bC: number, fC: number) {
  return bC * fC
}

function blendSeperable<T extends RGB | RGBA>(
  background: T,
  foreground: T,
  memberCallback: (a: number, b: number) => number,
): T {
  const res: T = [] as unknown as T // We're sure it's going to be RGB or RGBA
  for (let i = 0; i < foreground.length; i++) {
    const bC = background[i]
    const fC = foreground[i]

    res[i] = MathUtils.clamp(memberCallback(bC, fC), 0, 1) // clamp colours
    // TODO: Nice per channel rolloff?
  }

  return res
}

export interface BlendMaterialJSON {
  type: MATERIALS.BLEND
  background: MaterialJSON
  foreground: MaterialJSON
  mode: BlendMode
}

export const BlendMaterialDefaultJSON: BlendMaterialJSON = {
  type: MATERIALS.BLEND,
  background: {
    type: MATERIALS.COLOR,
    color: [1, 1, 1, 1],
  },
  foreground: {
    type: MATERIALS.COLOR,
    color: [1, 1, 1, 1],
  },
  mode: BlendMode.MULTIPLY,
}

export function isBlendMaterial(material: Material): material is BlendMaterial {
  return material.type === MATERIALS.BLEND
}
/**
 * BlendMaterials blend two materials on top of one another with a blend mode. Only supports Materials that use the calculateColor function.
 */
export class BlendMaterial extends Material {
  readonly type = MATERIALS.BLEND

  constructor(
    public background: Material,
    public foreground: Material,
    private mode: BlendMode,
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
    const backgroundColor = this.background.calculateColor(
      movement,
      settings,
      visualisationSettings,
      cameraPosition,
      t,
    )
    const foregroundColor = this.foreground.calculateColor(
      movement,
      settings,
      visualisationSettings,
      cameraPosition,
      t,
    )

    switch (this.mode) {
      case BlendMode.NORMAL:
        // TODO: Support alpha channel on colour calculation
        return foregroundColor

      case BlendMode.MULTIPLY:
        return blendSeperable(
          backgroundColor,
          foregroundColor,
          multiplySeperable,
        )
      default:
        return [1, 1, 1, 1]
    }
  }
}

// This is a circular reference to ../material.tsx
export function importBlendMaterial(json: BlendMaterialJSON): BlendMaterial {
  const background = importMaterial(json.background)
  const foreground = importMaterial(json.foreground)

  return new BlendMaterial(background, foreground, json.mode)
}

export interface BlendMaterialEditorProps {
  json: BlendMaterialJSON
  mutateJson: (writer: (json: BlendMaterialJSON) => void) => void
}

interface MaterialSelectorProps {}

const materialEditorStyle = {
  display: 'grid',
  gridTemplateColumns: '1fr 1fr',
}

export function BlendMaterialEditor(props: BlendMaterialEditorProps) {
  const selectBackgroundMaterialType = useCallback(
    (option: MaterialOption) => {
      const override = calculateInitialMaterialJSON(option.materialType, null)

      props.mutateJson(blendJson => {
        if (!override) {
          console.error(
            `A material doesn't have a default`,
            option.materialType,
          )
          return
        }
        blendJson.background = override
      })
    },
    [props.mutateJson],
  )
  const selectForegroundMaterialType = useCallback(
    (option: MaterialOption) => {
      const override = calculateInitialMaterialJSON(option.materialType, null)

      props.mutateJson(blendJson => {
        if (!override) {
          console.error(
            `A material doesn't have a default`,
            option.materialType,
          )
          return
        }
        blendJson.foreground = override
      })
    },
    [props.mutateJson],
  )

  const backgroundUpdate = useCallback(
    (backgroundWriter: (json: MaterialJSON) => void) => {
      props.mutateJson(blendJson => {
        backgroundWriter(blendJson.background)
      })
    },
    [props.mutateJson],
  )
  const foregroundUpdate = useCallback(
    (foregroundWriter: (json: MaterialJSON) => void) => {
      props.mutateJson(blendJson => {
        foregroundWriter(blendJson.foreground)
      })
    },
    [props.mutateJson],
  )

  return (
    <>
      <Composition templateCols="1fr 2fr" gap="1em" alignItems="center">
        Background
        <MaterialSelector
          selectedType={props.json.background.type}
          onChange={selectBackgroundMaterialType}
          disallowDefault
        />
      </Composition>
      <MaterialEditor
        json={props.json.background}
        updateJson={backgroundUpdate}
      />

      <Spacer />

      <Composition templateCols="1fr 2fr" gap="1em" alignItems="center">
        Foreground
        <MaterialSelector
          selectedType={props.json.foreground.type}
          onChange={selectForegroundMaterialType}
          disallowDefault
        />
      </Composition>

      <MaterialEditor
        json={props.json.foreground}
        updateJson={foregroundUpdate}
      />
    </>
  )
}
