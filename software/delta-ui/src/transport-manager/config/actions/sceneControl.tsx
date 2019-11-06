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
  num_lights: number
  viewer_vertices_path: string
  viewer_uv_path: string
}

interface CollectionForUI {
  name: string
  duration: number
  first_move: number
  last_move: number
  num_lights: number
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

const CAMERA_TRIGGER_ON_DELAY = 250
const INITIAL_MOVEMENT_DURATION = 500
const CAMERA_TRIGGER_OFF_DELAY = 250

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

    // Pause the queues
    await runAction('movement_queue_paused', true)
    await runAction('light_queue_paused', true)

    // Clear the queues
    await runAction('clear_ui_movement_queue', {})
    await runAction('clear_ui_light_queue', {})

    // Clear queues on the delta
    await runAction('clear_queues', {})

    // Stop the camera if it's on
    const cameraOffMessage = new Message('capture', 0)
    cameraOffMessage.metadata.ack = true

    await delta.write(cameraOffMessage)

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
    const summaryFilePath = metadata.summary_file_path
    const current_frame = metadata.current_frame
    const selectedCollections = metadata.selected_collections

    // Parse the summary file (this may be called later than the open_scene action)

    const summaryBytes = fs.readFileSync(summaryFilePath) // just read it all now
    const summary: SummaryFormat = JSON.parse(summaryBytes.toString())

    // Find the frames that we're running, sort them by frame number
    const selectedFrames = summary.frames
      .filter(
        frame =>
          frame.frame_num >= options.frameStart &&
          frame.frame_num <= options.frameEnd,
      )
      .sort((a, b) => {
        if (a.frame_num < b.frame_num) return -1
        if (a.frame_num > b.frame_num) return 1
        return 0
      })

    console.log('selectedFrames', selectedFrames.map(fr => fr.frame_num))

    // Set the metadata that we're starting
    delta.addMetadata({
      executing_scene: true,
      current_frame: options.frameStart,
      frames_complete_min: options.frameStart,
      frames_complete_max: options.frameStart,
      frame_start: options.frameStart,
      frame_end: options.frameEnd,
    })

    // Start iterating over the frames
    for (const frame of selectedFrames) {
      console.log('Executing frame', frame.frame_num)

      // Set the frame
      runAction('set_frame', frame.frame_num)

      // reload the collection data for this frame
      runAction('set_selected_collections', selectedCollections)

      // check if we've stopped execution, cancelling before the next frame
      if (!delta.getMetadata().executing_scene) {
        return
      }

      // render a collection
      await runAction('render_frame', frame)

      delta.addMetadata({
        frames_complete_max: frame.frame_num,
      })

      console.log('Finished frame', frame.frame_num)
    }

    // Clear queues on the delta
    await runAction('clear_queues', {})

    delta.addMetadata({
      executing_scene: false,
    })
  },
)

export const renderFrame = new Action(
  'render_frame',
  async (
    deviceManager: DeviceManager,
    runAction: RunActionFunction,
    frame: Frame,
  ) => {
    const delta = getDelta(deviceManager)

    if (!delta) {
      return
    }

    const metadata = delta.getMetadata()
    const selectedCollections = metadata.selected_collections

    const selectedCollectionData = frame.collections.filter(collection =>
      selectedCollections.includes(collection.name),
    )

    for (const collection of selectedCollectionData) {
      // check if we've stopped execution, cancelling mid frame
      if (!delta.getMetadata().executing_scene) {
        return
      }

      console.log('Starting collection', collection.name)

      // render a collection
      await runAction('render_collection', collection)
      console.log('Ended collection', collection.name)
    }
  },
)

export const renderCollection = new Action(
  'render_collection',
  async (
    deviceManager: DeviceManager,
    runAction: RunActionFunction,
    collection: Collection,
  ) => {
    const delta = getDelta(deviceManager)

    if (!delta) {
      return
    }

    const metadata = delta.getMetadata()
    const summaryFilePath = metadata.summary_file_path

    const absToolpathPath = path.join(
      path.dirname(summaryFilePath),
      collection.toolpath_path,
    )

    const amountOfMovements = collection.last_move - collection.first_move
    const amountOfLights = collection.num_lights

    console.log(
      'Movements in this collection',
      collection.name,
      collection.duration,
      amountOfMovements,
    )

    // Set gates to false
    // Set metadata "waiting on UI gate - start camera" to false
    // Set metadata "waiting on UI gate - end camera" to false
    // Set metadata "executing collection" to true
    delta.addMetadata({
      executing_collection: true,
      current_collection: collection.name,
      waiting_on_ui_gate_start_camera: false,
      waiting_on_ui_gate_stop_camera: false,
    })

    console.log('Pausing UI queues')

    // Pause the movement queue
    await runAction('movement_queue_paused', true)
    // Pause the light queue
    await runAction('light_queue_paused', true)

    console.log('Clearing hardware queues')

    // Clear queues on the delta
    await runAction('clear_queues', {})

    console.log('Loading the collection')

    // Load the collection file
    await runAction('load_collection', absToolpathPath)

    console.log('Streaming initial movements')

    // Unpause movement queue,
    await runAction('movement_queue_paused', false)

    console.log('Streaming initial movements')

    const amountOfMovesToWaitFor = Math.min(
      Math.max(Math.floor(amountOfMovements / 2), 1),
      50,
    ) // send at least 1, and the smaller of half the moves or 50

    console.log('waiting for', amountOfMovesToWaitFor, 'to be sent')

    // Wait until movement queue is somewhat saturated
    const { promise: waitForMovementQueueSaturation } = delta.waitForReply(
      message =>
        message.messageID === 'queue' &&
        message.payload.movements >= amountOfMovesToWaitFor,
    )

    await waitForMovementQueueSaturation

    // check if we've stopped execution, cancelling mid collection
    if (!delta.getMetadata().executing_scene) {
      return
    }

    console.log('Sufficient movements streamed')

    console.log('Streaming initial light ramps')

    // Unpause light queue
    await runAction('light_queue_paused', false)

    const amountOfLightsToWaitFor = Math.min(
      Math.max(Math.floor(amountOfLights / 2), 1),
      50,
    ) // send at least 1, and the smaller of half the moves or 50

    console.log('waiting for', amountOfLightsToWaitFor, 'to be sent')

    // Wait until movement queue is somewhat saturated
    const { promise: waitForLightingQueueSaturation } = delta.waitForReply(
      message =>
        message.messageID === 'queue' &&
        message.payload.lighting >= amountOfLightsToWaitFor,
    )

    await waitForLightingQueueSaturation

    // check if we've stopped execution, cancelling mid collection
    if (!delta.getMetadata().executing_scene) {
      return
    }

    console.log('Sufficient light ramps streamed')

    // Wait until light queue is >= the max queue depth
    // "waiting on UI gate - start camera" to true - modal on UI side, if that and executing collection both true

    // TODO

    // check if we've stopped execution, cancelling mid collection
    if (!delta.getMetadata().executing_scene) {
      return
    }

    console.log('Triggering camera')

    const cameraOnMessage = new Message(
      'capture',
      collection.duration + INITIAL_MOVEMENT_DURATION,
    )
    cameraOnMessage.metadata.ack = true

    await delta.write(cameraOnMessage)

    console.log(`Waiting ${CAMERA_TRIGGER_ON_DELAY} before syncing`)
    await new Promise((res, rej) => setTimeout(res, CAMERA_TRIGGER_ON_DELAY))

    console.log('Sending sync event')

    // Wait until "waiting on UI gate - start camera" to false (the human should trigger the camera)
    // send sync event
    await runAction('sync', 1)

    // check if we've stopped execution, cancelling mid collection
    if (!delta.getMetadata().executing_scene) {
      return
    }

    // we're now rendering the collection

    console.log('Waiting until queue is empty')

    // wait until the movement queue is complete.
    const { promise: waitForMovementQueueEmpty } = delta.waitForReply(
      message => {
        if (message.messageID === 'queue') {
          const queueDepth = message.payload.movements

          return queueDepth === 0
        }
        return false
      },
    )

    await waitForMovementQueueEmpty

    console.log('Queue emptied')

    // check if we've stopped execution, cancelling mid collection
    if (!delta.getMetadata().executing_scene) {
      return
    }

    // raise a toaster if there are any lights that are still in the queue.
    // clear both queues.

    // "waiting on UI gate - end camera" to true - modal on UI side, if that and executing collection both true

    // Wait until "waiting on UI gate - end camera" to false (the human should untrigger the camera)
    // Set metadata "executing collection" to false
    // increment current frame if next frame is <= max frame

    // We're finished
    delta.addMetadata({
      executing_collection: false,
    })

    console.log(`Waiting ${CAMERA_TRIGGER_OFF_DELAY} before finishing`)
    await new Promise((res, rej) => setTimeout(res, CAMERA_TRIGGER_OFF_DELAY))

    console.log('Camera should have turned off by now.')

    if (collection.duration < 1000) {
      console.log(
        'waiting for a minimum of 1 second since the collection was ',
        collection.duration,
      )
      await new Promise((res, rej) => setTimeout(res, 1000))
    }
  },
)
