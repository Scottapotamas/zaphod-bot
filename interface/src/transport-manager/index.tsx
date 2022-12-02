import 'source-map-support/register'

import { deviceManager } from './config'
import { setupProxyAndDebugInterface } from '@electricui/components-desktop-blueprint'
import { setupTransportWindow } from '@electricui/utility-electron'
import {
  ElectronIPCRemoteQueryExecutor,
  QueryableMessageIDProvider,
} from '@electricui/core-timeseries'

import './styles.css'

import { FocusStyleManager } from '@blueprintjs/core'
FocusStyleManager.onlyShowFocusOnTabs()

const root = document.createElement('div')
document.body.appendChild(root)

const hotReloadHandler = setupProxyAndDebugInterface(root, deviceManager)
setupTransportWindow()

const remoteQueryExecutor = new ElectronIPCRemoteQueryExecutor()
const queryableMessageIDProvider = new QueryableMessageIDProvider(
  deviceManager,
  remoteQueryExecutor,
)

if (module.hot) {
  module.hot.accept('./config', () => hotReloadHandler(root, deviceManager))
}
