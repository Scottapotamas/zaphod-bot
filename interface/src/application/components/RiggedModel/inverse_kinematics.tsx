// Types for position and joint angles
export interface CartesianPoint {
  x: number
  y: number
  z: number
}

interface AngleCalcResult {
  ok: boolean
  angle: number
}

// TODO: accept booleans from outside this function
const flip_x: number = 1
const flip_y: number = 1
const flip_z: number = 1

const f: number = 50.0 // radius of motor shafts on base
const rf: number = 180.0 // base joint to elbow joint distance
const re: number = 340.0 // elbow joint to end affector joint
const e: number = 34.0 // end effector joint radius

const offset_position: CartesianPoint = {
  x: 0,
  y: 0,
  z: 190,
}

// Calculation Constants
const sqrt3 = Math.sqrt(3.0)
const sqrt1div3 = Math.sqrt(1.0/3.0)
const sin120 = sqrt3 / 2.0
const sin30 = 0.5
const cos120 = -0.5
const tan60 = sqrt3
const tan30 = 1 / sqrt3

/*
 * Accept a x/y/z cartesian input, write into provided pointer to angle structure
 * Returns 0 when OK, 1 for error
 *
 * Calculate the output motor angles with an IK solver
 * Bounds checks to ensure motors aren't being commanded past their practical limits
 * Set the target angles for the clearpath driver to then handle.
 *
 * Returns status
 */

export function kinematics_point_to_angle(effectorPosition: CartesianPoint, armIndex: number): number {
  // Shallow clone of effector position to prevent mutation
  let position: CartesianPoint = { ...effectorPosition }

  let result: AngleCalcResult = { ok: false, angle: 0 }

  // Offset the work-area position frame into the kinematics domain position
  position.x = (position.x + offset_position.x) * flip_x
  position.y = (position.y + offset_position.y) * flip_y
  position.z = (position.z + offset_position.z) * flip_z

  // Perform kinematics calculations
  result = delta_angle_plane_calc(position.x, position.y, position.z)
  if (armIndex === 0) return result.angle

  if (result.ok) {
    // Rotate +120 degrees
    result = delta_angle_plane_calc(
      position.x * cos120 + position.y * sin120,
      position.y * cos120 - position.x * sin120,
      position.z,
    )
    if (armIndex === 1) return result.angle
  }

  if (result.ok) {
    // Rotate -120 degrees
    result = delta_angle_plane_calc(
      position.x * cos120 - position.y * sin120,
      position.y * cos120 + position.x * sin120,
      position.z,
    )
    if (armIndex === 2) return result.angle
  }

  // TODO: return an error if any of the 3 arm calculations don't resolve?

  return 0
}

/* -------------------------------------------------------------------------- */

// helper function, calculates angle theta (for YZ-pane)
function delta_angle_plane_calc(x0: number, y0: number, z0: number): AngleCalcResult {
  let theta: number = 0

  let y1: number = -0.5 * sqrt1div3 * f // f/2 * tg 30
  y0 -= 0.5 * sqrt1div3 * e // Shift center to edge

  // z = a + b*y
  let a: number = (x0 * x0 + y0 * y0 + z0 * z0 + rf * rf - re * re - y1 * y1) / (2.0 * z0)
  let b: number = (y1 - y0) / z0

  // Discriminant
  let d: number = -(a + b * y1) * (a + b * y1) + rf * (b * b * rf + rf)

  if (d < 0) {
    return { ok: false, angle: theta }
  }

  let yj: number = (y1 - a * b - Math.sqrt(d)) / (b * b + 1) // choose the outer point
  let zj: number = a + b * yj

  theta = (180.0 * Math.atan(-zj / (y1 - yj))) / Math.PI + (yj > y1 ? 180.0 : 0.0)

  return { ok: true, angle: theta }
}
