import 'source-map-support/register'

import React from 'react'
import ReactDOM from 'react-dom'
import { Root } from './Root'
import { configureStore } from './state'
import { setupDarkModeListenersRenderer } from '@electricui/utility-electron'

setupDarkModeListenersRenderer()

let root = document.createElement('div')
root.className = 'root'
document.body.appendChild(root)

const store = configureStore()

function render(Component: any) {
  ReactDOM.render(<Component store={store} />, root)
}

render(Root)
