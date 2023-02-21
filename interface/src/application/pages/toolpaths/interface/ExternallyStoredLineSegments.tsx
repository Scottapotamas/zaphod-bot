import * as React from 'react'

import { Color, Vector2 } from 'three'

import { RGBA } from '../optimiser/movements'
import { LineMaterial, LineSegmentsGeometry, Line2 } from 'three-stdlib'
import { lerpRGBA } from '../optimiser/materials/utilities'

export class MutableLineSegmentGeometry {
  public line
  public material: LineMaterial
  public geometry = new LineSegmentsGeometry()
  public resolution = new Vector2(512, 512)

  public positions: number[] = []
  public colors: number[] = []

  constructor(dashed: boolean, public lineWidth: number) {
    this.line = new Line2(this.geometry as any)
    this.material = new LineMaterial({
      dashed,
    })

    this.refreshGeometry = this.refreshGeometry.bind(this)
    this.dispose = this.dispose.bind(this)
    this.getReactComponent = this.getReactComponent.bind(this)
    this.setHoveredIndices = this.setHoveredIndices.bind(this)
  }

  refreshGeometry(numLines: number) {
    // This re-allocation sucks, but Three doesn't clean up after itself properly if we reuse it,
    // One of the computed length buffers is the wrong length I think?
    this.geometry = new LineSegmentsGeometry()
    this.geometry.setColors(this.colors)
    this.geometry.setPositions(this.positions)
    this.geometry.setDrawRange(0, numLines * 6)

    this.line = new Line2(this.geometry as any, this.material)
    this.line.computeLineDistances()
  }

  /**
   * Set the list of line indices that are hovered over.
   *
   * If none are hovered over, all lines are displayed at full colour
   */
  setHoveredIndices(lineIndices: number[], hideAll: boolean, backgroundCol: string) {
    if (lineIndices.length === 0 && !hideAll) {
      // Wipe it
      this.geometry.setColors(this.colors)
      return
    }
    const colors = this.colors.slice()

    const backgroundColor = new Color(backgroundCol)
    const backgroundColorAsRGBA: RGBA = [
      backgroundColor.r,
      backgroundColor.g,
      backgroundColor.b,
      1,
    ]

    const darkeningFactor = 0.9

    for (let lineIndex = 0; lineIndex < colors.length / 6; lineIndex++) {
      // If it's in the list, or we're doing them all
      if (!lineIndices.includes(lineIndex) || hideAll) {
        // Lerp the colours towards the background colour. In this array they've already been lerped by their alpha value
        const colA = lerpRGBA(
          [
            colors[lineIndex * 6 + 0],
            colors[lineIndex * 6 + 1],
            colors[lineIndex * 6 + 2],
            1,
          ],
          backgroundColorAsRGBA,
          darkeningFactor,
        )

        const colB = lerpRGBA(
          [
            colors[lineIndex * 6 + 3],
            colors[lineIndex * 6 + 4],
            colors[lineIndex * 6 + 5],
            1,
          ],
          backgroundColorAsRGBA,
          darkeningFactor,
        )

        colors[lineIndex * 6 + 0] = colA[0]
        colors[lineIndex * 6 + 1] = colA[1]
        colors[lineIndex * 6 + 2] = colA[2]
        colors[lineIndex * 6 + 3] = colB[0]
        colors[lineIndex * 6 + 4] = colB[1]
        colors[lineIndex * 6 + 5] = colB[2]
      }
    }

    // Set to our dimmed version
    this.geometry.setColors(colors)
  }

  dispose() {
    this.geometry.dispose()
  }

  getReactComponent() {
    return (
      <primitive object={this.line}>
        <primitive
          object={this.material}
          attach="material"
          vertexColors={true}
          resolution={this.resolution}
          linewidth={this.lineWidth}
          //   {...props}
        />
      </primitive>
    )
  }
}
