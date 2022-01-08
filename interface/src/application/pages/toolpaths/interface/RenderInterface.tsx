import {
  Card,
  FormGroup,
  Intent,
  MultiSlider,
  Slider,
  Button,
  ButtonProps,
  Colors,
} from '@blueprintjs/core'

import React, { useCallback, useEffect, useRef, useState } from 'react'
import { sparseToDense } from '../optimiser/passes'
import { toolpath } from '../optimiser/toolpath'

import { SequenceSender } from './sequenceSender'

import { LightMove, MovementMove } from './../optimiser/hardware'

import {
  useHardwareStateSubscription,
  useQuery,
  useSendMessage,
} from '@electricui/components-core'
import { CancellationToken, Message } from '@electricui/core'

import {
  getSetting,
  incrementViewportFrameVersion,
  setSetting,
  useSetting,
  useStore,
  useViewportFrameDuration,
  useViewportFrameState,
} from './state'

function SceneLengthSlider() {
  const sceneMinFrame = useStore(state => state.sceneMinFrame) ?? null
  const sceneMaxFrame = useStore(state => state.sceneMaxFrame) ?? null

  const [localMinMax, setLocalMinMax] = useState([
    getSetting(state => state.sceneMinFrame),
    getSetting(state => state.sceneMaxFrame),
  ])

  const updateMinCurrentMax = useCallback(values => {
    setSetting(state => {
      state.selectedMinFrame = values[0]
      state.selectedMaxFrame = values[1]

      if (state.viewportFrame < state.selectedMinFrame) {
        state.viewportFrame = state.selectedMinFrame
      }
      if (state.viewportFrame > state.selectedMaxFrame) {
        state.viewportFrame = state.selectedMaxFrame
      }
    })
  }, [])

  return (
    <div style={{ marginLeft: 10, marginRight: 10 }}>
      <MultiSlider
        defaultTrackIntent={Intent.SUCCESS}
        labelStepSize={20}
        min={sceneMinFrame}
        max={sceneMaxFrame}
        onRelease={updateMinCurrentMax}
        onChange={setLocalMinMax}
        stepSize={1}
      >
        <MultiSlider.Handle
          type="start"
          value={localMinMax[0]}
          intentBefore="warning"
        />

        <MultiSlider.Handle
          type="end"
          value={localMinMax[1]}
          intentAfter="warning"
        />
      </MultiSlider>
    </div>
  )
}

function Timeline() {
  const selectedMinFrame = useStore(state => state.selectedMinFrame) ?? 1
  const selectedMaxFrame = useStore(state => state.selectedMaxFrame) ?? 1
  const viewportFrame = useStore(state => state.viewportFrame) ?? 1

  const updateViewportFrame = useCallback(frameNumber => {
    setSetting(state => {
      state.viewportFrame = Math.min(
        selectedMaxFrame,
        Math.max(selectedMinFrame, frameNumber),
      )

      // Trigger an update, wrap around
      incrementViewportFrameVersion(state)
    })
  }, [])

  const updatePriorityFrame = useCallback(frameNumber => {
    setSetting(state => {
      state.priorityFrame = Math.min(
        selectedMaxFrame,
        Math.max(selectedMinFrame, frameNumber),
      )
    })
  }, [])

  return (
    <div style={{ marginLeft: 10, marginRight: 10 }}>
      <Slider
        labelStepSize={20}
        min={selectedMinFrame}
        max={selectedMaxFrame}
        value={viewportFrame}
        onChange={updateViewportFrame}
        onRelease={updatePriorityFrame}
        stepSize={1}
      />
    </div>
  )
}

import { clipboard } from 'electron'
import { Material } from '../optimiser/materials/Base'
import { GLOBAL_OVERRIDE_OBJECT_ID } from '../optimiser/movements'
import { importMaterial } from '../optimiser/material'
import { useDeviceID } from '@electricui/components-core'
import { MSGID } from 'src/application/typedState'
import { getOrderedMovementsForFrame } from './ToolpathVisualisation'
import { FRAME_STATE } from '../optimiser/main'

async function getToolpathForCurrentFrame() {
  const viewportFrame = getSetting(state => state.viewportFrame)
  const persistentOptimiser = getSetting(state => state.persistentOptimiser)
  if (!persistentOptimiser) return null

  await persistentOptimiser.waitUntilFrameReady(viewportFrame)
  const orderedMovements = getOrderedMovementsForFrame(viewportFrame)
  const settings = getSetting(state => state.settings)
  const visualisationSettings = getSetting(state => state.visualisationSettings)

  const dense = sparseToDense(orderedMovements, settings)

  const globalMaterialOverride = visualisationSettings.objectMaterialOverrides[
    GLOBAL_OVERRIDE_OBJECT_ID
  ]
    ? importMaterial(
        visualisationSettings.objectMaterialOverrides[
          GLOBAL_OVERRIDE_OBJECT_ID
        ],
      )
    : null

  dense.map(movement => {
    // Find any material overrides
    const movementMaterialOverride =
      visualisationSettings.objectMaterialOverrides[movement.objectID]

    // Global overrides take least precidence
    if (globalMaterialOverride) {
      movement.material = globalMaterialOverride
    }

    // Specific movement overrides take highest precidence
    if (movementMaterialOverride) {
      movement.material = importMaterial(movementMaterialOverride)
    }
  })

  return toolpath(dense)
}

function CopyToolpathToClipboard() {
  const [isLoading, setIsLoading] = useState(false)

  const handleRender = useCallback(() => {
    setIsLoading(true)

    getToolpathForCurrentFrame().then(t => {
      // Just copy it to the clipboard for now
      clipboard.writeText(JSON.stringify(t))

      setIsLoading(false)
    })
  }, [])

  return (
    <Button onClick={handleRender} loading={isLoading}>
      Copy to clipboard
    </Button>
  )
}

export function SendToolpath() {
  const sequenceSenderRef: React.MutableRefObject<SequenceSender | null> =
    useRef(null)

  const sendMessage = useSendMessage()
  const query = useQuery()

  const sendMovement = useCallback(
    (move: MovementMove) => {
      const cancellationToken = new CancellationToken()
      const message = new Message(MSGID.QUEUE_ADD_MOVE, move)

      return sendMessage(message, cancellationToken)
    },
    [sendMessage],
  )

  const sendLightMove = useCallback(
    (fade: LightMove) => {
      const cancellationToken = new CancellationToken()
      const message = new Message(MSGID.QUEUE_ADD_FADE, fade)

      return sendMessage(message, cancellationToken)
    },
    [sendMessage],
  )

  const sendSync = useCallback(async () => {
    const cancellationToken = new CancellationToken()
    const syncIDMessage = new Message(MSGID.QUEUE_SYNC_ID, 1)
    await sendMessage(syncIDMessage, cancellationToken)

    const syncMessage = new Message(MSGID.QUEUE_SYNC, null)
    await sendMessage(syncMessage, cancellationToken)
  }, [sendMessage])

  const sendClear = useCallback(async () => {
    const cancellationToken = new CancellationToken()
    const syncMessage = new Message(MSGID.QUEUE_CLEAR, null)
    await sendMessage(syncMessage, cancellationToken)

    await query(MSGID.QUEUE_INFO, cancellationToken)
  }, [sendMessage])

  const queryQueueDepth = useCallback(async () => {
    const cancellationToken = new CancellationToken()
    await query(MSGID.QUEUE_INFO, cancellationToken)
  }, [sendMessage])

  const updateOptimisticQueueDepth = useCallback(
    (movementDepth: number, lightQueueDepth: number) => {
      setSetting(state => {
        state.movementQueueUI = movementDepth
        state.lightQueueUI = lightQueueDepth
      })
    },
    [],
  )

  function getSequenceSender() {
    if (!sequenceSenderRef.current) {
      sequenceSenderRef.current = new SequenceSender(
        sendMovement,
        sendLightMove,
        sendClear,
        queryQueueDepth,
        updateOptimisticQueueDepth,
      )
    }
    return sequenceSenderRef.current
  }

  useHardwareStateSubscription(
    state => state.queue,
    (queue: { movements: number; lighting: number }) => {
      getSequenceSender().updateHardwareQueues(queue.movements, queue.lighting)
    },
  )

  useHardwareStateSubscription(
    state => state.err,
    (err: string) => {
      console.error(`Hardware reporting error: ${err}`)
    },
  )

  const [isLoading, setIsLoading] = useState(false)

  const handleRender = useCallback(() => {
    setIsLoading(true)

    getSequenceSender().clear()

    getToolpathForCurrentFrame().then(t => {
      setIsLoading(false)
      // The toolpath is generated, queue it

      if (!t) {
        console.log(`couldn't find toolpath for frame?`)
        return
      }

      getSequenceSender().ingest(t)
    })
  }, [setIsLoading])

  const handleStart = useCallback(() => {
    sendSync()
  }, [sendSync])

  const handleClear = useCallback(() => {
    getSequenceSender().clear()
  }, [])

  return (
    <>
      <Button onClick={handleRender} loading={isLoading}>
        Send to hardware
      </Button>

      <Button onClick={handleStart}>Sync</Button>
      <Button onClick={handleClear}>Clear</Button>
    </>
  )
}

export function SendToolpathToDeviceIfExists() {
  const deviceID = useDeviceID()

  const toolpathComponent = deviceID ? (
    <SendToolpath />
  ) : (
    <CopyToolpathToClipboard />
  )

  return toolpathComponent
}

export function CurrentFrameTime() {
  const duration = useViewportFrameDuration()
  const frameState = useViewportFrameState()

  const niceDurationTime = Math.round((duration / 1000) * 10) / 10

  let color = Colors.BLACK

  switch (frameState) {
    case FRAME_STATE.OPTIMISING_PARTIALLY:
      color = Colors.BLUE3
      break
    case FRAME_STATE.OPTIMISING_FULLY:
      color = Colors.GREEN3
      break
    case FRAME_STATE.UNOPTIMISED:
      color = Colors.BLACK
      break
    case FRAME_STATE.OPTIMISED_PARTIALLY:
      color = Colors.BLUE5
      break
    case FRAME_STATE.OPTIMISED_FULLY:
      color = Colors.GREEN5
      break
    case FRAME_STATE.ERRORED:
      color = Colors.RED3
      break

    default:
      break
  }

  return (
    <p>
      <b>Estimated Time:</b> <span style={{ color }}>{niceDurationTime}s</span>
    </p>
  )
}

export const RenderInterface = () => {
  const numFrames = useStore(state => state.sceneTotalFrames)

  return (
    <div>
      <FormGroup label="Frame Limits">
        {/* Re-render on total number of frames change */}
        {numFrames < 1 ? (
          <div style={{ textAlign: 'center' }}>Rendering all frames...</div>
        ) : (
          <SceneLengthSlider key={numFrames} />
        )}
      </FormGroup>
      <FormGroup label="Timeline">
        {/* Re-render on total number of frames change */}
        {numFrames < 1 ? null : <Timeline key={numFrames} />}
      </FormGroup>

      <CurrentFrameTime />

      <SendToolpathToDeviceIfExists />
    </div>
  )
}
