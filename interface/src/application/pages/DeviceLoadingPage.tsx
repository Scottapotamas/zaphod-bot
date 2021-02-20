import { DeviceLoadingScreen } from '@electricui/components-desktop-blueprint'
import { RouteComponentProps } from '@reach/router'
import React from 'react'

export const DeviceLoadingPage = (props: RouteComponentProps) => {
  return (
    <div style={{ height: '100vh' }}>
      <DeviceLoadingScreen />
    </div>
  )
}
