import './styles.css'

import {
  DarkModeWrapper,
  NoIPCModal,
} from '@electricui/components-desktop-blueprint'
import { LocationProvider, Router } from '@reach/router'

import React from 'react'
import { ConnectionPage } from './pages/ConnectionPage'
import { DarkModeChartThemeProvider } from '@electricui/components-desktop-charts'
import { DarkModeProvider } from '@electricui/components-desktop'
import { DeviceIDBridgeContext } from '@electricui/components-desktop-charts'
import { DeviceLoadingPage } from './pages/DeviceLoadingPage'
import { DeviceManagerProxy } from '@electricui/components-core'
import { DevicePages } from './pages/DevicePages'
import { Provider } from 'react-redux'
import { ReactReduxContext } from '@electricui/core-redux-state'
import { RefreshIndicator } from '@electricui/components-desktop-blueprint'
import { Store } from 'redux'
import { TimeSeriesProvider } from '@electricui/core-timeseries'
import { WrapDeviceContextWithLocation } from './pages/WrapDeviceContextWithLocation'
import { history } from '@electricui/utility-electron'

interface RootProps {
  store: Store
}

export class Root extends React.Component<RootProps> {
  render() {
    const { store } = this.props

    return (
      <RefreshIndicator>
        <Provider store={store} context={ReactReduxContext}>
          <DeviceManagerProxy renderIfNoIPC={<NoIPCModal />}>
            <DarkModeProvider>
              <DarkModeWrapper>
                <TimeSeriesProvider>
                  <DeviceIDBridgeContext>
                    <DarkModeChartThemeProvider>
                      <LocationProvider history={history}>
                        <Router>
                          <ConnectionPage path="/" />
                          <WrapDeviceContextWithLocation path="device_loading/:deviceID/">
                            <DeviceLoadingPage path="/" />
                          </WrapDeviceContextWithLocation>
                          <WrapDeviceContextWithLocation path="devices/:deviceID/">
                            <DevicePages path="*" />
                          </WrapDeviceContextWithLocation>
                        </Router>
                      </LocationProvider>
                    </DarkModeChartThemeProvider>
                  </DeviceIDBridgeContext>
                </TimeSeriesProvider>
              </DarkModeWrapper>
            </DarkModeProvider>
          </DeviceManagerProxy>
        </Provider>
      </RefreshIndicator>
    )
  }
}
