import React from 'react'

import { IntervalRequester } from '@electricui/components-core'

import { SceneController } from './../SceneControl/SceneController'

const EventPalette = () => {
  return (
    <>
      <IntervalRequester interval={100} variables={['rgb']} />
      <h3>Load Event Sequence from File</h3>
      <SceneController />
    </>
  )
}

export default EventPalette
