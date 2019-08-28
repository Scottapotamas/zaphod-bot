import React, { Component } from 'react'

import {
  ConnectionMetadata,
  CONNECTION_STATE,
  DeviceMetadata,
  Message,
  HANDSHAKE_STATES,
} from '@electricui/core'

import {
  withDeviceManager,
  InjectedDeviceManagerProps,
  UI_EVENTS,
} from '@electricui/components-core'

import { Toaster as BlueprintToaster, Position } from '@blueprintjs/core'

export const Toaster = BlueprintToaster.create({
  className: 'error-toaster',
  position: Position.TOP,
})

class BreadFactoryWithoutDeviceManager extends Component<
  InjectedDeviceManagerProps
> {
  errorCounter: Map<string, number> = new Map()
  errorTimer: Map<string, number> = new Map()

  componentDidMount() {
    const { deviceManager } = this.props

    deviceManager.on(UI_EVENTS.COMMITTED, this.receiveMessage) // prettier-ignore

    console.log()
  }

  componentWillUnmount() {
    const { deviceManager } = this.props

    deviceManager.removeListener(UI_EVENTS.COMMITTED, this.receiveMessage) // prettier-ignore
  }

  receiveMessage = (message: Message) => {
    if (message.metadata.internal) return

    if (message.deviceID === null) {
      console.warn('Found a message with a null deviceID', message)
      return
    }

    if (message.messageID === 'err') {
      if (
        this.errorCounter.has(message.payload) &&
        this.errorTimer.get(message.payload)! > new Date().getTime()
      ) {
        const counter = this.errorCounter.get(message.payload)! + 1

        this.errorCounter.set(message.payload, counter)
        this.errorTimer.set(message.payload, new Date().getTime() + 10000)

        Toaster.show(
          { message: `${message.payload} (${counter})`, intent: 'warning' },
          message.payload,
        )
      } else {
        Toaster.show(
          { message: message.payload, intent: 'warning' },
          message.payload,
        )
        this.errorCounter.set(message.payload, 0)
        this.errorTimer.set(message.payload, new Date().getTime() + 10000)
      }
    }
  }

  render() {
    // we never render anything
    return null
  }
}

export const BreadFactory = withDeviceManager(BreadFactoryWithoutDeviceManager)
