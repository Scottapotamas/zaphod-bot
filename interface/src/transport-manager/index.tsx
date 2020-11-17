import 'source-map-support/register'

import { Debug } from './pages/Debug'
import React from 'react'
import ReactDOM from 'react-dom'
import { deviceManager } from './config'
import { ipcRenderer } from 'electron'
import { setupProxyServer } from '@electricui/components-core'

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

    // Prepare the device manager proxy server for a hot reload
    const dataBundle = server.prepareForHotReload()

    console.log('Setting up new proxy server')

    // Setup the new proxy server
    server = setupProxyServer(deviceManager)
    server.setDataForHotReload(dataBundle)
  })
}

ipcRenderer.send('open-debug-window-dev-tools')
