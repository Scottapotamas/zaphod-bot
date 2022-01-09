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
  Tab,
  Tabs,
  TabId,
} from '@blueprintjs/core'
import { IconNames } from '@blueprintjs/icons'

import { Composition, Box } from 'atomic-layout'

import React, { useCallback, useState } from 'react'

import { getSetting, incrementViewportFrameVersion, setSetting } from './state'

function InterLineTransitionEnabledControl() {
  const [allowed, setAllow] = useState(
    getSetting(state => state.settings.optimisation.smoothInterlineTransitions),
  )

  const updateAllowed = useCallback(allowed => {
    setSetting(state => {
      state.settings.optimisation.smoothInterlineTransitions = allowed
    })
  }, [])

  const setAndUpdateAngle: React.FormEventHandler<HTMLInputElement> =
    useCallback(event => {
      const checked = (event.target as HTMLInputElement).checked
      setAllow(checked)
      updateAllowed(checked)
    }, [])

  return <Checkbox checked={allowed} onChange={setAndUpdateAngle} />
}

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
      majorStepSize={100}
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
      majorStepSize={100}
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
      majorStepSize={100}
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
      majorStepSize={1000}
      value={waitDuration}
      onValueChange={setAndUpdateDuration}
      rightElement={<Tag>ms</Tag>}
      style={{ width: '100%' }}
    />
  )
}

function HideLightIfBlackControl() {
  const [setting, set] = useState(
    getSetting(state => state.settings.objectSettings.light.hideIfBlack),
  )

  const updateSetting = useCallback(setting => {
    setSetting(state => {
      state.settings.objectSettings.light.hideIfBlack = setting
    })
  }, [])

  const handleClick: React.FormEventHandler<HTMLInputElement> = useCallback(
    event => {
      const checked = (event.target as HTMLInputElement).checked
      set(checked)
      updateSetting(checked)
    },
    [],
  )

  return <Checkbox checked={setting} onChange={handleClick} />
}

function DrawFrustumAlignmentControl() {
  const [setting, set] = useState(
    getSetting(
      state => state.settings.objectSettings.camera.drawAlignmentHelpers,
    ),
  )

  const updateSetting = useCallback(setting => {
    setSetting(state => {
      state.settings.objectSettings.camera.drawAlignmentHelpers = setting
    })
  }, [])

  const handleClick: React.FormEventHandler<HTMLInputElement> = useCallback(
    event => {
      const checked = (event.target as HTMLInputElement).checked
      set(checked)
      updateSetting(checked)
    },
    [],
  )

  return <Checkbox checked={setting} onChange={handleClick} />
}

function DrawRulersControl() {
  const [setting, set] = useState(
    getSetting(state => state.settings.objectSettings.camera.drawRulers),
  )

  const updateSetting = useCallback(setting => {
    setSetting(state => {
      state.settings.objectSettings.camera.drawRulers = setting
    })
  }, [])

  const handleClick: React.FormEventHandler<HTMLInputElement> = useCallback(
    event => {
      const checked = (event.target as HTMLInputElement).checked
      set(checked)
      updateSetting(checked)
    },
    [],
  )

  return <Checkbox checked={setting} onChange={handleClick} />
}

function DrawCalibrationChartControl() {
  const [setting, set] = useState(
    getSetting(
      state => state.settings.objectSettings.camera.drawCalibrationChart,
    ),
  )

  const updateSetting = useCallback(setting => {
    setSetting(state => {
      state.settings.objectSettings.camera.drawCalibrationChart = setting
    })
  }, [])

  const handleClick: React.FormEventHandler<HTMLInputElement> = useCallback(
    event => {
      const checked = (event.target as HTMLInputElement).checked
      set(checked)
      updateSetting(checked)
    },
    [],
  )

  return <Checkbox checked={setting} onChange={handleClick} />
}

function DrawParticlesInVelocityOrientationControl() {
  const [setting, set] = useState(
    getSetting(
      state =>
        state.settings.objectSettings.particles.drawInVelocityOrientation,
    ),
  )

  const updateSetting = useCallback(setting => {
    setSetting(state => {
      state.settings.objectSettings.particles.drawInVelocityOrientation =
        setting
    })
  }, [])

  const handleClick: React.FormEventHandler<HTMLInputElement> = useCallback(
    event => {
      const checked = (event.target as HTMLInputElement).checked
      set(checked)
      updateSetting(checked)
    },
    [],
  )

  return <Checkbox checked={setting} onChange={handleClick} />
}

const enum TABS {
  GENERAL = 'general',
  CAMERA_HELPERS = 'camera-helpers',
  PARTICLES = 'particles',
  LIGHT = 'light',
  LINE = 'line',
  VISUALISATION = 'visualisation',
}

function GeneralTab() {
  return (
    <Composition templateCols="1fr 2fr" gap="1em" alignItems="center">
      Max Speed <MaxSpeedControl />
      Max Transition <TransitionMaxSpeedControl />
      Wait before Start <WaitAtStartDurationControl />
    </Composition>
  )
}

function CameraHelpersTab() {
  return (
    <Composition templateCols="1fr 2fr" gap="1em" alignItems="center">
      Draw Ruler <DrawRulersControl />
      Draw Frustum Alignment <DrawFrustumAlignmentControl />
      Draw Calibration Chart <DrawCalibrationChartControl />
    </Composition>
  )
}

function ParticlesTab() {
  return (
    <Composition templateCols="1fr 2fr" gap="1em" alignItems="center">
      Wait at Particle <ParticleWaitDurationControl />
      Draw in Velocity Orientation <DrawParticlesInVelocityOrientationControl />
    </Composition>
  )
}

function LightTab() {
  return (
    <Composition templateCols="1fr 2fr" gap="1em" alignItems="center">
      Wait at Light <ParticleWaitDurationControl />
      Hide if Black <HideLightIfBlackControl />
    </Composition>
  )
}

function LineTab() {
  return (
    <Composition templateCols="1fr 2fr" gap="1em" alignItems="center">
      Inter-line Optimisations Enabled <InterLineTransitionEnabledControl />
      Inter-line Transition Angle <InterLineTransitionAngleControl />
      Inter-line Transition Distance <InterLineTransitionDistanceControl />
    </Composition>
  )
}

function VisualisationTab() {
  return (
    <Composition templateCols="1fr 2fr" gap="1em" alignItems="center">
      <p>Annotate Draw Order</p> <br />
      <p>Curve Segments</p> <br />
      <p>Orbit Camera</p> <br />
    </Composition>
  )
}

export const PlannerSettingsInterface = () => {
  const [selected, setSelected] = useState(TABS.GENERAL)

  const handleTabChange = useCallback(
    (newTabId: TABS) => {
      setSelected(newTabId)
    },
    [setSelected],
  )

  return (
    <div>
      <Tabs onChange={handleTabChange} selectedTabId={selected}>
        <Tab id={TABS.GENERAL} title="General" panel={<GeneralTab />} />
        <Tab id={TABS.PARTICLES} title="Particles" panel={<ParticlesTab />} />
        <Tab id={TABS.LINE} title="Lines" panel={<LineTab />} />
        <Tab id={TABS.LIGHT} title="GPencil" panel={<LightTab />} />
        <Tab
          id={TABS.CAMERA_HELPERS}
          title="Camera Helpers"
          panel={<CameraHelpersTab />}
        />

        {/* <Tabs.Expander />
        <Tab
          id={TABS.VISUALISATION}
          title="Visualisation"
          panel={<VisualisationTab />}
        /> */}
      </Tabs>
    </div>
  )
}
