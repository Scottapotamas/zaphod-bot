import { MathUtils, Vector3 } from 'three'
import { VisualisationSettings } from '../../interface/state'
import { Settings } from '../settings'
import { PlannerLightMove } from '../hardware'
import { AddComponentCallback, AddLineCallback, Movement } from '../movements'
import { Material } from './Base'

import { annotateDrawOrder, MATERIALS } from './utilities'
import { importMaterial, MaterialJSON } from '../material'

export interface MixMaterialJSON {
  type: MATERIALS.MIX
  background: MaterialJSON
  foreground: MaterialJSON
  transitionT: number
}

export const MixMaterialDefaultJSON: MixMaterialJSON = {
  type: MATERIALS.MIX,
  background: {
    type: MATERIALS.COLOR,
    color: [1, 1, 1, 1],
  },
  foreground: {
    type: MATERIALS.COLOR,
    color: [1, 1, 1, 1],
  },
  transitionT: 0.5,
}

export function isMixMaterial(material: Material): material is MixMaterial {
  return material.type === MATERIALS.MIX
}

/**
 * MixMaterials transition between two materials at time factor transitionT
 */
export class MixMaterial extends Material {
  readonly type = MATERIALS.MIX

  constructor(
    public start: Material,
    public end: Material,
    private transitionT: number = 0.5,
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
    const fadesStart: PlannerLightMove[] =
      this.start.generateLightpath(
        movement,
        settings,
        visualisationSettings,
        cameraPosition,
        fromT,
        toT,
      )
    const fadesEnd: PlannerLightMove[] = this.end.generateLightpath(
      movement,
      settings,
      visualisationSettings,
      cameraPosition,
      fromT,
      toT,
    )

    return [
      ...fadesStart.map(fade => {
        fade.duration = Math.ceil(fade.duration * this.transitionT)

        return fade
      }),
      ...fadesEnd.map(fade => {
        fade.duration = Math.ceil(fade.duration * (1 - this.transitionT))

        return fade
      }),
    ]
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
  ) => {
    // Annotate draw order
    annotateDrawOrder(
      movementIndex,
      movement,
      visualisationSettings,
      addReactComponent,
    )

    // Render the start material to the half way point
    this.start.generateThreeJSRepresentation(
      movementIndex,
      movement,
      settings,
      Object.assign({}, visualisationSettings, { annotateDrawOrder: false }), // disable draw order annotation for the segments
      cameraPosition,
      addColouredLine,
      addDottedLine,
      addReactComponent,
      MathUtils.mapLinear(fromT, 0, 1, 0, this.transitionT),
      MathUtils.mapLinear(toT, 0, 1, 0, this.transitionT),
    )

    // Render the end material from the half way point
    this.end.generateThreeJSRepresentation(
      movementIndex,
      movement,
      settings,
      Object.assign({}, visualisationSettings, { annotateDrawOrder: false }), // disable draw order annotation for the segments
      cameraPosition,
      addColouredLine,
      addDottedLine,
      addReactComponent,
      MathUtils.mapLinear(fromT, 0, 1, this.transitionT, 1),
      MathUtils.mapLinear(toT, 0, 1, this.transitionT, 1),
    )
  }
}

// // This is a circular reference to ../material.tsx
// export function importMixMaterial(json: MixMaterialJSON): MixMaterial {
//   const background = importMaterial(json.background)
//   const foreground = importMaterial(json.foreground)

//   return new MixMaterial(background, foreground, json.transitionT)
// }
