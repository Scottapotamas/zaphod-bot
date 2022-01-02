import * as React from 'react'
import { useRef, useMemo, useState, forwardRef } from 'react'

// import // Environment,
// // OrbitControls,
// '@electricui/components-desktop-three'

import { Canvas } from '@react-three/fiber'

import {
  Line,
  CubicBezierLine,
  Segments,
  Segment,
  OrbitControls,
  LineProps,
} from '@react-three/drei'

import { useFrame, useThree } from '@react-three/fiber'
import {
  Mesh,
  Group,
  Color,
  Vector2,
  Material,
  InstancedInterleavedBuffer,
  InterleavedBufferAttribute,
} from 'three'

import { MovementMoveType } from '../optimiser/hardware'
import { sparseToDense, Toolpath } from '../optimiser/passes'
import { CatmullRomLine } from './CatmullLine'
import { MovementPoint } from 'src/application/typedState'
import { Vector3 } from 'three'
import { RGBA, XYZ } from '../optimiser/movements'
import { Line2 } from 'three/examples/jsm/lines/Line2'
import { LineMaterial } from 'three/examples/jsm/lines/LineMaterial'
import { LineSegmentsGeometry } from 'three/examples/jsm/lines/LineSegmentsGeometry'

// Since we want to be able to scroll through the timeline at 60fps, we can't use React's render loop to handle line segment state.

interface BufferedLineSegmentProps {
  lineStarts: XYZ[]
  lineEnds: XYZ[]
  lineColors: RGBA[]
  dashed?: boolean
  limit?: number
  lineWidth?: number
}

const BufferedLineSegment = forwardRef<Line2, BufferedLineSegmentProps>(
  (props, forwardedRef) => {
    const {
      limit = 4000,
      lineWidth = 1.0,
      children,
      dashed,
      lineStarts,
      lineEnds,
      lineColors,
      ...rest
    } = props

    const [line] = useState(() => new Line2())
    const [material] = useState(() => new LineMaterial({ dashed }))
    const [geometry] = useState(() => new LineSegmentsGeometry())
    const [resolution] = useState(() => new Vector2(512, 512))

    const [positions] = useState(() => Array(limit * 6).fill(0))
    const [colors] = useState(() => Array(limit * 6).fill(0))

    useFrame(() => {
      for (let i = 0; i < limit; i++) {
        const segmentStart = lineStarts[i] ? lineStarts[i] : [0, 0, 0]
        const segmentEnd = lineEnds[i] ? lineEnds[i] : [0, 0, 0]
        const segmentColor = lineColors[i] ? lineColors[i] : [1, 1, 1]

        for (var j = 0; j < 3; j++) {
          positions[i * 6 + j] = segmentStart[j]
          positions[i * 6 + j + 3] = segmentEnd[j]
          colors[i * 6 + j] = segmentColor[j]
          colors[i * 6 + j + 3] = segmentColor[j]
        }
      }
      geometry.setColors(colors)
      geometry.setPositions(positions)
      line.computeLineDistances()
    })

    return (
      <primitive object={line} ref={forwardedRef}>
        <primitive object={geometry} attach="geometry" />
        <primitive
          object={material}
          attach="material"
          vertexColors={true}
          resolution={resolution}
          linewidth={lineWidth}
          {...rest}
        />
      </primitive>
    )
  },
)

export class MutableLineSegmentGeometry {
  public line
  public material: LineMaterial
  public geometry = new LineSegmentsGeometry()
  public resolution = new Vector2(512, 512)

  public positions: number[] = []
  public colors: number[] = []

  constructor(dashed: boolean, public lineWidth: number) {
    this.line = new Line2(this.geometry as any)
    this.material = new LineMaterial({ dashed })

    this.refreshGeometry = this.refreshGeometry.bind(this)
    this.dispose = this.dispose.bind(this)
    this.getReactComponent = this.getReactComponent.bind(this)
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
