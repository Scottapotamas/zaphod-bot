import { Card, FormGroup, Intent, MultiSlider, Slider, Button, ButtonProps, Colors } from '@blueprintjs/core'

import React, { useCallback, useEffect, useRef, useState } from 'react'
import { Composition, Box } from 'atomic-layout'

import { sparseToDense } from '../optimiser/passes'
import { toolpath } from '../optimiser/toolpath'

import { SequenceSender } from './sequenceSender'

import { PlannerLightMove, PlannerMovementMove } from './../optimiser/hardware'

import { useHardwareStateSubscription, useQuery, useSendMessage } from '@electricui/components-core'
import { CancellationToken, Message } from '@electricui/core'

import {
  changeState,
  getSetting,
  incrementViewportFrameVersion,
  useSetting,
  useStore,
  useViewportFrameDuration,
  useViewportFrameState,
} from './state'

function SceneLengthSlider() {
  const sceneMinFrame = useStore(state => state.sceneMinFrame) ?? null
  const sceneMaxFrame = useStore(state => state.sceneMaxFrame) ?? null
  const range = sceneMaxFrame - sceneMinFrame

  const [localMinMax, setLocalMinMax] = useState([
    getSetting(state => state.sceneMinFrame),
    getSetting(state => state.sceneMaxFrame),
  ])

  const updateMinCurrentMax = useCallback(values => {
    changeState(state => {
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
        labelStepSize={range > 100 ? 50 : 20}
        min={sceneMinFrame}
        max={sceneMaxFrame}
        onRelease={updateMinCurrentMax}
        onChange={setLocalMinMax}
        stepSize={1}
      >
        <MultiSlider.Handle type="start" value={localMinMax[0]} intentBefore={Intent.WARNING} />

        <MultiSlider.Handle type="end" value={localMinMax[1]} intentAfter={Intent.WARNING} />
      </MultiSlider>
    </div>
  )
}

function Timeline() {
  const selectedMinFrame = useStore(state => state.selectedMinFrame) ?? 1
  const selectedMaxFrame = useStore(state => state.selectedMaxFrame) ?? 1
  const viewportFrame = useStore(state => state.viewportFrame) ?? 1
  const range = selectedMaxFrame - selectedMinFrame

  const updateViewportFrame = useCallback(frameNumber => {
    changeState(state => {
      state.viewportFrame = Math.min(selectedMaxFrame, Math.max(selectedMinFrame, frameNumber))

      // Trigger an update, wrap around
      incrementViewportFrameVersion(state)
    })
  }, [])

  const updatePriorityFrame = useCallback(frameNumber => {
    changeState(state => {
      state.priorityFrame = Math.min(selectedMaxFrame, Math.max(selectedMinFrame, frameNumber))
    })
  }, [])

  return (
    <div style={{ marginLeft: 10, marginRight: 10 }}>
      <Slider
        labelStepSize={range > 100 ? 50 : 20}
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
import { MSGID, QueueDepthInfo } from 'src/application/typedState'
import { getOrderedMovementsForFrame } from './ToolpathVisualisation'
import { FRAME_STATE } from '../optimiser/main'
import { isCamera } from '../optimiser/camera'
import { Vector3 } from 'three'
import { IconNames } from '@blueprintjs/icons'

async function getToolpathForFrame(frameNumber: number) {
  const persistentOptimiser = getSetting(state => state.persistentOptimiser)
  if (!persistentOptimiser) return null

  await persistentOptimiser.waitUntilFrameReady(frameNumber)
  const orderedMovements = getOrderedMovementsForFrame(frameNumber)
  const settings = getSetting(state => state.settings)
  const visualisationSettings = getSetting(state => state.visualisationSettings)

  const renderablesForFrame = getSetting(state => state.renderablesByFrame[state.viewportFrame]) ?? []

  const blenderCamera = renderablesForFrame.find(isCamera)

  const cameraPosition = new Vector3(
    blenderCamera?.position[0] ?? 0,
    blenderCamera?.position[1] ?? 0,
    blenderCamera?.position[2] ?? 0,
  )

  const dense = sparseToDense(orderedMovements, settings)

  const globalMaterialOverride = visualisationSettings.objectMaterialOverrides[GLOBAL_OVERRIDE_OBJECT_ID]
    ? importMaterial(visualisationSettings.objectMaterialOverrides[GLOBAL_OVERRIDE_OBJECT_ID])
    : null

  dense.map(movement => {
    // Find any material overrides
    const movementMaterialOverride = visualisationSettings.objectMaterialOverrides[movement.objectID]

    // Global overrides take least precidence
    if (globalMaterialOverride) {
      movement.material = globalMaterialOverride
    }

    // Specific movement overrides take highest precidence
    if (movementMaterialOverride) {
      movement.material = importMaterial(movementMaterialOverride)
    }
  })

  return toolpath(dense, settings, visualisationSettings, cameraPosition)
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
    <Button
      onClick={handleRender}
      loading={isLoading}
      icon={IconNames.CLIPBOARD}
      intent={Intent.NONE}
      fill
      minimal
      outlined
    >
      <b>COPY TO CLIPBOARD</b>
    </Button>
  )
}

export function SendToolpath() {
  const sequenceSenderRef: React.MutableRefObject<SequenceSender | null> = useRef(null)

  const sendMessage = useSendMessage()
  const query = useQuery()

  const sendMovement = useCallback(
    (move: PlannerMovementMove) => {
      const cancellationToken = new CancellationToken()
      const message = new Message(MSGID.QUEUE_ADD_MOVE, move)

      return sendMessage(message, cancellationToken)
    },
    [sendMessage],
  )

  const sendLightMove = useCallback(
    (fade: PlannerLightMove) => {
      const cancellationToken = new CancellationToken()
      const message = new Message(MSGID.QUEUE_ADD_FADE, fade)

      return sendMessage(message, cancellationToken)
    },
    [sendMessage],
  )

  const sendSync = useCallback(async () => {
    const cancellationToken = new CancellationToken()

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
      const captureMessage = new Message(MSGID.CAPTURE, duration)
      console.log(`Capturing for ${duration}ms`)
      await sendMessage(captureMessage, cancellationToken)
    },
    [sendMessage],
  )

  const queryMotionAndQueueDepth = useCallback(async () => {
    const cancellationToken = new CancellationToken()
    await query(MSGID.QUEUE_INFO, cancellationToken)
    await query(MSGID.MOTION, cancellationToken)
  }, [sendMessage])

  const updateOptimisticQueueDepth = useCallback((movementDepth: number, lightQueueDepth: number) => {
    changeState(state => {
      state.movementQueueUI = movementDepth
      state.lightQueueUI = lightQueueDepth
    })
  }, [])

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
    state => state[MSGID.QUEUE_INFO],
    (queue: QueueDepthInfo) => {
      getSequenceSender().updateHardwareQueues(queue.movements, queue.lighting)
    },
  )

  useHardwareStateSubscription(
    state => state[MSGID.MOTION],
    moStat => {
      if (moStat) {
        getSequenceSender().updateHardwareProgress(moStat.movement_identifier, moStat.move_progress)
      }
    },
  )

  const [isLoading, setIsLoading] = useState(false)
  const cancellationTokenRef = useRef(new CancellationToken())

  const renderSequence = useCallback(
    async (minFrameNumber: number, maxFrameNumber: number, cancellationToken: CancellationToken) => {
      getSequenceSender().clear()

      cancellationTokenRef.current = cancellationToken

      try {
        for (let frameNumber = minFrameNumber; frameNumber <= maxFrameNumber; frameNumber++) {
          console.log(`rendering frame ${frameNumber}`)
          cancellationToken.haltIfCancelled()

          changeState(state => {
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

            // Round up the estimated duration to the nearest second
            return Math.ceil(state.estimatedDurationByFrame[frameNumber] / 1000) * 1000
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
          const captureStart = Date.now()
          let timedCaptureEnd = 0

          await sendCapture(captureDuration) // Start the camera capture

          const captureCompleteTime = new Promise((resolve, reject) => {
            setTimeout(resolve, captureDuration)
            timedCaptureEnd = Date.now()
          })

          console.log(`Sending sync`)

          await sendSync() // Begin rendering
          // TODO: being lighting command

          console.log(`Waiting for frame to complete`)

          await getSequenceSender().waitForFrameToComplete()
          const progressCaptureEnd = Date.now()

          // TODO: stop lighting command

          console.log(`Frame finished rendering`)

          cancellationToken.haltIfCancelled()
          await captureCompleteTime // Wait for the capture duration to finish

          const progressDuration = progressCaptureEnd - captureStart
          const timedDuration = timedCaptureEnd - captureStart
          const expectedDuration = captureDuration

          console.log(
            `Capture finished`,
            `progressDuration:`,
            progressDuration,
            `timedDuration:`,
            timedDuration,
            `expectedDuration:`,
            expectedDuration,
          )
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

    return renderSequence(minFrameNumber, maxFrameNumber, new CancellationToken())
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
    <Composition templateCols="2fr 2fr 1fr" gap={5}>
      <Button
        onClick={handleViewportFrameRender}
        loading={isLoading}
        icon={IconNames.LAB_TEST}
        intent={Intent.NONE}
        fill
        minimal
        outlined
      >
        <b>RENDER FRAME</b>
      </Button>
      <Button
        onClick={handleRangeRender}
        loading={isLoading}
        icon={IconNames.TEXT_HIGHLIGHT}
        intent={Intent.PRIMARY}
        fill
        minimal
        outlined
      >
        <b>RENDER TIMELINE</b>
      </Button>
      <Button onClick={handleClear} icon={IconNames.CROSS} intent={Intent.DANGER} fill minimal outlined>
        <b>CLEAR</b>
      </Button>
    </Composition>
  )
}

export function SendToolpathToDeviceIfExists() {
  const deviceID = useDeviceID()

  const toolpathComponent = deviceID ? <SendToolpath /> : <CopyToolpathToClipboard />

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

  const currentlyRenderingFrame = useSetting(state => state.currentlyRenderingFrame)
  const maxFrame = useSetting(state => state.selectedMaxFrame)

  const cameraDurationOverride = useSetting(state => state.cameraOverrideDuration)

  const totalRenderTimeMinutes = useSetting(state => {
    let total = 0

    for (const [frameNumber, duration] of Object.entries(state.estimatedDurationByFrame)) {
      if (Number(frameNumber) >= state.selectedMinFrame && Number(frameNumber) <= state.selectedMaxFrame) {
        // Round up the estimated duration to the nearest second
        let thisFrameDuration = Math.ceil(duration / 1000) * 1000

        // Override it if we have to
        if (state.cameraOverrideDuration > 0) {
          thisFrameDuration = state.cameraOverrideDuration
        }

        total += thisFrameDuration
      }
    }

    return Math.ceil(total / 1000 / 60)
  })

  return (
    <Composition templateCols="2fr 2fr 1fr" gap={5} justifyItems="center">
      <Box>
        <span style={{ color: Colors.GRAY3 }}>FRAME:</span> <b style={{ color }}>{niceDurationTime.toFixed(1)}s</b>
      </Box>

      <Box>
        <span style={{ color: Colors.GRAY3 }}>TIMELINE:</span>{' '}
        <b style={cameraDurationOverride > 0 ? { color: Colors.RED3 } : undefined}>{totalRenderTimeMinutes} min</b>
      </Box>

      <Box>
        {/* <span style={{ color: Colors.GRAY3 }}>PROGRESS:</span>{' '} */}
        <b>
          {currentlyRenderingFrame}/{maxFrame}
        </b>
      </Box>
    </Composition>
  )
}

export const RenderInterface = () => {
  const numFrames = useStore(state => state.sceneTotalFrames)

  return (
    <div>
      <FormGroup label="IN/OUT POINTS">
        {/* Re-render on total number of frames change */}
        {numFrames < 1 ? (
          <div style={{ textAlign: 'center' }}>Rendering all frames...</div>
        ) : (
          <SceneLengthSlider key={numFrames} />
        )}
      </FormGroup>
      <FormGroup label="TIMELINE">
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
