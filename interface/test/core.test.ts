import { describe, expect, it, xit } from '@jest/globals'
import { CancellationToken } from '@electricui/core'

describe(`CancellationToken`, () => {
  it(`Importing an eUI internal works`, () => {
    const cancellationToken = new CancellationToken()

    expect(typeof cancellationToken.cancel).toBe('function')
  })
})

