import { DeviceManager, Message, Device } from '@electricui/core'
import { Action, RunActionFunction } from '@electricui/core-actions'
import path from 'path'
import os from 'os'
import fs from 'fs'

import loadScene from './loadScene'
import { getDelta } from './utils'

import { MovementMove } from './../codecs'

const queueMovement = new Action(
  'queue_movement',
  async (
    deviceManager: DeviceManager,
    runAction: RunActionFunction,
    movementMove: MovementMove,
  ) => {
    const delta = getDelta(deviceManager)

    const message = new Message('inmv', movementMove)
    message.metadata.ack = true

    await delta.write(message)

    const commit = new Message('qumv', null)

    await delta.write(commit)
  },
)

const executeMovement = new Action(
  'execute_movement',
  async (
    deviceManager: DeviceManager,
    runAction: RunActionFunction,
    options: any,
  ) => {
    const delta = getDelta(deviceManager)

    const message = new Message('stmv', null)

    await delta.write(message)
  },
)

export { queueMovement, executeMovement }
