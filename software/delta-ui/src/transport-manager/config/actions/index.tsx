import { Action, RunActionFunction } from '@electricui/core-actions'
import { Device, DeviceManager, Message } from '@electricui/core'
import { lightQueuePause, queueLight } from './led'
import {
  moveBack,
  moveDown,
  moveForward,
  moveLeft,
  moveRight,
  moveUp,
  movementQueuePause,
  queueMovement,
  sync,
} from './delta'
import {
  openScene,
  renderCollection,
  renderFrame,
  setFrame,
  setSelectedCollections,
  startSceneExecution,
  stopSceneExecution,
} from './sceneControl'

import { loadCollection } from './loadCollection'

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
  loadCollection,
  wait,
  setFrame,
  queueLight,
  lightQueuePause,
  queueMovement,
  movementQueuePause,
  startSceneExecution,
  stopSceneExecution,
  moveUp,
  moveDown,
  moveLeft,
  moveRight,
  moveForward,
  moveBack,
  sync,
  openScene,
  setSelectedCollections,
  renderCollection,
  renderFrame,
]

export default actions
