import { Codec, Message } from '@electricui/core'

import {
  AputureLS
} from '../../application/typedState'
import { SmartBuffer } from 'smart-buffer'

export class AputureFixtureCodec extends Codec {
  filter(message: Message): boolean {
    return message.messageID === 'fixture'
  }

  encode(payload: AputureLS[]): Buffer {
    const packet = new SmartBuffer()

    for (let fixture of payload) {
      packet.writeUInt8(fixture.intensity)
      packet.writeUInt8(fixture.mode)
      packet.writeUInt8(fixture.fxSelect)
      packet.writeUInt8(fixture.fxFrequency)
      packet.writeUInt8(fixture.fxTrigger)
    }

    return packet.toBuffer()
  }

  decode(payload: Buffer): AputureLS[] {
    const reader = SmartBuffer.fromBuffer(payload)

    const lightFixtures: AputureLS[] = []

    while (reader.remaining() > 0) {
      const settings: AputureLS = {
        intensity: reader.readUInt8(),
        mode: reader.readUInt8(),
        fxSelect: reader.readUInt8(),
        fxFrequency: reader.readUInt8(),
        fxTrigger: reader.readUInt8(),
      }
      lightFixtures.push(settings)
    }

    return lightFixtures
  }
}

export const dmxCodecs = [
  new AputureFixtureCodec(),
]
