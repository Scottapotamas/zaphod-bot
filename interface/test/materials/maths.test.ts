import { describe, expect, it, xit } from '@jest/globals'

import { rgbToHsi } from '../../src/application/pages/toolpaths/optimiser/materials/utilities'

describe(`Colour Maths`, () => {
  describe(`rgbToHsi`, () => {
    it(`converts white correctly`, () => {
      const [h, s, i] = rgbToHsi(1, 1, 1)

      expect(h).toBeCloseTo(0)
      expect(s).toBeCloseTo(0)
      expect(i).toBeCloseTo(1)
    })
    it(`converts red correctly`, () => {
      const [h, s, i] = rgbToHsi(1, 0, 0)

      expect(h).toBeCloseTo(0)
      expect(s).toBeCloseTo(1)
      expect(i).toBeCloseTo(1 / 3)
    })
    it(`converts green correctly`, () => {
      const [h, s, i] = rgbToHsi(0, 1, 0)

      expect(h).toBeCloseTo(120 / 360)
      expect(s).toBeCloseTo(1)
      expect(i).toBeCloseTo(1 / 3)
    })
    it(`converts blue correctly`, () => {
      const [h, s, i] = rgbToHsi(0, 0, 1)

      expect(h).toBeCloseTo(240 / 360)
      expect(s).toBeCloseTo(1)
      expect(i).toBeCloseTo(1 / 3)
    })
  })
})
