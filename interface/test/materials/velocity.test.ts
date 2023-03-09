import { describe, expect, it, xit } from '@jest/globals'
import { Vector3 } from 'three'

import { ColorRampMaterial } from '../../src/application/pages/toolpaths/optimiser/materials/ColorRamp'
import { VelocityMaterial } from '../../src/application/pages/toolpaths/optimiser/materials/Velocity'
import {
  Line,
  Movement,
  Transition,
} from '../../src/application/pages/toolpaths/optimiser/movements'
import {
  declareDense,
  predictVelocityAtT,
} from '../../src/application/pages/toolpaths/optimiser/movement_utilities'
import {
  generateToolpathWithDefaults,
  renderMovementSequence,
} from '../optimiser/utilities'
import {
  generateLightPathFromMovement,
  getNominalLine,
  hexToRgba,
  prettyPrintLightPath,
} from './utilities'

function sampleSpeeds(movement: Movement) {
  const arr: string[] = []

  // for (const t of [0, 0.001, 0.3, 0.6, 0.999, 1]) {
  //   const speed = Math.round(predictVelocityAtT(movement, t) * 10) / 10
  //   arr.push(speed.toFixed(1).padStart(5, ' '))
  // }

  for (let index = 0; index <= 4; index++) {
    const t = index / 4
    const speed = Math.round(predictVelocityAtT(movement, t) * 10) / 10
    arr.push(speed.toFixed(1).padStart(5, ' '))
  }
  return arr
}

function assertVelocityContinuity(movements: Movement[]) {
  expect(movements.length).toBeGreaterThan(1)

  let previousMovement = movements[0]

  for (let index = 1; index < movements.length; index++) {
    const currentMovement = movements[index]

    const velocityAtPreviousMovementEnd = predictVelocityAtT(
      previousMovement,
      1,
    )

    const velocityAtCurrentMovementStart = predictVelocityAtT(
      currentMovement,
      0,
    )

    // Allow for 2% of the biggest velocity of the movements as leeyway
    const biggerVelocity = Math.max(
      predictVelocityAtT(previousMovement, 0),
      velocityAtPreviousMovementEnd,
      velocityAtCurrentMovementStart,
      predictVelocityAtT(currentMovement, 1),
    )

    const leeway = biggerVelocity / 50

    expect(
      Math.abs(velocityAtCurrentMovementStart - velocityAtPreviousMovementEnd),
    ).toBeLessThan(leeway)

    previousMovement = currentMovement
  }
}

function buildTest(
  startA: Vector3,
  endA: Vector3,
  speedA: number,
  startB: Vector3,
  endB: Vector3,
  speedB: number,
  transitionSize = 0.1,
) {
  const A = new Line(startA, endA, mat, 'A', [])
  A.setMaxSpeed(speedA)

  const B = new Line(startB, endB, mat, 'B', [])
  B.setMaxSpeed(speedB)

  const AtoB = new Transition(A, B, mat, transitionSize)

  AtoB.normaliseVelocities()

  assertVelocityContinuity([A, AtoB, B])
}

const mat = new VelocityMaterial()

describe(`VelocityMaterial`, () => {
  it(`playground`, () => {
    const A = new Line(
      new Vector3(0, 0, 0),
      new Vector3(100, 0, 0),
      mat,
      'objectId',
      [],
    )
    A.setMaxSpeed(100)

    const B = new Line(
      new Vector3(0, 100, 0),
      new Vector3(100, 100, 0),
      mat,
      'objectId',
      [],
    )
    B.setMaxSpeed(300)

    const C = new Transition(A, B, mat, 0.3333)

    C.normaliseVelocities()

    console.log(`Line 1: [${sampleSpeeds(A).join(', ')}]
Transt: [${sampleSpeeds(C).join(', ')}]
Line 2: [${sampleSpeeds(B).join(', ')}]`)
  })

  it(`straight lines, same velocity`, () => {
    buildTest(
      new Vector3(0, 0, 0),
      new Vector3(100, 0, 0),
      100,
      new Vector3(100, 0, 0),
      new Vector3(200, 0, 0),
      100,
    )
  })

  it(`straight lines, fast then slow`, () => {
    buildTest(
      new Vector3(0, 0, 0),
      new Vector3(100, 0, 0),
      300,
      new Vector3(100, 0, 0),
      new Vector3(200, 0, 0),
      100,
    )
  })

  it(`straight lines, slow then fast`, () => {
    buildTest(
      new Vector3(0, 0, 0),
      new Vector3(100, 0, 0),
      300,
      new Vector3(100, 0, 0),
      new Vector3(200, 0, 0),
      100,
    )
  })

  it(`90 degree lines, slow then fast`, () => {
    buildTest(
      new Vector3(0, 0, 0),
      new Vector3(100, 0, 0),
      300,
      new Vector3(100, 0, 0),
      new Vector3(100, 100, 0),
      100,
    )
  })

  it(`90 degree lines, fast then slow`, () => {
    buildTest(
      new Vector3(0, 0, 0),
      new Vector3(100, 0, 0),
      100,
      new Vector3(100, 0, 0),
      new Vector3(100, 100, 0),
      300,
    )
  })

  it(`parallel lines, same velocity`, () => {
    buildTest(
      new Vector3(0, 0, 0),
      new Vector3(100, 0, 0),
      100,
      new Vector3(0, 100, 0),
      new Vector3(100, 100, 0),
      100,
    )
  })

  it(`parallel lines, fast then slow`, () => {
    buildTest(
      new Vector3(0, 0, 0),
      new Vector3(100, 0, 0),
      300,
      new Vector3(0, 100, 0),
      new Vector3(100, 100, 0),
      100,
    )
  })

  it(`parallel lines, slow then fast`, () => {
    buildTest(
      new Vector3(0, 0, 0),
      new Vector3(100, 0, 0),
      300,
      new Vector3(0, 100, 0),
      new Vector3(100, 100, 0),
      100,
    )
  })
})
