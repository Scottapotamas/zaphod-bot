import { describe, expect, it, xit } from '@jest/globals'

import { ColorRampMaterial } from '../../src/application/pages/toolpaths/optimiser/materials/ColorRamp'
import { RemapMaterial } from '../../src/application/pages/toolpaths/optimiser/materials/Remap'
import { Material } from '../../src/application/pages/toolpaths/optimiser/materials/Base'

import {
  generateLightPathFromMovement,
  getNominalLine,
  hexToRgba,
  prettyPrintLightPath,
  formatLightPathColourSequence,
  matsToSeq,
} from './utilities'

describe(`RemapMaterial`, () => {
  it(`support remapping`, () => {
    const base = new ColorRampMaterial(
      hexToRgba(`#ff0000`),
      hexToRgba(`#00ff00`),
    )

    const seq1 = matsToSeq([base])
    const seq2 = matsToSeq([
      new RemapMaterial(base, 0, 0.25),
      new RemapMaterial(base, 0.25, 0.5),
      new RemapMaterial(base, 0.5, 0.75),
      new RemapMaterial(base, 0.75, 1),
    ])

    console.log(`should be visually identical: \n[${seq1}] \n[${seq2}]`)

    // expect(seq1).toBe(seq2)
  })
})
