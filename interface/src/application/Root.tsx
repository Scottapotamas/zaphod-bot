import './styles.css'

import { DarkModeWrapper, NoIPCModal } from '@electricui/components-desktop-blueprint'
import { LocationProvider, Router } from '@reach/router'

import { DarkModeChartThemeProvider, AcceptableDeviceContinuityProvider } from '@electricui/components-desktop-charts'
import { DeviceIDBridgeContext } from '@electricui/components-desktop-charts'
import { DeviceManagerProxy } from '@electricui/core-device-manager-proxy'
import React, { Suspense } from 'react'
import { RefreshIndicator } from '@electricui/components-desktop-blueprint'
import { WrapDeviceContextWithLocation } from './pages/WrapDeviceContextWithLocation'
import { history } from '@electricui/utility-electron'
import { StateProvider } from '@electricui/components-core'

import { FocusStyleManager } from '@blueprintjs/core'
FocusStyleManager.onlyShowFocusOnTabs()

import { DeviceLoadingPage } from './pages/DeviceLoadingPage'
import { DevicePages } from './pages/DevicePages'
import { ToolpathViewer } from './pages/toolpaths/interface/ToolpathViewer'
import { ConnectionPage } from './pages/ConnectionPage'

export function Root() {
  return (
    <RefreshIndicator>
      <StateProvider>
        <DeviceManagerProxy renderIfNoIPC={<NoIPCModal />}>
          <DarkModeWrapper>
            <DeviceIDBridgeContext>
              <DarkModeChartThemeProvider>
                <AcceptableDeviceContinuityProvider>
                  <LocationProvider history={history}>
                    <Suspense fallback={<div>Loading...</div>}>
                      <Router>
                        <ConnectionPage path="/" />
                        <ToolpathViewer path="/toolpaths/" />

                        <WrapDeviceContextWithLocation path="device_loading/:deviceID/">
                          <DeviceLoadingPage path="/" />
                        </WrapDeviceContextWithLocation>
                        <WrapDeviceContextWithLocation path="devices/:deviceID/">
                          <DevicePages path="*" />
                        </WrapDeviceContextWithLocation>
                      </Router>
                    </Suspense>
                  </LocationProvider>
                </AcceptableDeviceContinuityProvider>
              </DarkModeChartThemeProvider>
            </DeviceIDBridgeContext>
          </DarkModeWrapper>
        </DeviceManagerProxy>
      </StateProvider>
    </RefreshIndicator>
  )
}
