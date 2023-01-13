import { initialState } from '../../src/application/pages/toolpaths/interface/state'
import { Vector3 } from 'three'
import { Line, Movement } from '../../src/application/pages/toolpaths/optimiser/movements'
import { Material } from '../../src/application/pages/toolpaths/optimiser/materials/Base'

export function hexToRgba(hex: string): [r: number, g: number, b: number, a: number] {
  const result = /^#?([a-f\d]{2})([a-f\d]{2})([a-f\d]{2})$/i.exec(hex)

  if (!result) {
    throw new Error(`Couldn't parse hex colour: ${hex}`)
  }

  return [
    parseInt(result[1], 16) / 255, //
    parseInt(result[2], 16) / 255,
    parseInt(result[3], 16) / 255,
    1,
  ]
}

export function getDefaultSettings() {
  return initialState.settings
}

export function getDefaultVisualisationSettings() {
  return initialState.visualisationSettings
}

export function getDefaultCameraPosition() {
  return new Vector3(0, 0, 0)
}

/**
 * Generates a line as long as the duration, with a max speed of 1mm/s
 */
export function getNominalLine(material: Material, duration: number = 100) {
  const line = new Line(new Vector3(0, 0, 0), new Vector3(duration, 0, 0), material, 'objectId', [])

  line.setMaxSpeed(1)

  return line
}

export function generateLightPathFromMovement(movement: Movement, fromT = 0, toT = 1) {
  const settings = getDefaultSettings()
  const visualisationSettings = getDefaultVisualisationSettings()
  const cameraPosition = getDefaultCameraPosition()

  return movement.material.generateLightpath(movement, settings, visualisationSettings, cameraPosition, fromT, toT)
}
