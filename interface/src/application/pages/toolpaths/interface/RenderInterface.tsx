import { Card, FormGroup, Intent, MultiSlider } from '@blueprintjs/core'

import React, { useCallback, useEffect, useRef, useState } from 'react'

import { getSetting, setSetting, useStore } from './state'

function SceneLengthSlider() {
  const sceneMinFrame = useStore(state => state.sceneMinFrame) ?? null
  const sceneMaxFrame = useStore(state => state.sceneMaxFrame) ?? null

  const [localMinMax, setLocalMinMax] = useState([
    getSetting(state => state.sceneMinFrame),
    getSetting(state => state.sceneMaxFrame),
  ])

  const updateMinMax = useCallback(values => {
    setSetting(state => {
      state.selectedMinFrame = values[0]
      state.selectedMaxFrame = values[1]
    })
  }, [])

  return (
    <div style={{ marginLeft: 10, marginRight: 10 }}>
      <MultiSlider
        defaultTrackIntent={Intent.SUCCESS}
        labelStepSize={20}
        min={sceneMinFrame}
        max={sceneMaxFrame}
        onRelease={updateMinMax}
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

export const RenderInterface = () => {
  const numFrames = useStore(state => state.sceneTotalFrames)

  return (
    <Card>
      <FormGroup label="Scene Frame Limits">
        {/* Re-render on total number of frames change */}
        {numFrames < 1 ? (
          <div style={{ textAlign: 'center' }}>Rendering all frames...</div>
        ) : (
          <SceneLengthSlider key={numFrames} />
        )}
      </FormGroup>
    </Card>
  )
}
