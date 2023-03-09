import { describe, expect, it, xit } from '@jest/globals'
import { MathUtils, Vector3 } from 'three'
import { SimpleColorMaterial } from '../../src/application/pages/toolpaths/optimiser/materials/Color'
import { ColorRampMaterial } from '../../src/application/pages/toolpaths/optimiser/materials/ColorRamp'
import { Point } from '../../src/application/pages/toolpaths/optimiser/movements'
import { hexToRgba } from '../materials/utilities'
import { declareDense } from '../../src/application/pages/toolpaths/optimiser/movement_utilities'

import {
  findLightFade,
  generateToolpathWithDefaults,
  getXAxisAlignedLine,
  renderMovementSequence,
} from './utilities'

describe(`binarySearch`, () => {
  it(`correctly finds the light path`, () => {
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

    const toolpath = generateToolpathWithDefaults(declareDense(lines))

    console.log(`0`, findLightFade(toolpath, 0).timestamp)
    console.log(`50`, findLightFade(toolpath, 50).timestamp)
    console.log(`100`, findLightFade(toolpath, 100).timestamp)
    console.log(`150`, findLightFade(toolpath, 150).timestamp)
    console.log(`200`, findLightFade(toolpath, 200).timestamp)
    console.log(`300`, findLightFade(toolpath, 300).timestamp)
  })
})

describe(`mapLinear`, () => {
  xit(`clamps values`, () => {
    expect(MathUtils.mapLinear(-1, 0, 1, 0, 100)).toBe(0)
  })
})
