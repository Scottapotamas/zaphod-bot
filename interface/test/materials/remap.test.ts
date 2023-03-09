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
  Line,
  Point,
} from '../../src/application/pages/toolpaths/optimiser/movements'
import { declareDense } from '../../src/application/pages/toolpaths/optimiser/movement_utilities'

import { Vector3 } from 'three'
import { SimpleColorMaterial } from '../../src/application/pages/toolpaths/optimiser/materials/Color'
import {
  generateToolpathWithDefaults,
  getXAxisAlignedLine,
  renderMovementSequence,
} from '../optimiser/utilities'
import { sparseToDense } from '../../src/application/pages/toolpaths/optimiser/passes'

describe(`RemapMaterial`, () => {
  it(`support remapping`, () => {
    const base = new ColorRampMaterial(
      hexToRgba(`#000000`),
      hexToRgba(`#ffffff`),
    )

    const seq1 = matsToSeq([base])
    const seq2 = matsToSeq([
      new RemapMaterial(base, 0, 0.5),
      new RemapMaterial(base, 0.5, 1),
    ])

    console.log(`should be visually identical: \n[${seq1}] \n[${seq2}]`)

    expect(seq1).toBe(seq2)
  })

  it(`can flip a material`, () => {
    const base = new ColorRampMaterial(
      hexToRgba(`#ff00ee`),
      hexToRgba(`#eeff00`),
    )

    const lines = [
      new Line(new Vector3(0, 0, 0), new Vector3(100, 0, 0), base, `A`, []),
      new Line(
        new Vector3(100, 0, 0),
        new Vector3(0, 0, 0),
        new RemapMaterial(base, 1, 0),
        `B`,
        [],
      ),
    ]

    console.log(
      renderMovementSequence(
        generateToolpathWithDefaults(declareDense(lines)),
        { mmPerCell: 5 },
      ),
    )
  })

  it(`can shrink a material`, () => {
    const base = new ColorRampMaterial(
      hexToRgba(`#800020`),
      hexToRgba(`#FFC0CB`),
    )

    const lines = [
      new Line(new Vector3(0, 0, 0), new Vector3(100, 0, 0), base, `A`, []),
      new Line(
        new Vector3(0, 0, 0),
        new Vector3(50, 0, 0),
        new RemapMaterial(base, 0, 0.5),
        `B`,
        [],
      ),
      new Line(
        new Vector3(50, 0, 0),
        new Vector3(100, 0, 0),
        new RemapMaterial(base, 0.5, 1),
        `C`,
        [],
      ),
    ]

    console.log(
      renderMovementSequence(
        generateToolpathWithDefaults(declareDense(lines)),
        { mmPerCell: 5 },
      ),
    )
  })

  it(`handles an overshoot`, () => {
    const base = new ColorRampMaterial(
      hexToRgba(`#00ff00`),
      hexToRgba(`#ff00ff`),
    )

    const lines = [
      new Line(new Vector3(0, 0, 0), new Vector3(100, 0, 0), base, `A`, []),
      new Line(
        new Vector3(0, 0, 0),
        new Vector3(50, 0, 0),
        new RemapMaterial(base, -0.5, 0.3),
        `B`,
        [],
      ),
      new Line(
        new Vector3(50, 0, 0),
        new Vector3(100, 0, 0),
        new RemapMaterial(base, 0.7, 1.5),
        `C`,
        [],
      ),
    ]

    console.log(
      renderMovementSequence(
        generateToolpathWithDefaults(declareDense(lines)),
        { mmPerCell: 5 },
      ),
    )
  })
})
