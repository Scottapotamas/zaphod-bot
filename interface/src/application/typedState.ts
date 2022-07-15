/**
 * Centralised defines for message identifiers to improve readability
 * and allow for easier refactor/sync with embedded hardware
 */
export enum MSGID {
  NICKNAME = 'name',
  RESET_CAUSE = 'rt',
  ASSERT_CAUSE ='am',
  FIRMWARE_INFO = 'fwb',
  ERROR = 'err',

  SYSTEM = 'sx',
  SUPERVISOR = 'ss',
  FAN = 'sf',
  MOTION = 'sm',
  SERVO = 'sc',

  FAN_CURVE = 'curve',
  MODE_REQUEST = 'mr',

  POSITION_TARGET = 'pt',
  POSITION_CURRENT = 'pc',
  POSITION_EXPANSION = 'exp_ang',

  LED = 'rgb',
  LED_MANUAL_REQUEST = 'manual_led',

  QUEUE_INFO = 'qi',
  QUEUE_SYNC = 'qs',
  QUEUE_CLEAR = 'qc',
  QUEUE_ADD_MOVE = 'qm',
  QUEUE_ADD_FADE = 'qf',

  EMERGENCY_STOP = 'E',
  ARM = 'A',
  DISARM = 'D',
  HOME = 'H',
  CAPTURE = 'C',

  LED_CALIBRATION = 'calLED',
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
    [MSGID.NICKNAME]: string
    [MSGID.RESET_CAUSE]: string
    [MSGID.ASSERT_CAUSE]: string
    [MSGID.SYSTEM]: SystemStatus
    [MSGID.SUPERVISOR]: SupervisorState
    [MSGID.FIRMWARE_INFO]: FirmwareBuildInfo

    [MSGID.FAN]: FanStatus
    [MSGID.FAN_CURVE]: never // commented out in firmware

    [MSGID.MODE_REQUEST]: CONTROL_MODES
    [MSGID.MOTION]: MotionState
    [MSGID.SERVO]:
      | [ServoInfo, ServoInfo, ServoInfo]
      | [ServoInfo, ServoInfo, ServoInfo, ServoInfo] // if expansion slot is in

    [MSGID.POSITION_TARGET]: CartesianPoint
    [MSGID.POSITION_CURRENT]: CartesianPoint
    [MSGID.POSITION_EXPANSION]: number // float

    [MSGID.LED]: LedStatus
    [MSGID.LED_MANUAL_REQUEST]: LedStatus

    [MSGID.QUEUE_INFO]: QueueDepthInfo
    [MSGID.QUEUE_SYNC]: null // callback
    [MSGID.QUEUE_CLEAR]: null // callback
    [MSGID.QUEUE_ADD_MOVE]: MovementMove
    [MSGID.QUEUE_ADD_FADE]: LightMove

    [MSGID.EMERGENCY_STOP]: null // callback
    [MSGID.ARM]: null // callback
    [MSGID.DISARM]: null // callback
    [MSGID.HOME]: null // callback
    [MSGID.CAPTURE]: number // uint32 ms

    [MSGID.LED_CALIBRATION]: LedSettings
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
  bicep_length: number
  forearm_length: number
  effector_radius: number

  // Limits in cartesian space
  limit_radius: number
  limit_z_min: number
  limit_z_max: number

  // Flags if an axis is inverted
  flip_x: number
  flip_y: number
  flip_z: number
}

export enum FanState {
  OFF = 0,
  STALL = 1,
  STARTUP = 2,
  OK = 3,
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
  state: number         //state machine enum value
  feedback: number      // percentage of max torque
  target_angle: number  //degrees
  power: number         // watts
  speed: number         // degrees per second
}

export type MotionState = {
  pathing_state: number
  motion_state: number
  profile_type: number
  move_progress: number
  movement_identifier: number
  effector_speed: number  // mm/second
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
  sync_offset: number
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
  timestamp: number
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
export {}
