import { Codec, Message } from '@electricui/core'

import {
  MSGID,
  SystemStatus,
  KinematicsInfo,
  FirmwareBuildInfo,
  FanStatus,
  EffectorData,
  QueueDepthInfo,
  ServoInfo,
  MotionState,
  SUPERVISOR_STATES,
  CONTROL_MODES,
  SupervisorState,
  MovementMoveType,
  MovementMoveReference,
  MovementPoint,
  CartesianPoint,
  MovementMove,
  LightMoveType,
  LightSettingsField,
  LightMove,
  LightPoint,
  LedStatus,
  LedSettings,
  PowerCalibration,
  BOUNDARY_VIOLATION_MODES,
  Z_ROTATION_SCALE_FACTOR,
  SPEED_LIMIT_SCALE_FACTOR,
  UserConfigFlags,
  UserConfigFields,
  UserConfig,
} from '../../application/typedState'
import { SmartBuffer } from 'smart-buffer'

export class SystemDataCodec extends Codec {
  filter(message: Message): boolean {
    return message.messageID === MSGID.SYSTEM
  }

  encode(payload: SystemStatus): Buffer {
    throw new Error('System info is a read-only packet')
  }

  decode(payload: Buffer): SystemStatus {
    const reader = SmartBuffer.fromBuffer(payload)

    return {
      sensors_enable: reader.readUInt8(),
      module_enable: reader.readUInt8(),
      cpu_load: reader.readUInt8(),
      cpu_clock: reader.readUInt8(),

      input_voltage: reader.readInt16LE() / 100,
      temp_ambient: reader.readInt16LE() / 100,
      temp_regulator: reader.readInt16LE() / 100,
      temp_supply: reader.readInt16LE() / 100,
      temp_cpu: reader.readInt16LE() / 100,
    }
  }
}

const build_info_bytes: number = 12

export function splitBufferByLength(toSplit: Buffer, splitLength: number) {
  const chunks = []
  const n = toSplit.length
  let i = 0

  // if the result is only going to be one chunk, just return immediately.
  if (toSplit.length < splitLength) {
    return [toSplit]
  }
  while (i < n) {
    let end = i + splitLength
    chunks.push(toSplit.slice(i, end))
    i = end
  }
  return chunks
}

export class FirmwareInfoCodec extends Codec {
  filter(message: Message): boolean {
    return message.messageID === MSGID.FIRMWARE_INFO
  }

  encode(payload: FirmwareBuildInfo): Buffer {
    throw new Error('Firmware/build info is read-only')
  }

  decode(payload: Buffer): FirmwareBuildInfo {
    const chunks = splitBufferByLength(payload, build_info_bytes)
    const strings = chunks.map(chunk =>
      SmartBuffer.fromBuffer(chunk).readStringNT(),
    )

    return {
      branch: strings[0],
      info: strings[1],
      date: strings[2],
      time: strings[3],
      type: strings[4],
      name: strings[5],
    }
  }
}

export class KinematicsInfoCodec extends Codec {
  filter(message: Message): boolean {
    return message.messageID === 'kinematics'
  }

  encode(payload: KinematicsInfo): Buffer {
    throw new Error('Kinematics info is read-only')
  }

  decode(payload: Buffer): KinematicsInfo {
    const reader = SmartBuffer.fromBuffer(payload)

    return {
      // Dimensions used in the IK/FK calculations
      shoulder_radius: reader.readUInt32LE() / 1000,
      bicep_length: reader.readUInt32LE() / 1000,
      forearm_length: reader.readUInt32LE() / 1000,
      effector_radius: reader.readUInt32LE() / 1000,

      // Limits in cartesian space
      limit_radius: reader.readInt32LE() / 1000,
      limit_z_min: reader.readInt32LE() / 1000,
      limit_z_max: reader.readInt32LE() / 1000,

      // Flags if an axis is inverted
      flip_x: reader.readInt8(),
      flip_y: reader.readInt8(),
      flip_z: reader.readInt8(),
    }
  }
}

export class FanCodec extends Codec {
  filter(message: Message): boolean {
    return message.messageID === MSGID.FAN
  }

  encode(payload: FanStatus): Buffer {
    throw new Error('Fan info is read-only')
  }

  decode(payload: Buffer): FanStatus {
    const reader = SmartBuffer.fromBuffer(payload)

    return {
      rpm: reader.readUInt16LE(),
      setpoint: reader.readUInt8(),
      state: reader.readUInt8(),
    }
  }
}

export class MotorDataCodec extends Codec {
  filter(message: Message): boolean {
    return message.messageID === MSGID.SERVO
  }

  encode(payload: ServoInfo): Buffer {
    throw new Error('servo telemetryis read-only')
  }

  decode(payload: Buffer): ServoInfo[] {
    const reader = SmartBuffer.fromBuffer(payload)

    const servoStats: ServoInfo[] = []

    while (reader.remaining() > 0) {
      const motor: ServoInfo = {
        enabled: reader.readUInt8() === 0x01 ? true : false,
        state: reader.readUInt8(),
        feedback: reader.readInt16LE() / 10,
        target_angle: reader.readFloatLE(),
        power: reader.readFloatLE(),
        speed: reader.readFloatLE(),
      }
      servoStats.push(motor)
    }

    return servoStats
  }
}

export class MotionDataCodec extends Codec {
  filter(message: Message): boolean {
    return message.messageID === MSGID.MOTION
  }

  encode(payload: MotionState): Buffer {
    throw new Error('motion engine state info is read-only')
  }

  decode(payload: Buffer): MotionState {
    const reader = SmartBuffer.fromBuffer(payload)

    return {
      pathing_state: reader.readUInt8(),
      motion_state: reader.readUInt8(),
      profile_type: reader.readUInt8(),
      move_progress: reader.readUInt8(),

      movement_identifier: reader.readUInt32LE(),

      //microns per second is sent, convert to mm/second for UI use
      effector_speed: reader.readUInt32LE() / 1000, 
    }
  }
}

export class EffectorDataCodec extends Codec {
  filter(message: Message): boolean {
    return (
      message.messageID === MSGID.POSITION_CURRENT
    )
  }

  encode(payload: EffectorData): Buffer {
    const packet = new SmartBuffer()

    packet.writeInt32LE(payload.position.x * 1000)
    packet.writeInt32LE(payload.position.y * 1000)
    packet.writeInt32LE(payload.position.z * 1000)
    packet.writeUInt32LE(payload.speed / 1000 )

    return packet.toBuffer()
  }

  decode(payload: Buffer): EffectorData {
    const reader = SmartBuffer.fromBuffer(payload)

    let pos: CartesianPoint = {
      x: reader.readInt32LE() / 1000,
      y: reader.readInt32LE() / 1000,
      z: reader.readInt32LE() / 1000,

    }

    return {
      position: pos,
      // Packet is microns/second, convert to mm/second
      speed: reader.readInt32LE() * 1000
    }
  }
}

export class PositionTargetCodec extends Codec {
  filter(message: Message): boolean {
    return (
      message.messageID === MSGID.POSITION_TARGET
    )
  }

  encode(payload: CartesianPoint): Buffer {
    const packet = new SmartBuffer()

    packet.writeInt32LE(payload.x * 1000)
    packet.writeInt32LE(payload.y * 1000)
    packet.writeInt32LE(payload.z * 1000)

    return packet.toBuffer()
  }

  decode(payload: Buffer): CartesianPoint {
    const reader = SmartBuffer.fromBuffer(payload)

    return {
      x: reader.readInt32LE() / 1000,
      y: reader.readInt32LE() / 1000,
      z: reader.readInt32LE() / 1000,
    }
  }
}

export class ExpansionPositionCodec extends Codec {
  filter(message: Message): boolean {
    return (
      message.messageID === MSGID.POSITION_EXPANSION
    )
  }

  encode(payload: number): Buffer {
    const packet = new SmartBuffer()

    packet.writeInt32LE(payload * 1000)
    return packet.toBuffer()
  }

  decode(payload: Buffer): number {
    const reader = SmartBuffer.fromBuffer(payload)

    return reader.readInt32LE() / 1000
  }
}

export class SupervisorInfoCodec extends Codec {
  filter(message: Message): boolean {
    return message.messageID === MSGID.SUPERVISOR
  }

  encode(payload: SupervisorState): Buffer {
    throw new Error('Supervisor state info is read-only')
  }

  decode(payload: Buffer): SupervisorState {
    const reader = SmartBuffer.fromBuffer(payload)

    return {
      supervisor: SUPERVISOR_STATES[reader.readUInt8()] || SUPERVISOR_STATES[SUPERVISOR_STATES.DISARMED],
      motors: reader.readUInt8(),
      mode: CONTROL_MODES[reader.readUInt8()] || CONTROL_MODES[CONTROL_MODES.NONE],
    }
  }
}

export class QueueDepthCodec extends Codec {
  filter(message: Message): boolean {
    return message.messageID === MSGID.QUEUE_INFO
  }

  encode(payload: QueueDepthInfo): Buffer {
    throw new Error('queue depth info is read-only')
  }

  decode(payload: Buffer): QueueDepthInfo {
    const reader = SmartBuffer.fromBuffer(payload)

    return {
      movements: reader.readUInt8(),
      lighting: reader.readUInt8(),
    }
  }
}

export class InboundMotionCodec extends Codec {
  filter(message: Message): boolean {
    return message.messageID === MSGID.QUEUE_ADD_MOVE
  }

  encode(payload: MovementMove): Buffer {
    const packet = new SmartBuffer()

    // 2-byte metadata bitfield
    let meta = 0x0000;

    meta |= (payload.id)                  // 9 bits
    meta |= (payload.reference << 9)      // 1 bit
    meta |= (payload.type << 10)          // 3 bits
    meta |= (payload.points.length << 13)  // 3 bits

    packet.writeUInt16LE(meta)

    packet.writeUInt16LE(payload.duration)
    packet.writeUInt32LE(payload.sync_offset)

    for (let index = 0; index < 4; index++) {
      const pointData = payload.points[index]

      if (typeof pointData !== 'undefined') {
        packet.writeInt32LE(pointData[0] * 1000)
        packet.writeInt32LE(pointData[1] * 1000)
        packet.writeInt32LE(pointData[2] * 1000)
      } else {
        packet.writeInt32LE(0)
        packet.writeInt32LE(0)
        packet.writeInt32LE(0)
      }
    }

    return packet.toBuffer()
  }

  decode(payload: Buffer): MovementMove {
    const reader = SmartBuffer.fromBuffer(payload)

    const points_decoded: Array<MovementPoint> = []

    // 2-byte bitfield packing
    let meta = reader.readUInt16LE()

    let move_identifier = ( meta & 0x01FF )   // 9 bits
    let reference = ( (meta >> 9) & 0x01 )    // 1 bit
    let type = ( (meta >> 10) & 0x07 )        // 3 bits
    let num_points = ( (meta >> 13) & 0x07 )  // 3 bits

    let duration = reader.readUInt16LE()
    let sync_offset = reader.readUInt32LE()

    const movement: MovementMove = {
      type: type,
      reference: reference,
      num_points: num_points,
      id: move_identifier,
      sync_offset: sync_offset,
      duration: duration,
      points: points_decoded,
    }

    for (let index = 0; index < 4; index++) {
      const pointData: MovementPoint = [
        reader.readInt32LE(),
        reader.readInt32LE(),
        reader.readInt32LE(),
      ]
      points_decoded.push(pointData)
    }

    return movement
  }
}

export class InboundFadeCodec extends Codec {
  filter(message: Message): boolean {
    return message.messageID === MSGID.QUEUE_ADD_FADE
  }

  encode(payload: LightMove): Buffer {
    const packet = new SmartBuffer()

    payload.settings.num_points = payload.points.length

    let bitfield1 = 0x00;
    let bitfield2 = 0xFF;  // currently unused

    bitfield1 |= (payload.settings.num_points) 
    bitfield1 |= (payload.settings.type << 2 ) 
    bitfield1 |= ((payload.settings.speed_compensated ? 1 : 0) << 6 )
    bitfield1 |= ((payload.settings.positional_noise ? 1 : 0) << 7 )

    packet.writeUInt32LE(payload.timestamp)
    packet.writeUInt16LE(payload.duration)
    packet.writeUInt8(bitfield1)
    packet.writeUInt8(bitfield2)

    for (let index = 0; index < 2; index++) {
      const pointData = payload.points[index]

      if (typeof pointData !== 'undefined') {
        packet.writeFloatLE(pointData[0])
        packet.writeFloatLE(pointData[1])
        packet.writeFloatLE(pointData[2])
      } else {
        packet.writeFloatLE(0)
        packet.writeFloatLE(0)
        packet.writeFloatLE(0)
      }
    }

    return packet.toBuffer()
  }

  decode(payload: Buffer): LightMove {
    const reader = SmartBuffer.fromBuffer(payload)

    const points_decoded: Array<LightPoint> = []
    let settings:LightSettingsField = {} as LightSettingsField

    const movement: LightMove = {
      timestamp: reader.readUInt32LE(),
      duration: reader.readUInt16LE(),
      settings: settings,
      points: points_decoded,
    }

    let b1 = reader.readUInt8()
    let b2 = reader.readUInt8() // unused byte

    settings.num_points = (b1 & 0x03)
    settings.type = ((b1 >> 2) & 0x0F)
    settings.speed_compensated = ((b1 >> 6) & 0x01) == 1
    settings.positional_noise = ((b1 >> 7) & 0x01) == 1

    for (let index = 0; index < 2; index++) {
      const pointData: LightPoint = [
        reader.readFloatLE(),
        reader.readFloatLE(),
        reader.readFloatLE(),
      ]
      points_decoded.push(pointData)
    }

    return movement
  }
}

export class LEDCodec extends Codec<LedStatus> {
  filter(message: Message): boolean {
    return (
      message.messageID === MSGID.LED ||
      message.messageID === MSGID.LED_MANUAL_REQUEST
    )
  }

  encode(payload: LedStatus) {
    const packet = new SmartBuffer()

    packet.writeUInt16LE(payload.red * 0xFFFF )
    packet.writeUInt16LE(payload.green * 0xFFFF )
    packet.writeUInt16LE(payload.blue * 0xFFFF )
    packet.writeUInt8(payload.enable ? 1 : 0)

    return packet.toBuffer()
  }

  decode(payload: Buffer) {
    const reader = SmartBuffer.fromBuffer(payload)

    const settings: LedStatus = {
      red: reader.readUInt16LE() / 0xFFFF,
      green: reader.readUInt16LE() / 0xFFFF,
      blue: reader.readUInt16LE() / 0xFFFF,
      enable: reader.readUInt8() === 1 ? true : false,
    }

    return settings
  }
}

export class RGBSettingsCodec extends Codec {
  filter(message: Message): boolean {
    return message.messageID === MSGID.LED_CALIBRATION
  }

  encode(payload: LedSettings) {
    const packet = new SmartBuffer()

    let luma_value = (payload.correct_luma)? 1 : 0;
    let wb_value = (payload.correct_whitebalance)? 1 : 0;

    packet.writeUInt8(luma_value)
    packet.writeUInt8(wb_value)
    packet.writeUInt16LE(payload.offset_red * 0xFFFF)
    packet.writeUInt16LE(payload.offset_green * 0xFFFF)
    packet.writeUInt16LE(payload.offset_blue * 0xFFFF)
    packet.writeUInt16LE(payload.offset_global * 0xFFFF)

    return packet.toBuffer()
  }

  decode(payload: Buffer): LedSettings {
    const reader = SmartBuffer.fromBuffer(payload)

    return {
      correct_luma: reader.readUInt8() === 0x01 ? true : false,
      correct_whitebalance: reader.readUInt8() === 0x01 ? true : false,
      offset_red: reader.readUInt16LE() / 0xFFFF,
      offset_green: reader.readUInt16LE() / 0xFFFF,
      offset_blue: reader.readUInt16LE() / 0xFFFF,
      offset_global: reader.readUInt16LE() / 0xFFFF,
    }
  }
}

export class PowerCalibrationCodec extends Codec {
  filter(message: Message): boolean {
    return message.messageID === MSGID.POWER_CALIBRATION
  }

  encode(payload: PowerCalibration) {
    // SmartBuffers automatically keep track of read and write offsets / cursors.
    const packet = new SmartBuffer()

    packet.writeInt16LE(payload.voltage)
    packet.writeInt16LE(payload.current_servo[0])
    packet.writeInt16LE(payload.current_servo[1])
    packet.writeInt16LE(payload.current_servo[2])
    packet.writeInt16LE(payload.current_servo[3])

    return packet.toBuffer()
  }

  decode(payload: Buffer) {
    const reader = SmartBuffer.fromBuffer(payload)

    let calibration:PowerCalibration = {
      voltage: reader.readInt16LE(),
      current_servo: []
    }

    calibration.current_servo[0] = reader.readInt16LE()
    calibration.current_servo[1] = reader.readInt16LE()
    calibration.current_servo[2] = reader.readInt16LE()
    calibration.current_servo[3] = reader.readInt16LE()

    return calibration
  }
}

export class UserConfigCodec extends Codec {
  filter(message: Message): boolean {
    return message.messageID === MSGID.USER_CONFIG
  }

  encode(payload: UserConfig) {
    const packet = new SmartBuffer()

    let bitfield1 = 0x00;
    let bitfield2 = 0xAA;  // currently reserved
    let bitfield3 = 0x00;  // currently reserved
    let bitfield4 = 0xEE;  // currently reserved

    bitfield1 |= (payload.flags.buzzer_mute ? 1 : 0) 
    bitfield1 |= ((false ? 1 : 0) << 1 )  // field no longer used - replace as needed
    bitfield1 |= ((payload.flags.pendant_optional ? 1 : 0) << 2 )
    bitfield1 |= ((payload.flags.pendant_verify_on_arm ? 1 : 0) << 3 )
    bitfield1 |= ((payload.flags.pendant_light_enabled ? 1 : 0) << 4 )
    bitfield1 |= ((payload.flags.inverted ? 1 : 0) << 5 )
    bitfield1 |= (payload.flags.boundary_violation_mode << 6 )

    bitfield3 |= (payload.flags.expansion_enabled ? 1 : 0) 
    bitfield3 |= (payload.flags.expansion_type << 1 )
    bitfield3 |= (payload.flags.expansion_feedback << 3 )
    bitfield3 |= ((payload.flags.expansion_requires_homing ? 1 : 0) << 5 )
    bitfield3 |= ((payload.flags.expansion_reverse ? 1 : 0) << 6 )

    packet.writeUInt8(bitfield1)
    packet.writeUInt8(bitfield2)
    packet.writeUInt8(bitfield3)
    packet.writeUInt8(bitfield4)

    packet.writeUInt8(payload.values.z_rotation/Z_ROTATION_SCALE_FACTOR)
    packet.writeUInt8(payload.values.speed_limit/SPEED_LIMIT_SCALE_FACTOR)
    packet.writeUInt8(payload.values.volume_radius)
    packet.writeUInt8(payload.values.volume_z)

    packet.writeUInt8(payload.values.expansion_resolution)
    packet.writeUInt8(payload.values.expansion_ratio)
    packet.writeUInt8(payload.values.expansion_speed_limit/SPEED_LIMIT_SCALE_FACTOR)
    packet.writeUInt8(0xFF) // currently reserved values

    packet.writeInt16LE(payload.values.expansion_range_min)
    packet.writeInt16LE(payload.values.expansion_range_max)
  
    

    return packet.toBuffer()
  }

  decode(payload: Buffer): UserConfig {
    const reader = SmartBuffer.fromBuffer(payload)

    let b1 = reader.readUInt8()
    let b2 = reader.readUInt8() // reserved byte
    let b3 = reader.readUInt8() // reserved byte
    let b4 = reader.readUInt8() // reserved byte

    let flags:UserConfigFlags = {
      buzzer_mute: (b1 & 0x01) == 1,
      // reserved0: ((b1 >> 1) & 0x01) == 1,
      pendant_optional: ((b1 >> 2) & 0x01) == 1,
      pendant_verify_on_arm: ((b1 >> 3) & 0x01) == 1,
      pendant_light_enabled: ((b1 >> 4) & 0x01) == 1,
      inverted: ((b1 >> 5) & 0x01) == 1,
      boundary_violation_mode: (b1 >> 6) & 0x03,  //BOUNDARY_VIOLATION_MODES

      // byte 2 left reserved

      expansion_enabled: (b3 & 0x01) == 1,
      expansion_type: ((b3 >> 1) & 0x03),       // EXPANSION_MOTION_TYPES;
      expansion_feedback: ((b3 >> 3) & 0x03),   //EXPANSION_FEEDBACK_SIGNAL;
      expansion_requires_homing: ((b3 >> 5) & 0x01) == 1,
      expansion_reverse: ((b3 >> 6) & 0x01) == 1,

      // byte 4 left reserved
    }

    let fields:UserConfigFields = {
      z_rotation: reader.readUInt8()*Z_ROTATION_SCALE_FACTOR,
      speed_limit: reader.readUInt8()*SPEED_LIMIT_SCALE_FACTOR,
      volume_radius: reader.readUInt8(),
      volume_z: reader.readUInt8(),
      expansion_resolution: reader.readUInt8(),
      expansion_ratio: reader.readUInt8(),
      expansion_speed_limit: reader.readUInt8()*SPEED_LIMIT_SCALE_FACTOR,
      expansion_range_min: 0,
      expansion_range_max: 0,
    }

    let r12 = reader.readUInt8() // reserved value

    fields.expansion_range_min = reader.readInt16LE()
    fields.expansion_range_max = reader.readInt16LE()

    return {
      flags: flags,
      values: fields
    }
  }
}

// Create the instances of the codecs
export const customCodecs = [
  new SystemDataCodec(),
  new FirmwareInfoCodec(),
  new KinematicsInfoCodec(),
  new FanCodec(),
  new QueueDepthCodec(),
  new MotorDataCodec(),
  new MotionDataCodec(),
  new EffectorDataCodec(),
  new PositionTargetCodec(),
  new ExpansionPositionCodec(),
  new SupervisorInfoCodec(),
  new InboundMotionCodec(),
  new InboundFadeCodec(),
  new LEDCodec(),
  new RGBSettingsCodec(),
  new PowerCalibrationCodec(),
  new UserConfigCodec(),
]
