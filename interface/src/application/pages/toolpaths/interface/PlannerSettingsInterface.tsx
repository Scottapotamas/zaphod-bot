import {
  Alignment,
  Button,
  Card,
  Checkbox,
  FormGroup,
  NumericInput,
  Slider,
  Switch,
  Tag,
} from '@blueprintjs/core'
import { IconNames } from '@blueprintjs/icons'

import { Composition, Box } from 'atomic-layout'

import React, { useCallback, useState } from 'react'

import { getSetting, incrementViewportFrameVersion, setSetting } from './state'

function InterLineTransitionAngleControl() {
  const [angle, setAngle] = useState(
    getSetting(state => state.settings.optimisation.interLineTransitionAngle),
  )

  const updateAngle = useCallback(angle => {
    setSetting(state => {
      state.settings.optimisation.interLineTransitionAngle = angle
    })
  }, [])

  const setAndUpdateAngle = useCallback(angle => {
    setAngle(angle)
    updateAngle(angle)
  }, [])

  return (
    // <div style={{ marginLeft: 10, marginRight: 10 }}>
    //   <Slider
    //     min={0}
    //     max={360}
    //     stepSize={1}
    //     labelStepSize={30}
    //     value={angle}
    //     labelRenderer={value => `${value}°`}
    //     onRelease={updateAngle}
    //     onChange={setAngle}
    //   />
    // </div>
    <NumericInput
    fill
    min={0}
    max={360}
    stepSize={1}
    value={angle}
    onValueChange={setAndUpdateAngle}
    rightElement={<Tag>°</Tag>}
    style={{ width: '100%' }}
  />
  )
}

function InterLineTransitionDistanceControl() {
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

  const setAndUpdateDistance = useCallback(distance => {
    setDistance(distance)
    updateDistance(distance)
  }, [])

  return (
    // <div style={{ marginLeft: 10, marginRight: 10 }}>
    //   <Slider
    //     min={1}
    //     max={10}
    //     stepSize={1}
    //     labelStepSize={2}
    //     value={distange}
    //     labelRenderer={value => `${value}mm`}
    //     onRelease={updateDistance}
    //     onChange={setDistance}
    //   />
    // </div>
    <NumericInput
    fill
    min={1}
    max={10}
    stepSize={1}
    value={distange}
    onValueChange={setAndUpdateDistance}
    rightElement={<Tag>mm</Tag>}
    style={{ width: '100%' }}
  />

  )
}

function ParticleWaitDurationControl() {
  const [localStopDelay, setLocalStopDelay] = useState(
    getSetting(state => state.settings.objectSettings.particles.stopDelay) ?? 0,
  )

  const updateStopDelay = useCallback(delay => {
    setSetting(state => {
      state.settings.objectSettings.particles.stopDelay = delay
    })
  }, [])

  const setAndUpdateStopDelay = useCallback(delay => {
    setLocalStopDelay(delay)
    updateStopDelay(delay)
  }, [])

  return (
    <NumericInput
      fill
      min={0}
      max={200}
      stepSize={25}
      value={localStopDelay}
      onValueChange={setAndUpdateStopDelay}
      rightElement={<Tag>ms</Tag>}
      style={{ width: '100%' }}
    />
  )
}

function MaxSpeedControl() {
  const [localSpeed, setLocalSpeed] = useState(
    getSetting(state => state.settings.optimisation.maxSpeed),
  )

  const updateMaxSpeed = useCallback(newMaxSpeed => {
    setSetting(state => {
      state.settings.optimisation.maxSpeed = Math.max(newMaxSpeed, 1)
    })
  }, [])

  const setAndUpdateMaxSpeed = useCallback(newMaxSpeed => {
    setLocalSpeed(newMaxSpeed)
    updateMaxSpeed(newMaxSpeed)
  }, [])

  return (
    <NumericInput
      fill
      min={0}
      max={300}
      stepSize={25}
      value={localSpeed}
      onValueChange={setAndUpdateMaxSpeed}
      rightElement={<Tag>mm/sec</Tag>}
      style={{ width: '100%' }}
    />
  )
}

function TransitionMaxSpeedControl() {
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

  const setAndUpdateTransitionMaxSpeed = useCallback(newTransitionMaxSpeed => {
    setLocalSpeed(newTransitionMaxSpeed)
    updateTransitionMaxSpeed(newTransitionMaxSpeed)
  }, [])

  return (
    <NumericInput
      fill
      min={0}
      max={300}
      stepSize={25}
      value={localSpeed}
      onValueChange={setAndUpdateTransitionMaxSpeed}
      rightElement={<Tag>mm/sec</Tag>}
      style={{ width: '100%' }}
    />
  )
}

function WaitAtStartDurationControl() {
  const [waitDuration, setWaitDuration] = useState(
    getSetting(state => state.settings.optimisation.waitAtStartDuration),
  )

  const updateWaitDuration = useCallback(waitDuration => {
    setSetting(state => {
      state.settings.optimisation.waitAtStartDuration = waitDuration
    })
  }, [])

  const setAndUpdateDuration = useCallback(waitDuration => {
    setWaitDuration(waitDuration)
    updateWaitDuration(waitDuration)
  }, [])

  return (
    <NumericInput
      fill
      min={0}
      max={5000}
      stepSize={100}
      value={waitDuration}
      onValueChange={setAndUpdateDuration}
      rightElement={<Tag>ms</Tag>}
      style={{ width: '100%' }}
    />
  )
}

export const PlannerSettingsInterface = () => {
  return (
    <div>
      <Composition templateCols="1fr 2fr" gap="1em" alignItems="center">
        Max Speed
        <MaxSpeedControl />
        Max Transition
        <TransitionMaxSpeedControl />
        Wait at Particle
        <ParticleWaitDurationControl />
        Wait before Start
        <WaitAtStartDurationControl />
        Interline Transition Angle
        <InterLineTransitionAngleControl />
        Interline Transition Distance
        <InterLineTransitionDistanceControl />

        </Composition>
    </div>
  )
}
