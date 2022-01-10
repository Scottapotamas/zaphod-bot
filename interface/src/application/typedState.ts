/**
 * Centralised defines for message identifiers to improve readability
 * and allow for easier refactor/sync with embedded hardware
 */
 export enum MSGID {
  NICKNAME = "name",
  RESET_CAUSE = "reset_type",
  SYSTEM = "sys",
  SUPERVISOR = "super",
  FIRMWARE_INFO = "fwb",
  
  FAN = "fan",
  FAN_CURVE = "curve",
  
  MODE_REQUEST = "req_mode",
  MOTION = "moStat",
  SERVO = "servo",
  
  POSITION_TARGET = "tpos",
  POSITION_CURRENT = "cpos",
  POSITION_EXPANSION = "exp_ang",
  
  LED = "rgb",
  LED_MANUAL_REQUEST = "manual_led",
  
  QUEUE_INFO = "queue",
  QUEUE_SYNC = "sync",
  QUEUE_SYNC_ID = "syncid",
  QUEUE_CLEAR = "clmv",
  QUEUE_START = "stmv",
  QUEUE_ADD_MOVE = "inmv",
  QUEUE_ADD_FADE = "inlt",
  
  EMERGENCY_STOP = "estop",
  ARM = "arm",
  DISARM = "disarm",
  HOME = "home",
  CAPTURE = "capture",

  LED_CALIBRATION = "calLED",
}

/**
 * To strictly type all accessors and writers, remove
 *
 * [messageID: string]: any
 *
 * And replace with your entire state shape after codecs have decoded them.
 */
declare global {
  interface ElectricUIDeveloperState {
    [messageID: string]: any

    // Example messageID typings
    // led_blink: number
    // led_state: number
    // lit_time: number
  }
}

export type SystemStatus = {
  sensors_enable: number
  module_enable: number
  cpu_load: number
  cpu_clock: number
  input_voltage: number
  temp_ambient: number
  temp_regulator: number
  temp_supply: number
  temp_cpu: number
}

export type FirmwareBuildInfo = {
  branch: string
  info: string
  date: string
  time: string
  type: string
  name: string
}

export type KinematicsInfo = {
  // Dimensions used in the IK/FK calculations
  shoulder_radius: number
  bicep_length: number,
  forearm_length: number,
  effector_radius: number,

  // Limits in cartesian space
  limit_radius: number,
  limit_z_min: number,
  limit_z_max: number,

  // Flags if an axis is inverted
  flip_x: number,
  flip_y: number,
  flip_z: number,

}

export type FanStatus = {
  rpm: number
  setpoint: number
  state: number
}

export type QueueDepthInfo = {
  movements: number
  lighting: number
}



export type ServoInfo = {
  enabled: boolean
  state: number
  feedback: number
  target_angle: number
  power: number
}

export type MotionState = {
  pathing_state: number
  motion_state: number
  profile_type: number
  move_progress: number
  movement_identifier: number
}

export enum SUPERVISOR_STATES {
  NONE,
  MAIN,
  IDLE,
  ARMING,
  ERROR,
  SUCCESS,
  ARMED,
  DISARMING,
  NULL,
}

export enum CONTROL_MODES {
  NONE = 0,
  MANUAL,
  EVENT,
  TRACK,
  DEMO,
  CHANGING,
}

export type SupervisorState = {
  supervisor: string
  motors: number
  mode: string
}

export enum MovementMoveType {
  POINT_TRANSIT = 0,
  LINE,
  CATMULL_SPLINE,
  BEZIER_QUADRATIC,
  BEZIER_CUBIC,
}

export enum MovementMoveReference {
  ABSOLUTE = 0,
  RELATIVE,
}

export type MovementPoint = [number, number, number] // mm

export type CartesianPoint = {
  x: number
  y: number
  z: number
}

export type MovementMove = {
  id: number
  duration: number
  type: MovementMoveType
  reference: MovementMoveReference
  points: Array<MovementPoint>
  num_points?: number
}

export enum LightMoveType {
  IMMEDIATE,
  RAMP,
}

export type Hue = number
export type Saturation = number
export type Intensity = number

export type LightPoint = [Hue, Saturation, Intensity]

export type LightMove = {
  id: number
  duration: number
  type: LightMoveType
  points: Array<LightPoint>
  num_points?: number
}

export type LedStatus = {
  red: number
  green: number
  blue: number
  enable: boolean
}

export type LedSettings = {
  correct_luma: boolean
  correct_whitebalance: boolean
  offset_red: number
  offset_green: number
  offset_blue: number
  offset_global: number
}

export type PowerCalibration = {
  voltage: number
  current_servo_1: number
  current_servo_2: number
  current_servo_3: number
  current_servo_4: number
}

// This exports these types into the dependency tree.
export { }
