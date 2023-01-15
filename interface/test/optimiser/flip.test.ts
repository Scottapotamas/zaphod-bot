import { describe, expect, it, xit } from '@jest/globals'
import { Vector3 } from 'three'
import { SimpleColorMaterial } from '../../src/application/pages/toolpaths/optimiser/materials/Color'
import { ColorRampMaterial } from '../../src/application/pages/toolpaths/optimiser/materials/ColorRamp'
import {
  declareDense,
  deserialiseTour,
  Line,
  Point,
  serialiseTour,
} from '../../src/application/pages/toolpaths/optimiser/movements'
import {
  optimiseBySearch,
  OptimiserResult,
  sparseToDense,
} from '../../src/application/pages/toolpaths/optimiser/passes'
import { hexToRgba } from '../materials/utilities'

import { getDefaultSettings } from '../materials/utilities'

import { default as xxhash, XXHash } from 'xxhash-wasm'

import { BinaryLike, createHash } from 'crypto'

function createHasher(seed: number | undefined): XXHash<number> {
  const actual = createHash('md5')

  return {
    update: (data: BinaryLike) => {
      actual.update(data)
      return this
    },
    digest: () => {
      return actual.digest()[0]
    },
  }
}

import {
  generateToolpathWithDefaults,
  getXAxisAlignedLine,
  renderMovementSequence,
} from './utilities'

describe(`flip`, () => {
  it(`flipping a line doesn't flip the material`, async () => {
    const A = getXAxisAlignedLine(0, 100)
    const B = getXAxisAlignedLine(0, 100)

    const lines = [A, B]

    console.log(
      renderMovementSequence(
        generateToolpathWithDefaults(declareDense(lines)),
        { mmPerCell: 5 },
      ),
    )

    const serialisedPreOpt = serialiseTour(lines)

    const optimised = optimiseBySearch(lines, createHasher, {
      current: performance.now() + 1000,
    }).next()

    const res: OptimiserResult = optimised.value

    const tour = deserialiseTour(lines, res.best.tour)

    const serialisedPostOpt = serialiseTour(tour)

    console.log(serialisedPreOpt, serialisedPostOpt)

    console.log(
      renderMovementSequence(
        generateToolpathWithDefaults(declareDense(lines)),
        { mmPerCell: 5 },
      ),
    )
  })
})
