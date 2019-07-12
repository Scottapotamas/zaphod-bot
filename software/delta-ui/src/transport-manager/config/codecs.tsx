import { Codec, Message, PushCallback } from '@electricui/core'
import { SmartBuffer } from 'smart-buffer'
import { any } from 'prop-types'

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
      x: reader.readInt32LE() / 1000,
      y: reader.readInt32LE() / 1000,
      z: reader.readInt32LE() / 1000,

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
      mode: reader.readUInt8(),
    }

    return push(message)
  }
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
    packet.writeUInt8(message.payload.id)
    packet.writeUInt8(0x00) // padding
    packet.writeUInt16LE(message.payload.duration)
    packet.writeUInt16LE(message.payload.num_points)

    console.log('numpoints', message.payload.num_points)
    console.log('points', message.payload.points)

    for (let index = 0; index < 5; index++) {
      const pointData = message.payload.points[index]

      console.log(index, pointData)

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
    console.log(message.payload)
    return push(message)
  }

  decode(message: Message, push: PushCallback) {
    if (message.payload === null) {
      return push(message)
    }

    console.log('payload raw', message.payload)

    const reader = SmartBuffer.fromBuffer(message.payload)

    const typ = reader.readUInt8()
    const ref = reader.readUInt8()
    const id = reader.readUInt8()
    reader.readUInt8() // padding
    const dur = reader.readUInt16LE()
    const num = reader.readUInt16LE()

    const newPayload = {
      // _POINT_TRANSIT = 0,
      // _LINE,
      // _CATMULL_SPLINE,
      // _BEZIER_QUADRATIC,
      // _BEZIER_CUBIC
      type: typ,

      // _POS_ABSOLUTE = 0,
      // _POS_RELATIVE,
      reference: ref,

      // UUID for a given movement (used for tracking?)
      id: id,
      // 0x00 padding byte
      duration: dur, // in millis
      num_points: num,
      points: [],
    }

    for (let index = 0; index <= num; index++) {
      const point = [
        reader.readInt32LE() / 1000,
        reader.readInt32LE() / 1000,
        reader.readInt32LE() / 1000,
      ]

      newPayload.points.push(point)
    }
    message.payload = newPayload
    return push(message)
  }
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
      red: reader.readUInt8(),
      green: reader.readUInt8(),
      blue: reader.readUInt8(),
      enable: reader.readUInt8(),
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
  new RGBCodec(),
]
