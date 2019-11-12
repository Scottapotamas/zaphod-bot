import { Action, RunActionFunction } from '@electricui/core-actions'
import { Device, DeviceManager, Message } from '@electricui/core'

import { LightMove } from './../codecs'
import fs from 'fs'
import { getDelta } from './utils'
import { lightQueueSequencer } from './../sequence-senders'
import os from 'os'
import path from 'path'

const queueLight = new Action(
  'queue_light',
  async (
    deviceManager: DeviceManager,
    runAction: RunActionFunction,
    lightMove: any,
  ) => {
    lightQueueSequencer.queueItem(lightMove)
  },
)

const lightQueuePause = new Action(
  'light_queue_paused',
  async (
    deviceManager: DeviceManager,
    runAction: RunActionFunction,
    paused: boolean,
  ) => {
    lightQueueSequencer.setPaused(paused)
  },
)

const clearUILightQueue = new Action(
  'clear_ui_light_queue',
  async (
    deviceManager: DeviceManager,
    runAction: RunActionFunction,
    noop: boolean,
  ) => {
    lightQueueSequencer.clear()
  },
)

export { queueLight, lightQueuePause, clearUILightQueue }
