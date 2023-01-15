import { describe, expect, it, xit } from '@jest/globals'
import { Vector3 } from 'three'
import { SimpleColorMaterial } from '../../src/application/pages/toolpaths/optimiser/materials/Color'
import { ColorRampMaterial } from '../../src/application/pages/toolpaths/optimiser/materials/ColorRamp'
import {
  declareDense,
  Line,
  Point,
} from '../../src/application/pages/toolpaths/optimiser/movements'
import { sparseToDense } from '../../src/application/pages/toolpaths/optimiser/passes'
import { hexToRgba } from '../materials/utilities'

import { getDefaultSettings } from '../materials/utilities'

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
        new SimpleColorMaterial(hexToRgba(`#c0e1ff`)),
        `starting_flash`,
        [],
      ), // flash at the start
      getXAxisAlignedLine(0, 100),
      getXAxisAlignedLine(100, 120), // right way, should look different?
      new Point(
        new Vector3(120, 0, 0),
        100,
        new SimpleColorMaterial(hexToRgba(`#c0e1ff`)),
        `starting_flash`,
        [],
      ), // flash at the start
      getXAxisAlignedLine(120, 50), // right way, should look different?
      getXAxisAlignedLine(50, 120), // right way, should look different?
      getXAxisAlignedLine(120, 150),
    ]

    console.log(
      renderMovementSequence(
        generateToolpathWithDefaults(declareDense(lines)),
        { mmPerCell: 5 },
      ),
    )
  })
  it(`adds transition moves`, () => {
    const A = new Line(
      new Vector3(0, 0, 0),
      new Vector3(100, 0, 100),
      new ColorRampMaterial(hexToRgba(`#800020`), hexToRgba(`#FFC0CB`)),
      `A`,
      [],
    )
    const B = new Line(
      new Vector3(100, 0, 100),
      new Vector3(200, 0, 0),
      new ColorRampMaterial(hexToRgba(`#800020`), hexToRgba(`#FFC0CB`)),
      `A`,
      [],
    )

    const lines = [A, B]

    const settings = getDefaultSettings()

    settings.optimisation.waitAtStartDuration = 100
    settings.optimisation.maxSpeed = 1000

    const dense1 = sparseToDense(lines, settings)

    console.log(
      renderMovementSequence(generateToolpathWithDefaults(dense1), {
        mmPerCell: 5,
      }),
    )

    settings.optimisation.smoothInterlineTransitions = true
    settings.optimisation.interLineTransitionAngle = 360
    settings.optimisation.interLineTransitionShaveDistance = 10

    const dense2 = sparseToDense(lines, settings)

    console.log(
      'with inter line \n' +
        renderMovementSequence(generateToolpathWithDefaults(dense2), {
          mmPerCell: 5,
        }),
    )
  })
})
