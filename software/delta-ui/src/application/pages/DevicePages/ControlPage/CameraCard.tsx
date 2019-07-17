import React from 'react'
import {
  useDeviceIDByMetadata,
  DeviceIDContextProvider,
  useDeviceMetadataKey,
} from '@electricui/components-core'

import { Printer } from '@electricui/components-desktop'

import { Button } from '@electricui/components-desktop-blueprint'

import { NonIdealState, Card } from '@blueprintjs/core'

import path from 'path'

const CameraSubPage = () => {
  const model = useDeviceMetadataKey('model')
  const localSavePath = useDeviceMetadataKey('localSavePath')

  let captureButton

  if (typeof localSavePath === 'string' && localSavePath !== '') {
    captureButton = (
      <Button
        writer={{
          __capture: {
            savePath: path.join(localSavePath, './capture.cr2'),
          },
        }}
        disabled={localSavePath !== ''}
      >
        Capture
      </Button>
    )
  }

  return (
    <Card style={{ marginBottom: 10 }}>
      <p>I'm in a camera context! look at me, it's model is {model}</p>
      <p>
        ISO: <Printer accessor="/imgsettings/iso" />
      </p>

      <p>{captureButton}</p>

      <p>
        <Button writer={{ '/imgsettings/iso': '100' }}>100</Button>
      </p>
      <p>
        <Button writer={{ '/imgsettings/iso': '200' }}>200</Button>
      </p>
    </Card>
  )
}

const CameraCard = () => {
  const potentialDeviceID = useDeviceIDByMetadata({ type: 'Camera' })

  if (potentialDeviceID === null) {
    return (
      <NonIdealState
        title="No camera found"
        description="A camera was not detected"
      />
    )
  }

  return (
    <DeviceIDContextProvider deviceID={potentialDeviceID}>
      <CameraSubPage />
    </DeviceIDContextProvider>
  )
}

export default CameraCard
