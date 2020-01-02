import { Codec, Message, PushCallback } from '@electricui/core'

import { SmartBuffer } from 'smart-buffer'

export class SystemDataCodec extends Codec {
  filter(message: Message): boolean {
    return message.messageID === 'sys'
  }

  encode(message: Message, push: PushCallback) {
    if (message.payload === null) {
      return push(message)
    }

    return push(message)
  }

  decode(message: Message, push: PushCallback) {
    if (message.payload === null) {
      return push(message)
    }

    const reader = SmartBuffer.fromBuffer(message.payload)
    message.payload = {
      sensors_enable: reader.readUInt8(),
      module_enable: reader.readUInt8(),
      cpu_load: reader.readUInt8(),
      cpu_clock: reader.readUInt8(),
      cpu_temp: reader.readFloatLE(),
      input_voltage: reader.readFloatLE(),
    }

    return push(message)
  }
}

export class TempDataCodec extends Codec {
  filter(message: Message): boolean {
    return message.messageID === 'temp'
  }

  decode(message: Message, push: PushCallback) {
    if (message.payload === null) {
      return push(message)
    }

    const reader = SmartBuffer.fromBuffer(message.payload)
    message.payload = {
      ambient: reader.readFloatLE(),
      regulator: reader.readFloatLE(),
      supply: reader.readFloatLE(),
    }

    return push(message)
  }
}

export class FanCodec extends Codec {
  filter(message: Message): boolean {
    return message.messageID === 'fan'
  }

  decode(message: Message, push: PushCallback) {
    if (message.payload === null) {
      return push(message)
    }

    const reader = SmartBuffer.fromBuffer(message.payload)
    message.payload = {
      rpm: reader.readUInt16LE(),
      setpoint: reader.readUInt8(),
      state: reader.readUInt8(),
    }

    return push(message)
  }
}

export class QueueDepthCodec extends Codec {
  filter(message: Message): boolean {
    return message.messageID === 'queue'
  }

  decode(message: Message, push: PushCallback) {
    if (message.payload === null) {
      return push(message)
    }

    const reader = SmartBuffer.fromBuffer(message.payload)
    message.payload = {
      movements: reader.readUInt8(),
      lighting: reader.readUInt8(),
    }

    return push(message)
  }
}

export class TargetPositionCodec extends Codec {
  filter(message: Message): boolean {
    return message.messageID === 'tpos'
  }

  encode(message: Message, push: PushCallback) {
    if (message.payload === null) {
      return push(message)
    }
    const packet = new SmartBuffer()

    packet.writeInt32LE(message.payload.x * 1000)
    packet.writeInt32LE(message.payload.y * 1000)
    packet.writeInt32LE(message.payload.z * 1000)

    message.payload = packet.toBuffer()

    return push(message)
  }

  decode(message: Message, push: PushCallback) {
    if (message.payload === null) {
      return push(message)
    }

    const reader = SmartBuffer.fromBuffer(message.payload)
    message.payload = {
      x: reader.readInt32LE() / 1000,
      y: reader.readInt32LE() / 1000,
      z: reader.readInt32LE() / 1000,
    }

    return push(message)
  }
}

export class FirmwareBuildInfoCodec extends Codec {
  filter(message: Message): boolean {
    return message.messageID === 'fwb'
  }

  decode(message: Message, push: PushCallback) {
    if (message.payload === null) {
      return push(message)
    }

    const reader = SmartBuffer.fromBuffer(message.payload)
    message.payload = {
      branch: 'Master',
      info: 'bajasdkjaskdhj',
      date: 'today, or something',
      time: 'just then',
      type: 'DEBUG',
      version: '0.2.6',
    }

    return push(message)
  }
}

export class MotorDataCodec extends Codec {
  filter(message: Message): boolean {
    return (
      message.messageID === 'mo1' ||
      message.messageID === 'mo2' ||
      message.messageID === 'mo3'
    )
  }

  decode(message: Message, push: PushCallback) {
    if (message.payload === null) {
      return push(message)
    }

    const reader = SmartBuffer.fromBuffer(message.payload)
    message.payload = {
      enabled: reader.readUInt8() === 0x01 ? true : false,
      state: reader.readUInt8(),
      feedback: reader.readUInt16LE() / 10,
      target_angle: reader.readFloatLE(),
      power: reader.readFloatLE(),
    }

    return push(message)
  }
}

export class MotionDataCodec extends Codec {
  filter(message: Message): boolean {
    return message.messageID === 'moStat'
  }

  decode(message: Message, push: PushCallback) {
    if (message.payload === null) {
      return push(message)
    }

    const reader = SmartBuffer.fromBuffer(message.payload)
    message.payload = {
      pathing_state: reader.readUInt8(),
      motion_state: reader.readUInt8(), // this isn't called the queue state on embedded
      profile_type: reader.readUInt8(),
      move_progress: reader.readUInt8(),

      movement_identifier: reader.readUInt16LE(),
    }

    return push(message)
  }
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

export class SystemStateInfoCodec extends Codec {
  filter(message: Message): boolean {
    return message.messageID === 'super'
  }

  decode(message: Message, push: PushCallback) {
    if (message.payload === null) {
      return push(message)
    }

    const reader = SmartBuffer.fromBuffer(message.payload)
    message.payload = {
      supervisor: SUPERVISOR_STATES[reader.readUInt8()] || 'UNKNOWN',
      motors: reader.readUInt8(),
      mode: CONTROL_MODES[reader.readUInt8()] || 'UNKNOWN',
    }

    return push(message)
  }
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

export type MovementMove = {
  id: number
  duration: number
  type: MovementMoveType
  reference: MovementMoveReference
  points: Array<MovementPoint>
  num_points?: number
}

/**
 * There's the possibility that the message payload is not being created each time
 */
export class InboundMotionCodec extends Codec {
  filter(message: Message): boolean {
    return message.messageID === 'inmv'
  }

  encode(message: Message, push: PushCallback) {
    if (message.payload === null) {
      return push(message)
    }
    const packet = new SmartBuffer()

    message.payload.num_points = message.payload.points.length

    packet.writeUInt8(message.payload.type)
    packet.writeUInt8(message.payload.reference)
    packet.writeUInt16LE(message.payload.id)
    packet.writeUInt16LE(message.payload.duration)
    packet.writeUInt16LE(message.payload.num_points)

    for (let index = 0; index < 4; index++) {
      const pointData = message.payload.points[index]

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

    message.payload = packet.toBuffer()
    return push(message)
  }
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

export class InboundFadeCodec extends Codec {
  filter(message: Message): boolean {
    return message.messageID === 'inlt'
  }

  encode(message: Message, push: PushCallback) {
    if (message.payload === null) {
      return push(message)
    }
    const packet = new SmartBuffer()

    message.payload.num_points = message.payload.points.length

    packet.writeUInt16LE(message.payload.id)
    packet.writeUInt16LE(message.payload.duration)
    packet.writeUInt8(message.payload.type)
    packet.writeUInt8(message.payload.num_points)
    packet.writeUInt8(0x00)
    packet.writeUInt8(0x00)

    for (let index = 0; index < 2; index++) {
      const pointData = message.payload.points[index]

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

    message.payload = packet.toBuffer()
    return push(message)
  }
}

export type LedStatus = {
  red: number
  green: number
  blue: number
  enable: boolean
}

export class RGBCodec extends Codec {
  filter(message: Message): boolean {
    return message.messageID === 'rgb'
  }

  encode(message: Message, push: PushCallback) {
    if (message.payload === null) {
      return push(message)
    }
    const packet = new SmartBuffer()

    packet.writeUInt16LE(message.payload.red)
    packet.writeUInt16LE(message.payload.green)
    packet.writeUInt16LE(message.payload.blue)
    packet.writeUInt8(message.payload.enable ? 1 : 0)

    message.payload = packet.toBuffer()

    return push(message)
  }

  decode(message: Message, push: PushCallback) {
    if (message.payload === null) {
      return push(message)
    }

    const reader = SmartBuffer.fromBuffer(message.payload)
    message.payload = {
      red: reader.readUInt16LE(),
      green: reader.readUInt16LE(),
      blue: reader.readUInt16LE(),
      enable: reader.readUInt8() === 1 ? true : false,
    }

    return push(message)
  }
}

export class RGBManualControl extends Codec {
  filter(message: Message): boolean {
    return message.messageID === 'hsv'
  }

  encode(message: Message, push: PushCallback) {
    if (message.payload === null) {
      return push(message)
    }
    const packet = new SmartBuffer()

    packet.writeFloatLE(message.payload.hue)
    packet.writeFloatLE(message.payload.saturation)
    packet.writeFloatLE(message.payload.lightness)
    packet.writeUInt8(message.payload.enable)
    message.payload = packet.toBuffer()

    return push(message)
  }

  decode(message: Message, push: PushCallback) {
    if (message.payload === null) {
      return push(message)
    }

    const reader = SmartBuffer.fromBuffer(message.payload)
    message.payload = {
      hue: reader.readFloatLE(),
      saturation: reader.readFloatLE(),
      lightness: reader.readFloatLE(),
      enable: reader.readInt8(),
    }

    return push(message)
  }
}

export class RGBSettingsCodec extends Codec {
  filter(message: Message): boolean {
    return message.messageID === 'ledset'
  }

  encode(message: Message, push: PushCallback) {
    if (message.payload === null) {
      return push(message)
    }
    const packet = new SmartBuffer()

    packet.writeInt16LE(message.payload.offset_red * 32767)
    packet.writeInt16LE(message.payload.offset_green * 32767)
    packet.writeInt16LE(message.payload.offset_blue * 32767)
    packet.writeInt16LE(message.payload.offset_global * 32767)

    message.payload = packet.toBuffer()

    return push(message)
  }

  decode(message: Message, push: PushCallback) {
    if (message.payload === null) {
      return push(message)
    }

    const reader = SmartBuffer.fromBuffer(message.payload)
    message.payload = {
      offset_red: reader.readInt16LE() / 32767,
      offset_green: reader.readInt16LE() / 32767,
      offset_blue: reader.readInt16LE() / 32767,
      offset_global: reader.readInt16LE() / 32767,
    }

    return push(message)
  }
}

export class KinematicsInfoCodec extends Codec {
  filter(message: Message): boolean {
    return message.messageID === 'kinematics'
  }

  decode(message: Message, push: PushCallback) {
    if (message.payload === null) {
      return push(message)
    }

    const reader = SmartBuffer.fromBuffer(message.payload)
    message.payload = {
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

    return push(message)
  }
}

export const customCodecs = [
  new SystemDataCodec(),
  new TempDataCodec(),
  new FanCodec(),
  new QueueDepthCodec(),
  new TargetPositionCodec(),
  new FirmwareBuildInfoCodec(),
  new MotorDataCodec(),
  new MotionDataCodec(),
  new SystemStateInfoCodec(),
  new InboundMotionCodec(),
  new InboundFadeCodec(),
  new RGBCodec(),
  new RGBManualControl(),
  new RGBSettingsCodec(),
  new KinematicsInfoCodec(),
]
