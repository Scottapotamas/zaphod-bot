import { Card, Checkbox, FormGroup, Slider } from '@blueprintjs/core'

import React, { useCallback, useState } from 'react'

import { getSetting, setSetting } from './state'

function ParticleWaitDurationSlider() {
  const [localStopDelay, setLocalStopDelay] = useState(
    getSetting(state => state.settings.objectSettings.particles.stopDelay) ?? 0,
  )

  const updateStopDelay = useCallback(delay => {
    setSetting(state => {
      state.settings.objectSettings.particles.stopDelay = delay
    })
  }, [])

  return (
    <div style={{ marginLeft: 10, marginRight: 10 }}>
      <Slider
        min={0}
        max={200}
        stepSize={1}
        labelStepSize={50}
        value={localStopDelay}
        labelRenderer={value => `${value}ms`}
        onRelease={updateStopDelay}
        onChange={setLocalStopDelay}
      />
    </div>
  )
}

function MaxSpeedSlider() {
  const [localSpeed, setLocalSpeed] = useState(
    getSetting(state => state.settings.optimisation.maxSpeed),
  )

  const updateMaxSpeed = useCallback(newMaxSpeed => {
    setSetting(state => {
      state.settings.optimisation.maxSpeed = Math.max(newMaxSpeed, 1)
    })
  }, [])

  return (
    <div style={{ marginLeft: 10, marginRight: 10 }}>
      <Slider
        min={0}
        max={300}
        stepSize={1}
        labelStepSize={50}
        value={Math.max(localSpeed, 1)}
        labelRenderer={value => `${value}mm/s`}
        onRelease={updateMaxSpeed}
        onChange={setLocalSpeed}
      />
    </div>
  )
}

function TransitionMaxSpeedSlider() {
  const [localSpeed, setLocalSpeed] = useState(
    getSetting(state => state.settings.optimisation.transitionMaxSpeed),
  )

  const updateTransitionMaxSpeed = useCallback(newTransitionMaxSpeed => {
    setSetting(state => {
      state.settings.optimisation.transitionMaxSpeed = Math.max(
        newTransitionMaxSpeed,
        1,
      )
    })
  }, [])

  return (
    <div style={{ marginLeft: 10, marginRight: 10 }}>
      <Slider
        min={0}
        max={300}
        stepSize={1}
        labelStepSize={50}
        value={Math.max(localSpeed, 1)}
        labelRenderer={value => `${value}mm/s`}
        onRelease={updateTransitionMaxSpeed}
        onChange={setLocalSpeed}
      />
    </div>
  )
}

function WaitAtStartDurationSlider() {
  const [waitDuration, setWaitDuration] = useState(
    getSetting(state => state.settings.optimisation.waitAtStartDuration),
  )

  const updateWaitDuration = useCallback(waitDuration => {
    setSetting(state => {
      state.settings.optimisation.waitAtStartDuration = waitDuration
    })
  }, [])

  return (
    <div style={{ marginLeft: 10, marginRight: 10 }}>
      <Slider
        min={0}
        max={5000}
        stepSize={100}
        labelStepSize={500}
        value={waitDuration}
        labelRenderer={value => `${Math.round((value / 1000) * 10) / 10}s`}
        onRelease={updateWaitDuration}
        onChange={setWaitDuration}
      />
    </div>
  )
}


function EfficientGenerationCheckbox() {
  const [efficient, setEfficient] = useState(
    getSetting(state => state.efficientVisualisation),
  )

  const updateChecked = useCallback(checked => {
    setEfficient(checked)
    setSetting(state => {
      state.efficientVisualisation = checked
    })
  }, [])

  return (
    <div style={{ marginLeft: 10, marginRight: 10 }}>
      <Checkbox
        checked={efficient}
        onChange={updateChecked}
      />
    </div>
  )
}

export const SettingsInterface = () => {
  return (
    <Card>
      <FormGroup label="Particle wait duration">
        <ParticleWaitDurationSlider />
      </FormGroup>
      <FormGroup label="Maximum speed">
        <MaxSpeedSlider />
      </FormGroup>
      <FormGroup label="Maximum transition speed">
        <TransitionMaxSpeedSlider />
      </FormGroup>
      <FormGroup label="Wait at start duration">
        <WaitAtStartDurationSlider />
      </FormGroup>
      <FormGroup label="Efficient generation of visualisation">
        <EfficientGenerationCheckbox />
      </FormGroup>
    </Card>
  )
}
