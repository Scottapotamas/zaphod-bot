import type { VisualisationSettings } from '../../interface/state'
import type { Settings } from '../../optimiser/settings'
import { PlannerLightMove } from './../hardware'
import { AddComponentCallback, AddLineCallback, Movement } from './../movements'
import { Material } from './Base'
import { MATERIALS } from './utilities'
import { invisibleLightFade } from './Invisible'
import { Vector3 } from 'three'

export function isDelayMaterial(material: Material): material is DelayMaterial {
  return material.type === MATERIALS.DELAY
}

/**
 * DelayMaterials wait a certain amount of time before they turn on,
 * display another material, then turn off for a certain amount of time
 */
export class DelayMaterial extends Material {
  readonly type = MATERIALS.DELAY

  constructor(
    public material: Material,
    private preWait: number,
    private onDuration: number,
    private postWait: number,
  ) {
    super()
  }

  // TODO: I think this can be generalised to use the fromT and toT methods
  public generateLightpath = (
    movement: Movement,
    settings: Settings,
    visualisationSettings: VisualisationSettings,
    cameraPosition: Vector3,
    fromT: number,
    toT: number,
  ) => {
    const totalDuration = movement.getDuration()
    const onDuration = this.onDuration
    const factor = onDuration / totalDuration

    const fades: PlannerLightMove[] = this.material.generateLightpath(
      movement,
      settings,
      visualisationSettings,
      cameraPosition,
      fromT,
      toT,
    )

    fades.forEach(fade => {
      if (fades.length <= 1) {
        // Reduce the material's duration by the factor
        // TODO: Are we sometimes out by a millisecond?
        fade.duration = Math.ceil(fade.duration * factor)
      } else {
        // Set the duration if there's only one move, this fulfills the current use case of Points
        // with delays before and after
        fade.duration = this.onDuration
      }

      return fade
    })

    if (this.preWait > 0) {
      fades.unshift(invisibleLightFade(this.preWait))
    }

    if (this.postWait > 0) {
      fades.push(invisibleLightFade(this.postWait))
    }

    return fades
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
    // Defer the material rendering
    this.material.generateThreeJSRepresentation(
      movementIndex,
      movement,
      settings,
      visualisationSettings,
      cameraPosition,
      addColouredLine,
      addDottedLine,
      addReactComponent,
      fromT,
      toT,
      spatialRenderFrom,
      spatialRenderTo,
    )
  }
}
