require('@electricui/helpers')

import { ipcRenderer } from 'electron'
import React from 'react'
import ReactDOM from 'react-dom'

import { setupProxyServer } from '@electricui/components-core'

import { deviceManager } from './config'

import { Debug } from './pages/Debug'

const root = document.createElement('div')
document.body.appendChild(root)

// TODO: Figure out why the webpack env isn't taking
declare const module: any

let server = setupProxyServer(deviceManager)

ReactDOM.render(<Debug />, root)

if (module.hot) {
  module.hot.accept('./pages/Debug', () => {
    const NextDebug = require('./pages/Debug').default
    ReactDOM.render(<NextDebug />, root)
  })

  module.hot.accept('./config', () => {
    console.log('Hot reloading device manager configuration...')
    console.log('Tearing down old proxy server')

    // tear down IPC
    server.teardown()

    // Remove all listeners from the old device manager
    deviceManager.removeAllListeners()

    console.log('Setting up new proxy server')

    // Setup the new proxy server
    server = setupProxyServer(deviceManager)
  })
}

ipcRenderer.send('open-debug-window-dev-tools')
