import { Canvas, useFrame } from 'react-three-fiber'
import { Card, Divider, HTMLTable } from '@blueprintjs/core'
import { CollectionForUI, Vertices, ViewerVertices } from './types'
import {
  IntervalRequester,
  StateTree,
  useDeviceMetadataKey,
  useHardwareState,
} from '@electricui/components-core'
import React, { useRef } from 'react'

import { Renderer } from './Renderer'

const useCurrentFrame = () => {
  return useDeviceMetadataKey('current_frame') as number
}

export const ThreeD = () => {
  const summaryFilePath = useDeviceMetadataKey('summary_file_path') as string
  const loadedCollectionData = useDeviceMetadataKey('loaded_collection_data') as Array<CollectionForUI> || [] // prettier-ignore

  const currentFrame = useCurrentFrame()

  const headPosition = useHardwareState<[number, number, number]>(
    state => state.cpos,
  ) || [0, 0, 0]

  // Do our scaling and reference frame conversion
  const headPositionCm: [number, number, number] = [
    -headPosition[0] / 1000,
    headPosition[2] / 1000,
    headPosition[1] / 1000,
  ]

  if (!summaryFilePath) {
    return null
  }

  // Parse the file and find the collections to render out

  return (
    <Card style={{ width: '100%', height: '100%' }}>
      <Renderer
        loadedCollectionData={loadedCollectionData}
        summaryFilePath={summaryFilePath}
        currentFrame={currentFrame}
        headPosition={headPositionCm}
      />
    </Card>
  )
}
