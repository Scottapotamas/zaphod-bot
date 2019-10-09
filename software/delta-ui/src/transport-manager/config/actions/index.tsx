import { Action, RunActionFunction } from '@electricui/core-actions'
import { Device, DeviceManager, Message } from '@electricui/core'
import { lightQueuePause, queueLight } from './led'
import { loadScene, setLoadedScene } from './loadScene'
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
// Only one that matters now
import {
  openScene,
  setFrame,
  setSelectedCollections,
  startSceneExecution,
  stopSceneExecution,
} from './scene_control'

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
  loadScene,
  setLoadedScene,
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
]

export default actions
