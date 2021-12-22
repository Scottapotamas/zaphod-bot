import * as React from 'react'
import { CatmullRomCurve3, Vector3 } from 'three'
import { Line2 } from 'three/examples/jsm/lines/Line2'
import { Line, LineProps } from '@react-three/drei'
import { MathUtils } from 'three'

type Props = Omit<LineProps, 'points' | 'ref'> & {
  points: (Vector3 | [number, number, number])[]
  segments?: number
  closed?: boolean
  curveType?: 'centripetal' | 'chordal' | 'catmullrom'
  tension?: number
}

export const CatmullRomLine = React.forwardRef<Line2, Props>(
  function CatmullRomLine(
    { points, segments = 20, closed, curveType, tension, ...rest },
    ref,
  ) {
    const pointsInterpolated = React.useMemo(() => {
      const vectors = points.map(point =>
        point instanceof Vector3 ? point : new Vector3(...point),
      )

      const curve = new CatmullRomCurve3(vectors, closed, curveType, tension)

      // By default CatmullRomCurve3.getPoints(segments) gives us points along the entire length of vectors,
      // including the control points, we need to only build points between the start and end.

      // The control points are 1/3 and 2/3 of the way along the curve.

      const segmentPoints: Vector3[] = []

      for (let index = 0; index < segments; index++) {
        // remap 0 -> num segments to a float between 1/3 and 2/3
        const t = MathUtils.mapLinear(index, 0, segments, 1 / 3, 2 / 3)

        segmentPoints.push(curve.getPoint(t))
      }

      return segmentPoints
    }, [points, segments, closed, curveType, tension])

    return <Line ref={ref as any} points={pointsInterpolated} {...rest} />
  },
)
