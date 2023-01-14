import { describe, expect, it, xit } from '@jest/globals'

import { ColorRampMaterial } from '../../src/application/pages/toolpaths/optimiser/materials/ColorRamp'
import {
  generateLightPathFromMovement,
  getNominalLine,
  hexToRgba,
  prettyPrintLightPath,
} from './utilities'

describe(`ColorRampMaterial`, () => {
  it(`has correct start and end points (black to black)`, () => {
    const mat = new ColorRampMaterial(
      hexToRgba(`#000000`),
      hexToRgba(`#ffffff`),
    )

    const line = getNominalLine(mat)

    const lightPath = generateLightPathFromMovement(line)

    // prettyPrintLightPath(lightPath)
  })
})
