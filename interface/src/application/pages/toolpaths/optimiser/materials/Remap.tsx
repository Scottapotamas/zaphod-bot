import { MathUtils, Vector3 } from 'three'
import type { VisualisationSettings } from '../../interface/state'
import type { Settings } from '../settings'
import { AddComponentCallback, AddLineCallback, Movement } from '../movements'
import { Material } from './Base'

import { annotateDrawOrder, MATERIALS } from './utilities'
import { importMaterial, MaterialJSON } from '../material'

export interface RemapMaterialJSON {
  type: MATERIALS.REMAP
  parent: MaterialJSON
  remapFrom: number
  remapTo: number
}

export const RemapMaterialDefaultJSON: RemapMaterialJSON = {
  type: MATERIALS.REMAP,
  parent: {
    type: MATERIALS.COLOR,
    color: [1, 1, 1, 1],
  },
  remapFrom: 0,
  remapTo: 1,
}

export function isRemapMaterial(material: Material): material is RemapMaterial {
  return material.type === MATERIALS.REMAP
}

/**
 * RemapMaterials remap a singular material to a new time range. They do this on the material only,
 */
export class RemapMaterial extends Material {
  readonly type = MATERIALS.REMAP

  constructor(
    public parent: Material,
    public remapFrom: number = 0,
    private remapTo: number = 1,
  ) {
    super()
  }

  public generateLightpath = (
    movement: Movement,
    settings: Settings,
    visualisationSettings: VisualisationSettings,
    cameraPosition: Vector3,
    fromT: number,
    toT: number,
  ) => {
    return this.parent.generateLightpath(
      movement,
      settings,
      visualisationSettings,
      cameraPosition,
      MathUtils.clamp(
        MathUtils.mapLinear(fromT, 0, 1, this.remapFrom, this.remapTo),
        0,
        1,
      ),
      MathUtils.clamp(
        MathUtils.mapLinear(toT, 0, 1, this.remapFrom, this.remapTo),
        0,
        1,
      ),
    )
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
    // Render the parent material
    this.parent.generateThreeJSRepresentation(
      movementIndex,
      movement,
      settings,
      visualisationSettings,
      cameraPosition,
      addColouredLine,
      addDottedLine,
      addReactComponent,
      MathUtils.clamp(
        MathUtils.mapLinear(fromT, 0, 1, this.remapFrom, this.remapTo),
        0,
        1,
      ),
      MathUtils.clamp(
        MathUtils.mapLinear(toT, 0, 1, this.remapFrom, this.remapTo),
        0,
        1,
      ),
      spatialRenderFrom,
      spatialRenderTo,
    )
  }
}

//
