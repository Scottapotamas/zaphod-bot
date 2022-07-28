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
  Intent,
} from '@blueprintjs/core'
import { IconNames } from '@blueprintjs/icons'

import { Composition, Box } from 'atomic-layout'

import React, { useCallback, useState, useEffect } from 'react'

import {
  getSetting,
  incrementViewportFrameVersion,
  setSetting,
  useSetting,
  useStore
} from './state'

import { RenderableTree } from './RenderableTree'
import { MaterialEditorInterface } from './MaterialEditorInterface'

import { isCamera } from '../optimiser/camera'

function DisableShapedTransitionsControl() {
  const [allowed, setAllow] = useState(
    getSetting(state => state.settings.optimisation.disableShapedTransitions),
  )

  const updateAllowed = useCallback(allowed => {
    setSetting(state => {
      state.settings.optimisation.disableShapedTransitions = allowed
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
      intent={Intent.PRIMARY}
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
      intent={Intent.PRIMARY}
    />
  )
}

function GPencilOutputType() {
  const [setting, set] = useState(
    getSetting(state => state.settings.objectSettings.gpencil.outputType) ?? 0,
  )

  const update = useCallback(t => {
    setSetting(state => {
      state.settings.objectSettings.gpencil.outputType = t
    })
  }, [])

  const handleUpdate = useCallback(t => {
    set(t)
    update(t)
  }, [])

  return (
    <NumericInput
      fill
      min={0}
      max={2}
      stepSize={1}
      majorStepSize={1}
      value={setting}
      onValueChange={handleUpdate}
      rightElement={<Tag>type</Tag>}
      style={{ width: '100%' }}
      intent={Intent.PRIMARY}
    />
  )
}

function GPencilSimplificationControl() {
  const [distange, setDistance] = useState(
    getSetting(
      state => state.settings.objectSettings.gpencil.simplificationTolerance,
    ),
  )

  const updateDistance = useCallback(distance => {
    setSetting(state => {
      state.settings.objectSettings.gpencil.simplificationTolerance = distance
    })
  }, [])

  const setAndUpdateDistance = useCallback(distance => {
    setDistance(distance)
    updateDistance(distance)
  }, [])

  return (
    <NumericInput
      fill
      min={0}
      max={10}
      stepSize={0.5}
      minorStepSize={0.1}
      value={distange}
      onValueChange={setAndUpdateDistance}
      style={{ width: '100%' }}
      intent={Intent.PRIMARY}
    />
  )
}

function LineRunUpDistanceControl() {
  const [distange, setDistance] = useState(
    getSetting(state => state.settings.optimisation.lineRunUp) * 100,
  )

  const updateDistance = useCallback(distance => {
    setSetting(state => {
      state.settings.optimisation.lineRunUp = distance
    })
  }, [])

  const setAndUpdateDistance = useCallback(distance => {
    setDistance(distance)
    updateDistance(distance / 100)
  }, [])

  return (
    <NumericInput
      fill
      min={0}
      max={100}
      stepSize={1}
      value={distange}
      onValueChange={setAndUpdateDistance}
      rightElement={<Tag>%</Tag>}
      style={{ width: '100%' }}
      intent={Intent.PRIMARY}
    />
  )
}

function ParticlePreWaitDurationControl() {
  const [setting, set] = useState(
    getSetting(state => state.settings.objectSettings.particles.preWait) ?? 0,
  )

  const update = useCallback(delay => {
    setSetting(state => {
      state.settings.objectSettings.particles.preWait = delay
    })
  }, [])

  const handleUpdate = useCallback(delay => {
    set(delay)
    update(delay)
  }, [])

  return (
    <NumericInput
      fill
      min={0}
      max={200}
      stepSize={25}
      majorStepSize={100}
      value={setting}
      onValueChange={handleUpdate}
      rightElement={<Tag>ms</Tag>}
      style={{ width: '100%' }}
      intent={Intent.PRIMARY}
    />
  )
}

function ParticlePostWaitDurationControl() {
  const [setting, set] = useState(
    getSetting(state => state.settings.objectSettings.particles.postWait) ?? 0,
  )

  const update = useCallback(delay => {
    setSetting(state => {
      state.settings.objectSettings.particles.postWait = delay
    })
  }, [])

  const handleUpdate = useCallback(delay => {
    set(delay)
    update(delay)
  }, [])

  return (
    <NumericInput
      fill
      min={0}
      max={200}
      stepSize={25}
      majorStepSize={100}
      value={setting}
      onValueChange={handleUpdate}
      rightElement={<Tag>ms</Tag>}
      style={{ width: '100%' }}
      intent={Intent.PRIMARY}
    />
  )
}

function ParticleOnDurationControl() {
  const [setting, set] = useState(
    getSetting(state => state.settings.objectSettings.particles.onDuration) ??
      1,
  )

  const update = useCallback(delay => {
    setSetting(state => {
      state.settings.objectSettings.particles.onDuration = delay
    })
  }, [])

  const handleUpdate = useCallback(delay => {
    set(delay)
    update(delay)
  }, [])

  return (
    <NumericInput
      fill
      min={1}
      max={200}
      stepSize={25}
      majorStepSize={100}
      value={setting}
      onValueChange={handleUpdate}
      rightElement={<Tag>ms</Tag>}
      style={{ width: '100%' }}
      intent={Intent.PRIMARY}
    />
  )
}

function LightPreWaitDurationControl() {
  const [setting, set] = useState(
    getSetting(state => state.settings.objectSettings.light.preWait) ?? 0,
  )

  const update = useCallback(delay => {
    setSetting(state => {
      state.settings.objectSettings.light.preWait = delay
    })
  }, [])

  const handleUpdate = useCallback(delay => {
    set(delay)
    update(delay)
  }, [])

  return (
    <NumericInput
      fill
      min={0}
      max={200}
      stepSize={25}
      majorStepSize={100}
      value={setting}
      onValueChange={handleUpdate}
      rightElement={<Tag>ms</Tag>}
      style={{ width: '100%' }}
      intent={Intent.PRIMARY}
    />
  )
}

function LightPostWaitDurationControl() {
  const [setting, set] = useState(
    getSetting(state => state.settings.objectSettings.light.postWait) ?? 0,
  )

  const update = useCallback(delay => {
    setSetting(state => {
      state.settings.objectSettings.light.postWait = delay
    })
  }, [])

  const handleUpdate = useCallback(delay => {
    set(delay)
    update(delay)
  }, [])

  return (
    <NumericInput
      fill
      min={0}
      max={200}
      stepSize={25}
      majorStepSize={100}
      value={setting}
      onValueChange={handleUpdate}
      rightElement={<Tag>ms</Tag>}
      style={{ width: '100%' }}
      intent={Intent.PRIMARY}
    />
  )
}

function LightOnDurationControl() {
  const [setting, set] = useState(
    getSetting(state => state.settings.objectSettings.light.onDuration) ?? 1,
  )

  const update = useCallback(delay => {
    setSetting(state => {
      state.settings.objectSettings.light.onDuration = delay
    })
  }, [])

  const handleUpdate = useCallback(delay => {
    set(delay)
    update(delay)
  }, [])

  return (
    <NumericInput
      fill
      min={1}
      max={200}
      stepSize={25}
      majorStepSize={100}
      value={setting}
      onValueChange={handleUpdate}
      rightElement={<Tag>ms</Tag>}
      style={{ width: '100%' }}
      intent={Intent.PRIMARY}
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
      max={500}
      stepSize={25}
      majorStepSize={100}
      value={localSpeed}
      onValueChange={setAndUpdateMaxSpeed}
      rightElement={<Tag>mm/sec</Tag>}
      style={{ width: '100%' }}
      intent={Intent.PRIMARY}
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
      intent={Intent.PRIMARY}
    />
  )
}

function TransitionSizeControl() {
  const [localSpeed, setLocalSpeed] = useState(
    getSetting(state => state.settings.optimisation.transitionSize),
  )

  const updateTransitionMaxSpeed = useCallback(newTransitionMaxSpeed => {
    setSetting(state => {
      state.settings.optimisation.transitionSize = Math.max(
        newTransitionMaxSpeed,
        0,
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
      max={0.5}
      stepSize={0.01}
      minorStepSize={0.001}
      majorStepSize={0.1}
      value={localSpeed}
      onValueChange={setAndUpdateTransitionMaxSpeed}
      rightElement={<Tag>mm</Tag>}
      style={{ width: '100%' }}
      intent={Intent.PRIMARY}
    />
  )
}

function LightFadeOffsetControl() {
  const [setting, set] = useState(
    getSetting(state => state.settings.lightFadeOffset),
  )

  const update = useCallback(val => {
    setSetting(state => {
      state.settings.lightFadeOffset = val
    })
  }, [])

  const setAndUpdate = useCallback(val => {
    set(val)
    update(val)
  }, [])

  return (
    <NumericInput
      fill
      min={-1000}
      max={1000}
      stepSize={1}
      majorStepSize={10}
      value={setting}
      onValueChange={setAndUpdate}
      rightElement={<Tag>ms</Tag>}
      style={{ width: '100%' }}
      intent={Intent.PRIMARY}
    />
  )
}

function CameraDurationControl() {
  const [setting, set] = useState(
    getSetting(state => state.cameraOverrideDuration),
  )

  const update = useCallback(newTransitionMaxSpeed => {
    setSetting(state => {
      state.cameraOverrideDuration = Math.max(newTransitionMaxSpeed, 1)
    })
  }, [])

  const setAndUpdateTransitionMaxSpeed = useCallback(newTransitionMaxSpeed => {
    set(newTransitionMaxSpeed)
    update(newTransitionMaxSpeed)
  }, [])

  return (
    <NumericInput
      fill
      min={1}
      max={30000}
      stepSize={25}
      majorStepSize={100}
      value={setting}
      onValueChange={setAndUpdateTransitionMaxSpeed}
      rightElement={<Tag>ms</Tag>}
      style={{ width: '100%' }}
      intent={Intent.PRIMARY}
    />
  )
}

function CameraDurationOverrideControl() {
  const [setting, set] = useState(
    getSetting(state => state.cameraOverrideDuration > 0),
  )

  const updateSetting = useCallback(setting => {
    setSetting(state => {
      state.cameraOverrideDuration = setting ? 100 : 0
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
      intent={Intent.PRIMARY}
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

function HideParticleIfOccludedControl() {
  const [setting, set] = useState(
    getSetting(state => state.settings.objectSettings.particles.hideIfOccluded),
  )

  const updateSetting = useCallback(setting => {
    setSetting(state => {
      state.settings.objectSettings.particles.hideIfOccluded = setting
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
      state => state.settings.objectSettings.camera.drawColorCalibrationChart,
    ),
  )

  const updateSetting = useCallback(setting => {
    setSetting(state => {
      state.settings.objectSettings.camera.drawColorCalibrationChart = setting
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

function DrawExtrinsicCalibrationHelpersControl() {
  const [setting, set] = useState(
    getSetting(
      state => state.settings.objectSettings.camera.drawExtrinsicCalibrators,
    ),
  )

  const updateSetting = useCallback(setting => {
    setSetting(state => {
      state.settings.objectSettings.camera.drawExtrinsicCalibrators = setting
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

function PreviewProgressControl() {
  const [setting, set] = useState(
    getSetting(state => state.visualisationSettings.previewProgress),
  )

  const updateSetting = useCallback(setting => {
    setSetting(state => {
      state.visualisationSettings.previewProgress = setting
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

function PreviewTimelineControl() {
  const [setting, set] = useState(
    getSetting(state => state.visualisationSettings.frameProgress),
  )

  const updatePreviewTimeline = useCallback(factor => {
    set(factor)

    setSetting(state => {
      state.visualisationSettings.frameProgress = factor

      // Trigger an update, wrap around
      incrementViewportFrameVersion(state)
    })
  }, [])

  const duration = useSetting(
    state => state.estimatedDurationByFrame[state.viewportFrame] ?? 0,
  )

  return (
    <div style={{ marginLeft: 10, marginRight: 10 }}>
      <Slider
        min={0}
        max={1}
        value={setting}
        labelRenderer={val => {
          if (val === 0) return 'Start'
          if (val === 1) return 'End'

          return `${Math.round(((val * duration) / 1000) * 10) / 10}s`
        }}
        labelStepSize={1}
        onChange={updatePreviewTimeline}
        stepSize={0.001}
      />
    </div>
  )
}

function setThreeJSCamera() {
  const sceneCamera = getSetting(state => state.camera)
  const renderablesForFrame =
    getSetting(state => state.renderablesByFrame[state.viewportFrame]) ?? []

  const blenderCamera = renderablesForFrame.find(isCamera)

  if (sceneCamera && blenderCamera) {
    blenderCamera.alignCamera(sceneCamera)
  }
}

function OrbitCameraToggle() {
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
      if (getSetting(state => state.matchCameraAnimation)) {
        setThreeJSCamera()
      }
    },
    [],
  )

  useEffect(() => {
    return useStore.subscribe(
      state => state.viewportFrame,
      frameNumber => {
        if (getSetting(state => state.matchCameraAnimation)) {
          setThreeJSCamera()
        }
      },
    )
  })

  return (
    <Checkbox checked={enabled} onChange={updateChecked} />
  )
}

function AnnotateOrderingToggle() {
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
    <Checkbox checked={enabled} onChange={updateChecked} />
  )
}

const enum TABS {
  GENERAL = 'general',
  CAMERA_HELPERS = 'camera-helpers',
  PARTICLES = 'particles',
  LIGHT = 'light',
  LINE = 'line',
  GPENCIL = 'gpencil',
  VISUALISATION = 'visualisation',
  GEOMETRY = 'geometry',
}

function GeneralTab() {
  return (
    <Composition templateCols="1fr 200px" gap="1em" alignItems="center">
      Max Speed <MaxSpeedControl />
      Max Transition <TransitionMaxSpeedControl />
      TransitionSize <TransitionSizeControl />
      Wait before Start <WaitAtStartDurationControl />
      LightFade Offset <LightFadeOffsetControl />
      Disable Shaped Transitions <DisableShapedTransitionsControl />
    </Composition>
  )
}

function CameraHelpersTab() {
  const cameraDurationOverrideEnabled = useSetting(
    state => state.cameraOverrideDuration > 0,
  )

  return (
    <Composition templateCols="1fr 200px" gap="1em" alignItems="center">
      Draw Ruler <DrawRulersControl />
      Draw Frustum Alignment <DrawFrustumAlignmentControl />
      Draw Extrinsic Calibrators <DrawExtrinsicCalibrationHelpersControl />
      Draw Calibration Chart <DrawCalibrationChartControl />
      Enable duration override <CameraDurationOverrideControl />
      {cameraDurationOverrideEnabled ? (
        <>
          {' '}
          Camera duration <CameraDurationControl />
        </>
      ) : null}
    </Composition>
  )
}

function ParticlesTab() {
  return (
    <Composition templateCols="1fr 200px" gap="1em" alignItems="center">
      Pre Wait <ParticlePreWaitDurationControl />
      On Duration <ParticleOnDurationControl />
      Post Wait <ParticlePostWaitDurationControl />
      Draw in Velocity Orientation <DrawParticlesInVelocityOrientationControl />
      Hide if Occluded <HideParticleIfOccludedControl />
    </Composition>
  )
}

function LightTab() {
  return (
    <Composition templateCols="1fr 200px" gap="1em" alignItems="center">
      Pre Wait <LightPreWaitDurationControl />
      On Duration <LightOnDurationControl />
      Post Wait <LightPostWaitDurationControl />
      Hide if Black <HideLightIfBlackControl />
    </Composition>
  )
}

function LineTab() {
  const interlineOptimisationsEnabled = useSetting(
    state => state.settings.optimisation.smoothInterlineTransitions,
  )

  return (
    <Composition templateCols="1fr 200px" gap="1em" alignItems="center">
      GPencil simplification <GPencilSimplificationControl />
      GPencil Output type <GPencilOutputType />
      Inter-line Optimisations Enabled <InterLineTransitionEnabledControl />
      {interlineOptimisationsEnabled ? (
        <>
          Inter-line Transition Angle <InterLineTransitionAngleControl />
          Inter-line Transition Distance <InterLineTransitionDistanceControl />
        </>
      ) : null}
      Line run up <LineRunUpDistanceControl />
    </Composition>
  )
}

function VisualisationTab() {
  const previewEnabled = useSetting(
    state => state.visualisationSettings.previewProgress,
  )

  return (
    <Composition templateCols="1fr 200px" gap="1em" alignItems="center">
      Preview Progress <PreviewProgressControl />
      {previewEnabled ? (
        <>
          Preview Timeline <PreviewTimelineControl />
        </>
      ) : null}
      Annotate Draw Order <AnnotateOrderingToggle/>
      Orbit Camera <OrbitCameraToggle/>
      {/* <p>Curve Segments</p> <br /> */}

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
        <Tab id={TABS.GENERAL} title="GENERAL" panel={<GeneralTab />} />
        <Tab id={TABS.PARTICLES} title="PARTICLES" panel={<ParticlesTab />} />
        <Tab id={TABS.LIGHT} title="LIGHTS" panel={<LightTab />} />
        <Tab id={TABS.LINE} title="LINES" panel={<LineTab />} />
        <Tabs.Expander />
        <Tab
          id={TABS.GEOMETRY}
          title="GEOMETRY"
          panel={
            <div>
              <RenderableTree />
              <MaterialEditorInterface />
            </div>
          }
        />
        <Tab
          id={TABS.CAMERA_HELPERS}
          title="CAM"
          panel={<CameraHelpersTab />}
        />

        <Tab
          id={TABS.VISUALISATION}
          title="VIS"
          panel={<VisualisationTab />}
        />
      </Tabs>
    </div>
  )
}
