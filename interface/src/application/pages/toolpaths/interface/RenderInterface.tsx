import { Card, FormGroup, Intent, MultiSlider, Slider } from '@blueprintjs/core'

import React, { useCallback, useEffect, useRef, useState } from 'react'

import {
  getSetting,
  incrementViewportFrameVersion,
  setSetting,
  useStore,
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
        stepSize={2}
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

export const RenderInterface = () => {
  const numFrames = useStore(state => state.sceneTotalFrames)

  return (
    <Card>
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
    </Card>
  )
}
