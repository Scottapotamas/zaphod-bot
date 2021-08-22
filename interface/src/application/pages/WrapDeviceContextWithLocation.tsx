import React from 'react'
import { Button } from '@blueprintjs/core'
import { RouteComponentProps } from '@reach/router'
import { navigate } from '@electricui/utility-electron'
import { TimeSeriesProvider } from '@electricui/core-timeseries'
import { DeviceIDContextProvider } from '@electricui/components-core'
import { NonIdealState } from '@blueprintjs/core'
import { DeviceID } from '@electricui/core'

interface InjectDeviceIDFromLocation {
  deviceID?: DeviceID
}
interface PotentialErrorState {
  hasError: boolean
}

export class WrapDeviceContextWithLocation extends React.Component<
  RouteComponentProps & InjectDeviceIDFromLocation,
  PotentialErrorState
> {
  constructor(props: RouteComponentProps & InjectDeviceIDFromLocation) {
    super(props)
    this.state = { hasError: false }
  }

  static getDerivedStateFromError(error: Error) {
    // Update state so the next render will show the fallback UI.
    return { hasError: true }
  }

  componentDidCatch(error: Error, info: any) {
    // You can also log the error to an error reporting service
    console.log('Caught error', error, info)
  }

  render() {
    const { children, deviceID } = this.props

    if (this.state.hasError) {
      // You can render any custom fallback UI
      return (
        <div style={{ height: '100vh' }}>
          <NonIdealState
            icon="error"
            title="An error occurred"
            description={
              <React.Fragment>
                <p>Something went wrong, go back to the connections page?</p>

                <br />
                <Button
                  onClick={() => {
                    navigate('/')
                  }}
                  intent="primary"
                >
                  Go Back
                </Button>
              </React.Fragment>
            }
          />
        </div>
      )
    }

    return (
      <DeviceIDContextProvider deviceID={deviceID!}>
        <TimeSeriesProvider deviceID={deviceID!}>{children}</TimeSeriesProvider>
      </DeviceIDContextProvider>
    )
  }
}
