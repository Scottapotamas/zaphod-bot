import {
  Alignment,
  Button,
  Card,
  Checkbox as BlueprintCheckbox,
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
  Store,
  useSetting,
  useStore
} from './state'

import { RenderableTree } from './RenderableTree'
import { MaterialEditorInterface } from './MaterialEditorInterface'

import { isCamera } from '../optimiser/camera'
import { StateSelector } from 'zustand'
import { WritableDraft } from 'immer/dist/internal'

interface CheckboxProps {
  selector: StateSelector<Store, boolean | undefined>
  writer: (draft: WritableDraft<Store>, value: boolean) => void
}

// function Checkbox(props: CheckboxProps) {
//   const [localSetting, setLocalSetting] = useState(getSetting(props.selector))

//   const onChangeHandler: React.FormEventHandler<HTMLInputElement> =
//     useCallback(event => {
//       const checked = (event.target as HTMLInputElement).checked
//       setLocalSetting(checked)
//       setSetting(state => {
//         props.writer(state, checked)
//       })
//     }, [props.writer, setLocalSetting])

//   return <BlueprintCheckbox checked={localSetting} onChange={onChangeHandler} />
// }

function Checkbox(props: CheckboxProps) {
  // If the setting is undefined, explicitly start as false instead of undefined.
  const setting = useSetting(props.selector) ?? false

  const onChangeHandler: React.FormEventHandler<HTMLInputElement> =
    useCallback(event => {
      const checked = (event.target as HTMLInputElement).checked
      setSetting(state => {
        props.writer(state, checked)
      })
    }, [props.writer])

  return <BlueprintCheckbox checked={setting} onChange={onChangeHandler} />
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

      const matchCameraAnimation = !checked

      setSetting((state) => {
        // Orbit controls true means match camera animation false.
        state.matchCameraAnimation = matchCameraAnimation
      })

      const orbitControls = getSetting(state => state.orbitControls)
      if (orbitControls) {
        orbitControls.enableRotate = !matchCameraAnimation
        orbitControls.enableZoom = !matchCameraAnimation
      }

      // Search for the camera for this frame if we've disabled orbitControls
      if (matchCameraAnimation) {
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
    <BlueprintCheckbox checked={enabled} onChange={updateChecked} />
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
    <BlueprintCheckbox checked={enabled} onChange={updateChecked} />
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
      Disable Shaped Transitions <Checkbox selector={state => state.settings.optimisation.disableShapedTransitions} writer={(state, value) => state.settings.optimisation.disableShapedTransitions = value} />
    </Composition>
  )
}

function CameraHelpersTab() {
  const cameraDurationOverrideEnabled = useSetting(
    state => state.cameraOverrideDuration > 0,
  )

  return (
    <Composition templateCols="1fr 200px" gap="1em" alignItems="center">
      Draw Ruler <Checkbox selector={state => state.settings.objectSettings.camera.drawRulers} writer={(state, value) => state.settings.objectSettings.camera.drawRulers = value} />
      Draw Frustum Alignment <Checkbox selector={state => state.settings.objectSettings.camera.drawAlignmentHelpers} writer={(state, value) => state.settings.objectSettings.camera.drawAlignmentHelpers = value} />
      Draw Extrinsic Calibrators <Checkbox selector={state => state.settings.objectSettings.camera.drawExtrinsicCalibrators} writer={(state, value) => state.settings.objectSettings.camera.drawExtrinsicCalibrators = value} />
      Draw Col Calibration Chart <Checkbox selector={state => state.settings.objectSettings.camera.drawColorCalibrationChart} writer={(state, value) => state.settings.objectSettings.camera.drawColorCalibrationChart = value} />
      Enable duration override  <Checkbox selector={state => state.cameraOverrideDuration > 0} writer={(state, value) => state.cameraOverrideDuration = value ? 100 : 0} />
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
      Draw in Velocity Orientation <Checkbox selector={state => state.settings.objectSettings.particles.drawInVelocityOrientation} writer={(state, value) => state.settings.objectSettings.particles.drawInVelocityOrientation = value} />
      Hide if Occluded <Checkbox selector={state => state.settings.objectSettings.particles.hideIfOccluded} writer={(state, value) => state.settings.objectSettings.particles.hideIfOccluded = value} />
    </Composition>
  )
}

function LightTab() {
  return (
    <Composition templateCols="1fr 200px" gap="1em" alignItems="center">
      Pre Wait <LightPreWaitDurationControl />
      On Duration <LightOnDurationControl />
      Post Wait <LightPostWaitDurationControl />
      Hide if Black <Checkbox selector={state => state.settings.objectSettings.light.hideIfBlack} writer={(state, value) => state.settings.objectSettings.light.hideIfBlack = value} />
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
      Inter-line Optimisations Enabled <Checkbox selector={state => state.settings.optimisation.smoothInterlineTransitions} writer={(state, value) => state.settings.optimisation.smoothInterlineTransitions = value} />
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
      Preview Progress <Checkbox selector={state => state.visualisationSettings.previewProgress} writer={(state, value) => state.visualisationSettings.previewProgress = value} />
      {previewEnabled ? (
        <>
          Preview Timeline <PreviewTimelineControl />
        </>
      ) : null}
      Annotate Draw Order <AnnotateOrderingToggle />
      Orbit Camera <OrbitCameraToggle />
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
          title="GEO"
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
