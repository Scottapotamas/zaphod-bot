import { Action, RunActionFunction } from '@electricui/core-actions'
import { Device, DeviceManager, Message } from '@electricui/core'

import fs from 'fs'
import { getDelta } from './utils'
import path from 'path'

const Datauri = require('datauri')

export type LoadSceneOptions = {
  filePath: string
}

type Vertices = [number, number, number]
type ViewerVertices = Array<Vertices>

interface Collection {
  name: string
  toolpath_path: string
  duration: number
  first_move: number
  last_move: number
  viewer_vertices_path: string
  viewer_uv_path: string
}

interface CollectionForUI {
  name: string
  duration: number
  first_move: number
  last_move: number
  viewer_vertices: ViewerVertices
  viewer_uv: string
}

interface Frame {
  frame_num: number
  collections: Array<Collection>
}

interface SummaryFormat {
  collections: Array<string>
  frames: Array<Frame>
}

export const setFrame = new Action(
  'set_frame',
  async (
    deviceManager: DeviceManager,
    runAction: RunActionFunction,
    frame_number: number,
  ) => {
    const delta = getDelta(deviceManager)

    if (!delta) {
      return
    }

    const current_frame = frame_number

    delta.addMetadata({
      current_frame,
    })
  },
)

export const openScene = new Action(
  'open_scene',
  async (
    deviceManager: DeviceManager,
    runAction: RunActionFunction,
    summaryFilePath: string,
  ) => {
    const delta = getDelta(deviceManager)

    if (!delta) {
      return
    }

    // Parse the summary file
    const summaryBytes = fs.readFileSync(summaryFilePath) // just read it all now
    const summary: SummaryFormat = JSON.parse(summaryBytes.toString())

    const summary_file_path = summaryFilePath

    const available_collections = summary.collections
    const min_frame = summary.frames.reduce((min, frame) => Math.min(min, frame.frame_num), summary.frames[0].frame_num) // prettier-ignore
    const max_frame = summary.frames.reduce((max, frame) => Math.max(max, frame.frame_num), summary.frames[0].frame_num) // prettier-ignore
    const selected_collections = available_collections
    const frames_complete_min = min_frame
    const frames_complete_max = min_frame
    const frame_start = min_frame
    const frame_end = max_frame

    const executing_scene = false
    const executing_collection = false
    const current_collection = selected_collections[0]
    const waiting_on_ui_gate_start_camera = false
    const waiting_on_ui_gate_stop_camera = false

    /*
    Set metadata for 
    - min frame,
    - max frame
    - available collections

    - current frame
    - current collection
    - selected collections
    */

    delta.addMetadata({
      summary_file_path,
      available_collections,
      min_frame,
      max_frame,
      frames_complete_min,
      frames_complete_max,
      executing_scene,
      executing_collection,
      current_collection,
      waiting_on_ui_gate_start_camera,
      waiting_on_ui_gate_stop_camera,
      frame_start,
      frame_end,
    })

    // Parse the current frame and set it
    runAction('set_frame', min_frame)
    // Set the selected collections to all of them
    runAction('set_selected_collections', available_collections)

    console.log('open_scene set to ', summaryFilePath)
  },
)

// When executing, set
// const frames_complete_min = min_frame
// const frames_complete_max = min_frame

export const setSelectedCollections = new Action(
  'set_selected_collections',
  async (
    deviceManager: DeviceManager,
    runAction: RunActionFunction,
    selectedCollections: Array<string>,
  ) => {
    const delta = getDelta(deviceManager)

    if (!delta) {
      return
    }

    const metadata = delta.getMetadata()
    const summaryFilePath = metadata.summary_file_path
    const current_frame = metadata.current_frame

    // Parse the summary file (this may be called later than the open_scene action)

    const summaryBytes = fs.readFileSync(summaryFilePath) // just read it all now
    const summary: SummaryFormat = JSON.parse(summaryBytes.toString())

    // parse out the current frame's data

    const currentFrameData = summary.frames.find(
      frame => frame.frame_num === current_frame,
    )

    if (!currentFrameData) {
      return
    }

    const selectedCollectionData = currentFrameData.collections.filter(
      collection => selectedCollections.includes(collection.name),
    )

    const loadedCollectionData = selectedCollectionData.map(collectionData => {
      const {
        viewer_vertices_path,
        viewer_uv_path,
        toolpath_path, // we pull this out and throw it away
        ...rest
      } = collectionData

      const uvPath = path.join(path.dirname(summaryFilePath), viewer_uv_path)
      const uvDataURI = new Datauri(uvPath)
      const viewer_uv = uvDataURI.content // Send down the UV data as a Datauri for simplicity

      const verticesPath = path.join(
        path.dirname(summaryFilePath),
        viewer_vertices_path,
      )

      const verticesBytes = fs.readFileSync(verticesPath) // just read it all now
      const viewer_vertices: ViewerVertices = JSON.parse(
        verticesBytes.toString(),
      )

      const collectionDataForUI: CollectionForUI = {
        ...rest,
        viewer_vertices,
        viewer_uv,
      }

      return collectionDataForUI
    })

    // Parse out the data of the current frame for these collections // parse out the collection

    // Set the currently selected collections
    delta.addMetadata({
      selected_collections: selectedCollections,
      loaded_collection_data: loadedCollectionData,
    })

    console.log('selected_collections set to ', selectedCollections)
  },
)

interface StopSceneExecutionOptions {}

interface StartSceneExecutionOptions {
  frameStart: number
  frameEnd: number
}

export const stopSceneExecution = new Action(
  'stop_scene_execution',
  async (
    deviceManager: DeviceManager,
    runAction: RunActionFunction,
    options: StopSceneExecutionOptions,
  ) => {
    const delta = getDelta(deviceManager)

    if (!delta) {
      return
    }

    // Set the currently selected collections
    delta.addMetadata({
      executing_scene: false,
      executing_collection: false,
    })

    console.log('Stopped scene execution')
  },
)

export const startSceneExecution = new Action(
  'start_scene_execution',
  async (
    deviceManager: DeviceManager,
    runAction: RunActionFunction,
    options: StartSceneExecutionOptions,
  ) => {
    const delta = getDelta(deviceManager)

    if (!delta) {
      return
    }

    const metadata = delta.getMetadata()

    // Set the selected frames and the start and endpoints
    delta.addMetadata({
      executing_scene: true,
      current_frame: options.frameStart,
      frames_complete_min: options.frameStart,
      frames_complete_max: options.frameStart,
      frame_start: options.frameStart,
      frame_end: options.frameEnd,
      //  executing_collection: false,
    })

    await new Promise((res, rej) => setTimeout(res, 300))

    delta.addMetadata({
      frames_complete_max: options.frameStart + 1,
    })

    await new Promise((res, rej) => setTimeout(res, 1000))
    delta.addMetadata({
      frames_complete_max: options.frameStart + 2,
    })

    await new Promise((res, rej) => setTimeout(res, 1000))
    delta.addMetadata({
      frames_complete_max: options.frameStart + 3,
    })

    await new Promise((res, rej) => setTimeout(res, 1000))

    delta.addMetadata({
      frames_complete_max: options.frameStart + 4,
    })
    await new Promise((res, rej) => setTimeout(res, 1000))
    delta.addMetadata({
      frames_complete_max: options.frameStart + 5,
    })
    await new Promise((res, rej) => setTimeout(res, 1000))

    delta.addMetadata({
      executing_scene: false,
    })

    // trigger sync(1)

    console.log('Started scene execution')
  },
)
