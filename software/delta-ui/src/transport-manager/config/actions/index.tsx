import { DeviceManager, Message, Device } from '@electricui/core'
import { Action, RunActionFunction } from '@electricui/core-actions'
import path from 'path'
import os from 'os'
import fs from 'fs'

import { loadScene, setLoadedScene } from './loadScene'

import { queueLight, lightQueuePause } from './led'
import {
  queueMovement,
  moveUp,
  movementQueuePause,
  moveDown,
  moveLeft,
  moveRight,
  moveForward,
  moveBack,
  sync,
} from './delta'
import { cameraSettings, cameraTrigger, setLocalSavePath } from './camera'

export type WaitOptions = number

const wait = new Action(
  'wait',
  async (
    deviceManager: DeviceManager,
    runAction: RunActionFunction,
    options: WaitOptions,
  ) => {
    const waitTime = options
    await new Promise((resolve, reject) => setTimeout(resolve, waitTime))
  },
)

const actions = [
  setLocalSavePath,
  loadScene,
  setLoadedScene,
  cameraSettings,
  cameraTrigger,
  wait,
  queueLight,
  lightQueuePause,
  queueMovement,
  movementQueuePause,
  moveUp,
  moveDown,
  moveLeft,
  moveRight,
  moveForward,
  moveBack,
  sync,
]

export default actions
