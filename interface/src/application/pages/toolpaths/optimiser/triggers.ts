import { CancellationToken, DeviceID, DeviceManagerPublicInterface, Message } from '@electricui/core'
import { AputureLS, MODE_SELECT, FXMODE, FXLIGHTINGMODE, FXEXPLOSIONMODE } from '../../../../application/typedState'

export enum TriggerType {
  DMX = 'dmx',
}

export enum TriggerAlignment {
  START = 'start',
  MIDDLE = 'middle',
  END = 'end',
}

export const Triggers: {
  [key: string]: TriggerCall
} = {}

export interface TriggerCallGeneric {
  type: TriggerType
  align: TriggerAlignment
  args: {
    [key: string]: any
  }
}

export interface TriggerCallDMX extends TriggerCallGeneric {
  type: TriggerType.DMX
  align: TriggerAlignment
  args: {
    level: number
  }
}

export type TriggerCall = TriggerCallDMX | TriggerCallDMX

export interface TimedTrigger extends TriggerCallGeneric {
  timestamp: number
}

export function callTrigger(
  triggerCall: TriggerCallGeneric,
  deviceManager: DeviceManagerPublicInterface,
  deltaDeviceID: DeviceID | null,
  dmxDeviceID: DeviceID | null,
) {
  const cancellationToken = new CancellationToken('trigger')

  switch (triggerCall.type) {
    case TriggerType.DMX:
      {
        const args: TriggerCallDMX['args'] = (triggerCall as TriggerCallDMX).args

        const intensity = Math.min(Math.max((args.level / 100) * 255, 0), 255)

        const payload: AputureLS = {
          intensity: intensity, // 0 - 255, direct from blender
          mode: MODE_SELECT.MODE_MANUAL,
          fxSelect: FXMODE.FX_UNDEFINED,
          fxFrequency: 0,
          fxTrigger: 0,
        }

        if (!dmxDeviceID) {
          console.log(`would trigger DMX event`, payload, `but there doesn't appear to be one connected`)
          return
        }

        const message = new Message('fixture', payload)
        message.deviceID = dmxDeviceID

        deviceManager.write(message, cancellationToken).catch(err => {
          console.error("Error triggering DMX event", err)
        })
      }

      return
    default:
      break
  }
}
