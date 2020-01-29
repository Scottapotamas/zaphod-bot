import React from 'react'

import {
  useDeviceMetadataKey,
  IntervalRequester,
} from '@electricui/components-core'

import { SceneController } from './SceneControl/SceneController'

const EventPalette = () => {
  const sceneFilePath = useDeviceMetadataKey('summary_file_path')

  return (
    <>
      <IntervalRequester interval={100} variables={['rgb']} />
      <h3>Load Event Sequence from File</h3>
      <SceneController key={sceneFilePath} />
    </>
  )
}

export default EventPalette
