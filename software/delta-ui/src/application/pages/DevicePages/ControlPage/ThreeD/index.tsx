import { Canvas, useFrame } from 'react-three-fiber'
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

type Vertices = [number, number, number]
type ViewerVertices = Array<Vertices>
interface CollectionForUI {
  name: string
  duration: number
  first_move: number
  last_move: number
  viewer_vertices: ViewerVertices
  viewer_uv: string
}

export const ThreeD = () => {
  const summaryFilePath = useDeviceMetadataKey('summary_file_path') as string
  const loadedCollectionData = useDeviceMetadataKey('loaded_collection_data') as Array<CollectionForUI> || [] // prettier-ignore

  const currentFrame = useCurrentFrame()

  if (!summaryFilePath) {
    return null
  }

  // Parse the file and find the collections to render out

  return <Renderer />
}
