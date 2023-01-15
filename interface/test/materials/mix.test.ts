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
  getDefaultSettings,
} from './utilities'
import {
  declareDense,
  Line,
  Point,
} from '../../src/application/pages/toolpaths/optimiser/movements'
import { Vector3 } from 'three'
import { SimpleColorMaterial } from '../../src/application/pages/toolpaths/optimiser/materials/Color'
import {
  generateToolpathWithDefaults,
  getXAxisAlignedLine,
  renderMovementSequence,
} from '../optimiser/utilities'
import { sparseToDense } from '../../src/application/pages/toolpaths/optimiser/passes'
import { MixMaterial } from '../../src/application/pages/toolpaths/optimiser/materials/Mix'

describe(`MixMaterial`, () => {
  it(`can shrink a material`, () => {
    const base = new ColorRampMaterial(
      hexToRgba(`#800020`),
      hexToRgba(`#FFC0CB`),
    )

    const mix = new MixMaterial(
      new RemapMaterial(base, 0, 0.5),
      new RemapMaterial(base, 0.5, 1),
      0.5,
    )

    const lines = [
      new Line(new Vector3(0, 0, 0), new Vector3(100, 0, 0), base, `A`, []),
      new Line(new Vector3(0, 0, 0), new Vector3(100, 0, 0), mix, `B`, []),
      new Line(new Vector3(0, 0, 0), new Vector3(50, 0, 0), base, `C`, []),
      new Line(new Vector3(50, 0, 0), new Vector3(100, 0, 0), base, `D`, []),
    ]

    console.log(
      renderMovementSequence(
        generateToolpathWithDefaults(declareDense(lines)),
        { mmPerCell: 5 },
      ),
    )
  })
})
