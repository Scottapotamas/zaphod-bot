export enum MovementMoveType {
  POINT_TRANSIT = 0,
  LINE,
  CATMULL_SPLINE,
  CATMULL_SPLINE_LINEARISED,
  BEZIER_QUADRATIC,
  BEZIER_QUADRATIC_LINEARISED,
  BEZIER_CUBIC,
  BEZIER_CUBIC_LINEARISED
}

export enum MovementMoveReference {
  ABSOLUTE = 0,
  RELATIVE,
}

export type MovementPoint = [number, number, number] // mm

export type PlannerMovementMove = {
  duration: number // ms
  type: MovementMoveType
  reference: MovementMoveReference
  points: Array<MovementPoint>
}

export enum LightMoveType {
  IMMEDIATE,
  RAMP,
}

export type Hue = number
export type Saturation = number
export type Intensity = number

export type LightPoint = [Hue, Saturation, Intensity]

export type PlannerLightMove = {
  duration: number
  type: LightMoveType
  points: Array<LightPoint>
}
