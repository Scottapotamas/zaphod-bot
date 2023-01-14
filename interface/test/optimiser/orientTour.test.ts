import { describe, expect, it, xit } from '@jest/globals'
import { Vector3 } from 'three'
import { SimpleColorMaterial } from '../../src/application/pages/toolpaths/optimiser/materials/Color'
import { ColorRampMaterial } from '../../src/application/pages/toolpaths/optimiser/materials/ColorRamp'
import {
  declareDense,
  Point,
} from '../../src/application/pages/toolpaths/optimiser/movements'
import { hexToRgba } from '../materials/utilities'

import {
  generateToolpathWithDefaults,
  getXAxisAlignedLine,
  renderMovementSequence,
} from './utilities'

describe(`orientTour`, () => {
  it(`correctly aligns a series of lines`, () => {
    const lines = [
      // start
      new Point(
        new Vector3(0, 0, 0),
        100,
        new SimpleColorMaterial(hexToRgba(`#0000ff`)),
        `starting_flash`,
        [],
      ), // flash at the start
      getXAxisAlignedLine(0, 100),
      getXAxisAlignedLine(120, 100), // wrong way, should flip
      getXAxisAlignedLine(120, 150),
    ]

    console.log(
      renderMovementSequence(
        generateToolpathWithDefaults(declareDense(lines)),
        { mmPerCell: 5 },
      ),
    )
  })
})
