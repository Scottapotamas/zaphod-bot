import { DeviceManager, Message, Device } from '@electricui/core'
import { Action, RunActionFunction } from '@electricui/core-actions'
import path from 'path'
import os from 'os'
import fs from 'fs'

import loadScene from './loadScene'

import { queueLight } from './led'
import { executeMovement, queueMovement } from './delta'
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
  cameraSettings,
  cameraTrigger,
  wait,
  queueLight,
  queueMovement,
  executeMovement,
]

export default actions
