import React from 'react'
import { Provider } from 'react-redux'
import { Store } from 'redux'
import { Button } from '@blueprintjs/core'

import { DeviceManagerProxy } from '@electricui/components-core'
import { ReactReduxContext } from '@electricui/core-redux-state'
import {
  DeviceManagerStatusModal,
  DarkModeWrapper,
} from '@electricui/components-desktop-blueprint'
import { DarkModeProvider } from '@electricui/components-desktop'

import { LocationProvider, Router } from '@reach/router'
import { history } from '@electricui/utility-electron'

import ConnectionPage from './pages/ConnectionPage'
import DevicePages from './pages/DevicePages'
import DeviceLoadingPage from './pages/DeviceLoadingPage'
import { TimeSeriesDataStore } from '@electricui/core-timeseries'
import { sourceFactory, timeseriesFactories } from './datasources'

import WrapDeviceContextWithLocation from './pages/WrapDeviceContextWithLocation'

interface RootProps {
  store: Store
}

interface PotentialErrorState {
  hasError: boolean
}

class Wrap extends React.Component<{}, PotentialErrorState> {
  constructor(props: any) {
    super(props)
    this.state = { hasError: false }
  }

  static getDerivedStateFromError(error: Error) {
    // Update state so the next render will show the fallback UI.
    console.error(error)
    return { hasError: true }
  }

  componentDidCatch(error: Error, info: any) {
    // You can also log the error to an error reporting service
    console.log('Caught error', error, info)
  }

  render() {
    const { children } = this.props

    if (this.state.hasError) {
      // You can render any custom fallback UI
      return <div>Something went wrong</div>
    }

    return children
  }
}

export default class Root extends React.Component<RootProps> {
  render() {
    const { store } = this.props

    return (
      <Wrap>
        <Provider store={store} context={ReactReduxContext}>
          <DeviceManagerProxy>
            <TimeSeriesDataStore
              sourceFactory={sourceFactory}
              timeseriesFactories={timeseriesFactories}
              duration={30 * 1000}
              maxItems={10000}
            >
              <DarkModeProvider>
                <DarkModeWrapper>
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
                </DarkModeWrapper>
              </DarkModeProvider>
            </TimeSeriesDataStore>
          </DeviceManagerProxy>
        </Provider>
      </Wrap>
    )
  }
}
