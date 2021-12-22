import { Codec, Message } from '@electricui/core'

import {
  MSGID,
  SystemStatus,
  KinematicsInfo,
  FirmwareBuildInfo,
  FanStatus,
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
  LightMove,
  LightPoint,
  LedStatus,
  LedSettings,
  PowerCalibration,
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
    const chunks = splitBufferByLength(payload, 12)
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
      motion_state: reader.readUInt8(), // this isn't called the queue state on embedded
      profile_type: reader.readUInt8(),
      move_progress: reader.readUInt8(),

      movement_identifier: reader.readUInt16LE(),
    }
  }
}

export class PositionCodec extends Codec {
  filter(message: Message): boolean {
    return message.messageID === MSGID.POSITION_TARGET || message.messageID === MSGID.POSITION_CURRENT
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
      supervisor: SUPERVISOR_STATES[reader.readUInt8()] || 'UNKNOWN',
      motors: reader.readUInt8(),
      mode: CONTROL_MODES[reader.readUInt8()] || 'UNKNOWN',
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

    payload.num_points = payload.points.length

    packet.writeUInt8(payload.type)
    packet.writeUInt8(payload.reference)
    packet.writeUInt16LE(payload.id)
    packet.writeUInt16LE(payload.duration)
    packet.writeUInt16LE(payload.num_points)

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

    const movement: MovementMove = {
      type: reader.readUInt8(),
      reference: reader.readUInt8(),
      id: reader.readUInt16LE(),
      duration: reader.readUInt16LE(),
      num_points: reader.readUInt16LE(),
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

    payload.num_points = payload.points.length

    packet.writeUInt16LE(payload.id)
    packet.writeUInt16LE(payload.duration)
    packet.writeUInt8(payload.type)
    packet.writeUInt8(payload.num_points)
    packet.writeUInt8(0x00)
    packet.writeUInt8(0x00)

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

    const movement: LightMove = {
      id: reader.readUInt16LE(),
      duration: reader.readUInt16LE(),
      type: reader.readUInt8(),
      num_points: reader.readUInt8(),
      points: points_decoded,
    }

    // two padding bytes
    const garbage = reader.readUInt16LE()

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
    return message.messageID === MSGID.LED || message.messageID === MSGID.LED_MANUAL_REQUEST
  }

  encode(payload: LedStatus) {
    const packet = new SmartBuffer()

    packet.writeUInt16LE(payload.red)
    packet.writeUInt16LE(payload.green)
    packet.writeUInt16LE(payload.blue)
    packet.writeUInt8(payload.enable ? 1 : 0)

    return packet.toBuffer()
  }

  decode(payload: Buffer) {
    const reader = SmartBuffer.fromBuffer(payload)

    const settings: LedStatus = {
      red: reader.readUInt16LE(),
      green: reader.readUInt16LE(),
      blue: reader.readUInt16LE(),
      enable: reader.readUInt8() === 1 ? true : false,
    }

    return settings
  }
}

export class RGBSettingsCodec extends Codec {
  filter(message: Message): boolean {
    return message.messageID === 'ledset'
  }

  encode(payload: LedSettings) {
    const packet = new SmartBuffer()

    packet.writeInt16LE(payload.offset_red * 32767)
    packet.writeInt16LE(payload.offset_green * 32767)
    packet.writeInt16LE(payload.offset_blue * 32767)
    packet.writeInt16LE(payload.offset_global * 32767)

    return packet.toBuffer()
  }

  decode(payload: Buffer): LedSettings {
    const reader = SmartBuffer.fromBuffer(payload)

    return {
      offset_red: reader.readInt16LE() / 32767,
      offset_green: reader.readInt16LE() / 32767,
      offset_blue: reader.readInt16LE() / 32767,
      offset_global: reader.readInt16LE() / 32767,
    }
  }
}

export class PowerCalibrationCodec extends Codec {
  filter(message: Message): boolean {
    return message.messageID === 'pwr_cal'
  }

  encode(payload: PowerCalibration) {
    // SmartBuffers automatically keep track of read and write offsets / cursors.
    const packet = new SmartBuffer()

    packet.writeInt16LE(payload.voltage)
    packet.writeInt16LE(payload.current_servo_1)
    packet.writeInt16LE(payload.current_servo_2)
    packet.writeInt16LE(payload.current_servo_3)
    packet.writeInt16LE(payload.current_servo_4)

    return packet.toBuffer()
  }

  decode(payload: Buffer) {
    const reader = SmartBuffer.fromBuffer(payload)

    return {
      voltage: reader.readInt16LE(),
      current_servo_1: reader.readInt16LE(),
      current_servo_2: reader.readInt16LE(),
      current_servo_3: reader.readInt16LE(),
      current_servo_4: reader.readInt16LE(),
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
  new PositionCodec(),
  new SupervisorInfoCodec(),
  new InboundMotionCodec(),
  new InboundFadeCodec(),
  new LEDCodec(),
  new RGBSettingsCodec(),
  new PowerCalibrationCodec(),
]
