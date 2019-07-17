import { DeviceManager, Message, Device } from '@electricui/core'
import { Action, RunActionFunction } from '@electricui/core-actions'
import path from 'path'
import os from 'os'
import fs from 'fs'

import loadScene from './loadScene'
import { getDelta } from './utils'

import { LightMove } from './../codecs'

const queueLight = new Action(
  'queue_light',
  async (
    deviceManager: DeviceManager,
    runAction: RunActionFunction,
    lightMove: LightMove,
  ) => {
    const delta = getDelta(deviceManager)

    const message = new Message('inlt', lightMove)
    message.metadata.ack = true

    await delta.write(message)

    const commit = new Message('qult', null)

    await delta.write(commit)
  },
)

const startLight = new Action(
  'start_light',
  async (
    deviceManager: DeviceManager,
    runAction: RunActionFunction,
    options: any,
  ) => {
    const delta = getDelta(deviceManager)

    // todo
  },
)

export { queueLight, startLight }
