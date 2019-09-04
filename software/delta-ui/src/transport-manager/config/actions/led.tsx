import { DeviceManager, Message, Device } from '@electricui/core'
import { Action, RunActionFunction } from '@electricui/core-actions'
import path from 'path'
import os from 'os'
import fs from 'fs'

import { lightQueueSequencer } from './../sequence-senders'

import { getDelta } from './utils'

import { LightMove } from './../codecs'

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

export { queueLight, lightQueuePause }
