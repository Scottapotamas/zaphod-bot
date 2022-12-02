import 'source-map-support/register'

import React from 'react'
import ReactDOM from 'react-dom'
import { Root } from './Root'
import { setupDarkModeListenersRenderer } from '@electricui/utility-electron'

setupDarkModeListenersRenderer()

const root = document.createElement('div')
root.className = 'root'
document.body.appendChild(root)

function render(Component: React.FC) {
  ReactDOM.render(<Component />, root)
}

render(Root)
