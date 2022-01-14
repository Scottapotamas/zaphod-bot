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

async function getToolpathForFrame(frameNumber: number) {
  const persistentOptimiser = getSetting(state => state.persistentOptimiser)
  if (!persistentOptimiser) return null

  await persistentOptimiser.waitUntilFrameReady(frameNumber)
  const orderedMovements = getOrderedMovementsForFrame(frameNumber)
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

    const viewportFrame = getSetting(state => state.viewportFrame)

    getToolpathForFrame(viewportFrame).then(t => {
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

const ESTIMATED_DURATION_OFFSET = 2000

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
    await sendCapture(0) // cancel any currently running capture

    await query(MSGID.QUEUE_INFO, cancellationToken)
  }, [sendMessage])

  const sendAngle = useCallback(
    async (angle: number) => {
      const cancellationToken = new CancellationToken()
      const syncMessage = new Message(MSGID.POSITION_EXPANSION, angle)
      await sendMessage(syncMessage, cancellationToken)
    },
    [sendMessage],
  )

  const sendCapture = useCallback(
    async (duration: number) => {
      const cancellationToken = new CancellationToken()
      const syncMessage = new Message(MSGID.CAPTURE, duration)
      console.log(`Capturing for ${duration}ms`)
      await sendMessage(syncMessage, cancellationToken)
    },
    [sendMessage],
  )

  const queryMotionAndQueueDepth = useCallback(async () => {
    const cancellationToken = new CancellationToken()
    await query(MSGID.QUEUE_INFO, cancellationToken)
    await query(MSGID.MOTION, cancellationToken)
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
        queryMotionAndQueueDepth,
        updateOptimisticQueueDepth,
      )
    }
    return sequenceSenderRef.current
  }

  useEffect(() => {
    return () => {
      // clean up the sequence sender on unmount
      getSequenceSender().clear()
    }
  }, [])

  useHardwareStateSubscription(
    state => state.queue,
    (queue: { movements: number; lighting: number }) => {
      getSequenceSender().updateHardwareQueues(queue.movements, queue.lighting)
    },
  )

  useHardwareStateSubscription(
    state => state[MSGID.MOTION],
    moStat => {
      if (moStat) {
        getSequenceSender().updateHardwareProgress(
          moStat.movement_identifier,
          moStat.move_progress,
        )
      }
    },
  )

  const [isLoading, setIsLoading] = useState(false)
  const cancellationTokenRef = useRef(new CancellationToken())

  const renderSequence = useCallback(
    async (
      minFrameNumber: number,
      maxFrameNumber: number,
      cancellationToken: CancellationToken,
    ) => {
      getSequenceSender().clear()

      cancellationTokenRef.current = cancellationToken

      try {
        for (
          let frameNumber = minFrameNumber;
          frameNumber <= maxFrameNumber;
          frameNumber++
        ) {
          console.log(`rendering frame ${frameNumber}`)
          cancellationToken.haltIfCancelled()

          setSetting(state => {
            state.currentlyRenderingFrame = frameNumber
            state.viewportFrame = frameNumber
            incrementViewportFrameVersion(state)

            // TODO: Report progress to UI, frame number, expected duration remaining
          })

          // Calculate how long to capture for
          const captureDuration = getSetting(state => {
            if (state.cameraOverrideDuration > 0) {
              return state.cameraOverrideDuration
            }

            return (
              state.estimatedDurationByFrame[frameNumber] +
              ESTIMATED_DURATION_OFFSET
            ) // two seconds of wiggle room because who knows
          })

          // Process the toolpath into final form
          const toolpath = await getToolpathForFrame(frameNumber)

          if (!toolpath) {
            console.error(`Couldn't find toolpath for frame ${frameNumber}`)
            return
          }

          console.log(`toolpath has ${toolpath.movementMoves.length} moves`)

          if (toolpath.movementMoves.length === 0) {
            // TODO: Still capture with the camera
            continue
          }

          // TODO: Ask fourth axis to go to desired position
          await getSequenceSender().ingest(toolpath, cancellationToken)
          await getSequenceSender().waitForInitialBatch()

          console.log(`Initial batch sent to Delta`)

          // TODO: Wait for fourth axis to reach desired position

          cancellationToken.haltIfCancelled()

          // Start the capture
          await sendCapture(captureDuration) // Start the camera capture
          const captureCompleteTime = new Promise((resolve, reject) =>
            setTimeout(resolve, captureDuration),
          )

          console.log(`Sending sync`)

          await sendSync() // Begin rendering

          console.log(`Waiting for frame to complete`)

          await getSequenceSender().waitForFrameToComplete()

          console.log(`Frame finished rendering`)

          cancellationToken.haltIfCancelled()
          await captureCompleteTime // Wait for the capture duration to finish

          console.log(`Capture finished`)
        }
      } catch (e) {
        if (cancellationToken.caused(e)) {
          // noop
        } else {
          console.error(e)
        }
      }
    },
    [setIsLoading, sendSync],
  )

  const handleRangeRender = useCallback(() => {
    const minFrameNumber = getSetting(state => state.selectedMinFrame)
    const maxFrameNumber = getSetting(state => state.selectedMaxFrame)
    cancellationTokenRef.current?.cancel()

    return renderSequence(
      minFrameNumber,
      maxFrameNumber,
      new CancellationToken(),
    )
  }, [renderSequence])

  const handleViewportFrameRender = useCallback(() => {
    const viewportFrame = getSetting(state => state.viewportFrame)
    cancellationTokenRef.current?.cancel()

    return renderSequence(viewportFrame, viewportFrame, new CancellationToken())
  }, [setIsLoading, sendSync])

  const handleClear = useCallback(() => {
    cancellationTokenRef.current?.cancel()
    getSequenceSender().clear()
  }, [])

  return (
    <>
      <Button
        onClick={handleViewportFrameRender}
        loading={isLoading}
        intent="primary"
      >
        Render viewport frame
      </Button>
      <Button onClick={handleRangeRender} loading={isLoading} intent="success">
        Render sequence
      </Button>
      <Button onClick={handleClear} intent="danger">
        Clear
      </Button>
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

  const currentlyRenderingFrame = useSetting(
    state => state.currentlyRenderingFrame,
  )
  const maxFrame = useSetting(state => state.selectedMaxFrame)

  const totalRendertime = useSetting(state => {
    let total = 0

    for (const duration of Object.values(state.estimatedDurationByFrame)) {
      total += duration + ESTIMATED_DURATION_OFFSET
    }

    return total
  })

  return (
    <>
      <b>Estimated Time:</b> <span style={{ color }}>{niceDurationTime}s</span>
      <br />
      <b>Rendering frame</b>:{' '}
      <span style={{ color: Colors.GRAY3 }}>
        {currentlyRenderingFrame}/{maxFrame}
      </span>
      <br />
      <b>Total duration</b>:{' '}
      <span style={{ color: Colors.GRAY3 }}>
        {Math.round((totalRendertime / 1000) * 10) / 10}s (
        {Math.ceil(totalRendertime / 1000 / 60)} min)
      </span>
    </>
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

      <p>
        <CurrentFrameTime />
      </p>

      <SendToolpathToDeviceIfExists />
    </div>
  )
}
