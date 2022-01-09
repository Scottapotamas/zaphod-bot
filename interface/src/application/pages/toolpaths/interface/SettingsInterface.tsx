import { Card, Checkbox, FormGroup, Slider } from '@blueprintjs/core'

import React, { useCallback, useState } from 'react'
import { isCamera } from '../optimiser/camera'

import { getSetting, incrementViewportFrameVersion, setSetting } from './state'

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
        max={500}
        stepSize={10}
        labelStepSize={100}
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
        max={500}
        stepSize={10}
        labelStepSize={100}
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
        min={500}
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

function InterLineTransitionAngleSlider() {
  const [angle, setAngle] = useState(
    getSetting(state => state.settings.optimisation.interLineTransitionAngle),
  )

  const updateAngle = useCallback(angle => {
    setSetting(state => {
      state.settings.optimisation.interLineTransitionAngle = angle
    })
  }, [])

  return (
    <div style={{ marginLeft: 10, marginRight: 10 }}>
      <Slider
        min={0}
        max={360}
        stepSize={1}
        labelStepSize={30}
        value={angle}
        labelRenderer={value => `${value}°`}
        onRelease={updateAngle}
        onChange={setAngle}
      />
    </div>
  )
}

function InterLineTransitionDistanceSlider() {
  const [distange, setDistance] = useState(
    getSetting(
      state => state.settings.optimisation.interLineTransitionShaveDistance,
    ),
  )

  const updateDistance = useCallback(distance => {
    setSetting(state => {
      state.settings.optimisation.interLineTransitionShaveDistance = distance
    })
  }, [])

  return (
    <div style={{ marginLeft: 10, marginRight: 10 }}>
      <Slider
        min={1}
        max={10}
        stepSize={1}
        labelStepSize={2}
        value={distange}
        labelRenderer={value => `${value}mm`}
        onRelease={updateDistance}
        onChange={setDistance}
      />
    </div>
  )
}

function OrbitCameraCheckbox() {
  const [enabled, setEnabled] = useState(true)

  const updateChecked: React.FormEventHandler<HTMLInputElement> = useCallback(
    event => {
      const checked = event.currentTarget.checked
      setEnabled(checked)
      const orbitControls = getSetting(state => state.orbitControls)

      if (orbitControls) {
        orbitControls.enabled = checked
      }

      // Search for the camera for this frame if we've disabled orbitControls
      if (!checked) {
        console.log(`aligning camera`)

        const sceneCamera = getSetting(state => state.camera)
        const renderablesForFrame =
          getSetting(state => state.renderablesByFrame[state.viewportFrame]) ??
          []

        const blenderCamera = renderablesForFrame.find(isCamera)

        if (sceneCamera && blenderCamera) {
          blenderCamera.alignCamera(sceneCamera)
        }
      }
    },
    [],
  )

  return (
    <div>
      <Checkbox checked={enabled} onChange={updateChecked} />
    </div>
  )
}

function AnnotateOrderingCheckbox() {
  const [enabled, setEnabled] = useState(
    getSetting(state => state.visualisationSettings.annotateDrawOrder),
  )

  const updateChecked: React.FormEventHandler<HTMLInputElement> = useCallback(
    event => {
      const checked = event.currentTarget.checked
      setEnabled(checked)

      setSetting(state => {
        state.visualisationSettings.annotateDrawOrder = checked
        incrementViewportFrameVersion(state)
      })
    },
    [],
  )

  return (
    <div>
      <Checkbox checked={enabled} onChange={updateChecked} />
    </div>
  )
}

export const SettingsInterface = () => {
  return (
    <Card>
      Optimisation | Materials | View
      <FormGroup label="Particle wait duration">
        <ParticleWaitDurationSlider />
      </FormGroup>
      <FormGroup label="InterLine Transition Angle">
        <InterLineTransitionAngleSlider />
      </FormGroup>
      <FormGroup label="InterLine Transition Distance">
        <InterLineTransitionDistanceSlider />
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
      <FormGroup label="Orbit Camera">
        <OrbitCameraCheckbox />
      </FormGroup>
      <FormGroup label="Annotate Ordering">
        <AnnotateOrderingCheckbox />
      </FormGroup>
    </Card>
  )
}
