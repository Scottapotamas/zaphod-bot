import * as THREE from 'three'

export class PolyLineCurve3 extends THREE.CatmullRomCurve3 {
  getPoint(t: number) {
    const points = this.points
    const l = points.length

    if (l < 2) {
      throw new Error('Incorrect number of points for the PolyLineCurve3')
    }

    const point = (l - 1) * t
    let intPoint = Math.floor(point)
    let weight = point - intPoint

    if (weight === 0 && intPoint === l - 1) {
      intPoint = l - 2
      weight = 1
    }

    const p1 = points[intPoint % l]
    const p2 = points[(intPoint + 1) % l]

    return p1.clone().lerp(p2, weight)
  }
}
