import { DeviceManager, Message, Device } from '@electricui/core'
import { Action, RunActionFunction } from '@electricui/core-actions'
import path from 'path'
import os from 'os'
import fs from 'fs'

import { getDelta } from './utils'

import { movementQueueSequencer } from './../sequence-senders'

import {
  MovementMove,
  MovementMoveType,
  MovementMoveReference,
} from './../codecs'

// Simplified movements
async function writeMovement(delta: Device, movement: MovementMove) {
  const message = new Message('inmv', movement)
  message.metadata.ack = true

  await delta.write(message)
}

type SimpleMovementPayload = {
  id: number
  amount: number
  duration: number
}

const moveUp = new Action(
  'move_up',
  async (
    deviceManager: DeviceManager,
    runAction: RunActionFunction,
    payload: SimpleMovementPayload,
  ) => {
    const delta = getDelta(deviceManager)

    const movement: MovementMove = {
      type: MovementMoveType.LINE,
      reference: MovementMoveReference.RELATIVE,
      id: payload.id,
      duration: payload.duration,
      points: [[0, 0, 0], [0, 0, payload.amount]],
    }

    console.log('move_up', payload.amount)

    return writeMovement(delta, movement)
  },
)

const moveDown = new Action(
  'move_down',
  async (
    deviceManager: DeviceManager,
    runAction: RunActionFunction,
    payload: SimpleMovementPayload,
  ) => {
    const delta = getDelta(deviceManager)

    const movement: MovementMove = {
      type: MovementMoveType.LINE,
      reference: MovementMoveReference.RELATIVE,
      id: payload.id,
      duration: payload.duration,
      points: [[0, 0, 0], [0, 0, -payload.amount]],
    }

    console.log('move_down', payload.amount)

    return writeMovement(delta, movement)
  },
)
const moveLeft = new Action(
  'move_left',
  async (
    deviceManager: DeviceManager,
    runAction: RunActionFunction,
    payload: SimpleMovementPayload,
  ) => {
    const delta = getDelta(deviceManager)

    const movement: MovementMove = {
      type: MovementMoveType.LINE,
      reference: MovementMoveReference.RELATIVE,
      id: payload.id,
      duration: payload.duration,
      points: [[0, 0, 0], [payload.amount, 0, 0]],
    }

    console.log('move_left', payload.amount)

    return writeMovement(delta, movement)
  },
)
const moveRight = new Action(
  'move_right',
  async (
    deviceManager: DeviceManager,
    runAction: RunActionFunction,
    payload: SimpleMovementPayload,
  ) => {
    const delta = getDelta(deviceManager)

    const movement: MovementMove = {
      type: MovementMoveType.LINE,
      reference: MovementMoveReference.RELATIVE,
      id: payload.id,
      duration: payload.duration,
      points: [[0, 0, 0], [-payload.amount, 0, 0]],
    }

    console.log('move_right', payload.amount)

    return writeMovement(delta, movement)
  },
)
const moveForward = new Action(
  'move_forward',
  async (
    deviceManager: DeviceManager,
    runAction: RunActionFunction,
    payload: SimpleMovementPayload,
  ) => {
    const delta = getDelta(deviceManager)

    const movement: MovementMove = {
      type: MovementMoveType.LINE,
      reference: MovementMoveReference.RELATIVE,
      id: payload.id,
      duration: payload.duration,
      points: [[0, 0, 0], [0, payload.amount, 0]],
    }

    console.log('move_forward', payload.amount)

    return writeMovement(delta, movement)
  },
)
const moveBack = new Action(
  'move_back',
  async (
    deviceManager: DeviceManager,
    runAction: RunActionFunction,
    payload: SimpleMovementPayload,
  ) => {
    const delta = getDelta(deviceManager)

    const movement: MovementMove = {
      type: MovementMoveType.LINE,
      reference: MovementMoveReference.RELATIVE,
      id: payload.id,
      duration: payload.duration,
      points: [[0, 0, 0], [0, -payload.amount, 0]],
    }

    console.log('move_back', payload.amount)

    return writeMovement(delta, movement)
  },
)

const sync = new Action(
  'sync',
  async (
    deviceManager: DeviceManager,
    runAction: RunActionFunction,
    syncID: number,
  ) => {
    const delta = getDelta(deviceManager)

    const message = new Message('syncid', syncID)
    message.metadata.ack = true

    await delta.write(message)

    const commit = new Message('sync', null)

    await delta.write(commit)
  },
)

const queueMovement = new Action(
  'queue_movement',
  async (
    deviceManager: DeviceManager,
    runAction: RunActionFunction,
    movementMove: any,
  ) => {
    movementQueueSequencer.queueItem(movementMove)
  },
)

const movementQueuePause = new Action(
  'movement_queue_paused',
  async (
    deviceManager: DeviceManager,
    runAction: RunActionFunction,
    paused: boolean,
  ) => {
    movementQueueSequencer.setPaused(paused)
  },
)

export {
  queueMovement,
  movementQueuePause,
  moveUp,
  moveDown,
  moveLeft,
  moveRight,
  moveForward,
  moveBack,
  sync,
}
