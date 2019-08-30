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
      feedback: reader.readUInt8(),
      _: reader.readUInt8(),
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
      move_state: reader.readUInt8(),
      move_id: reader.readUInt8(),
      move_type: reader.readUInt8(),
      move_progress: reader.readUInt8(),

      queue_state: reader.readUInt8(),
      queue_depth: reader.readUInt8(),
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
  queue_depth: number
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

    packet.writeUInt8(message.payload.red)
    packet.writeUInt8(message.payload.green)
    packet.writeUInt8(message.payload.blue)
    packet.writeUInt8(message.payload.enable ? 1 : 0)
    packet.writeUInt8(message.payload.queue_depth)

    message.payload = packet.toBuffer()

    return push(message)
  }

  decode(message: Message, push: PushCallback) {
    if (message.payload === null) {
      return push(message)
    }

    const reader = SmartBuffer.fromBuffer(message.payload)
    message.payload = {
      red: reader.readUInt8(),
      green: reader.readUInt8(),
      blue: reader.readUInt8(),
      enable: reader.readUInt8() === 1 ? true : false,
      queue_depth: reader.readUInt8(),
    }

    return push(message)
  }
}

export const customCodecs = [
  new SystemDataCodec(),
  new TempDataCodec(),
  new FanCodec(),
  new TargetPositionCodec(),
  new FirmwareBuildInfoCodec(),
  new MotorDataCodec(),
  new MotionDataCodec(),
  new SystemStateInfoCodec(),
  new InboundMotionCodec(),
  new InboundFadeCodec(),
  new RGBCodec(),
]
