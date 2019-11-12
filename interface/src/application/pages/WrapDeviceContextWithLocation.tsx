import { Button } from '@blueprintjs/core'
import { DeviceIDContextProvider } from '@electricui/components-core'
import React from 'react'
import { RouteComponentProps } from '@reach/router'
import { navigate } from '@electricui/utility-electron'

interface InjectDeviceIDFromLocation {
  deviceID?: string
}
interface PotentialErrorState {
  hasError: boolean
}

class WrapDeviceContextWithLocation extends React.Component<
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
        <div>
          Something went wrong, go back?
          <Button
            onClick={() => {
              navigate('/')
            }}
          >
            Back
          </Button>
        </div>
      )
    }

    return (
      <DeviceIDContextProvider deviceID={deviceID!}>
        {children}
      </DeviceIDContextProvider>
    )
  }
}

export default WrapDeviceContextWithLocation
