import {
  Alignment,
  Button,
  Card,
  Checkbox as BlueprintCheckbox,
  FormGroup,
  NumericInput as BlueprintNumericInput,
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
  changeState,
  getSetting,
  incrementViewportFrameVersion,
  setSetting,
  Store,
  useSetting,
  useStore,
} from './state'

import { RenderableTree } from './RenderableTree'
import { MaterialEditorInterface } from './MaterialEditorInterface'

import { isCamera } from '../optimiser/camera'
import { StateSelector } from 'zustand'
import { WritableDraft } from 'immer/dist/internal'

// TODO: How do we get the defaults from here?
// Bool defaults are false, number defaults are 0

interface CheckboxProps {
  selector: StateSelector<Store, boolean | undefined>
  writer: (draft: WritableDraft<Store>, value: boolean) => void
  label: string
}

function Checkbox(props: CheckboxProps) {
  if (useSetting(props.selector) === undefined) {
    console.warn(`Checkbox setting started undefined`, props.selector)
  }

  // If the setting is undefined, explicitly start as false instead of undefined.
  const setting = useSetting(props.selector) ?? false

  const onChangeHandler: React.FormEventHandler<HTMLInputElement> = useCallback(
    event => {
      const checked = (event.target as HTMLInputElement).checked
      setSetting(state => {
        props.writer(state, checked)
      })
    },
    [props.writer],
  )

  return (
    <>
      {props.label} <BlueprintCheckbox checked={setting} onChange={onChangeHandler} />
    </>
  )
}

interface NumericInputProps {
  selector: StateSelector<Store, number | undefined>
  writer: (draft: WritableDraft<Store>, value: number) => void
  min: number
  max: number
  stepSize: number
  minorStepSize?: number
  majorStepSize?: number
  rightText?: string
  label: string
}

function NumericInput(props: NumericInputProps) {
  if (useSetting(props.selector) === undefined) {
    console.warn(`NumericInput setting started undefined`, props.selector)
  }

  // If the setting is undefined, explicitly start as 0 instead of undefined.
  const setting = useSetting(props.selector) ?? 0

  const handleOnValueChange = useCallback(
    value => {
      setSetting(state => {
        props.writer(state, value)
      })
    },
    [props.writer],
  )

  return (
    <>
      {props.label}{' '}
      <BlueprintNumericInput
        fill
        min={props.min}
        max={props.max}
        stepSize={props.stepSize}
        minorStepSize={props.minorStepSize}
        majorStepSize={props.majorStepSize}
        value={setting}
        onValueChange={handleOnValueChange}
        rightElement={props.rightText ? <Tag>{props.rightText}</Tag> : undefined}
        style={{ width: '100%' }}
        intent={Intent.PRIMARY}
      />
    </>
  )
}

// TODO: Dropdown component for this
// function GPencilOutputType() {
//   const [setting, set] = useState(
//     getSetting(state => state.settings.objectSettings.gpencil.outputType) ?? 0,
//   )

//   const update = useCallback(t => {
//     setSetting(state => {
//       state.settings.objectSettings.gpencil.outputType = t
//     })
//   }, [])

//   const handleUpdate = useCallback(t => {
//     set(t)
//     update(t)
//   }, [])

//   return (
//     <BlueprintNumericInput
//       fill
//       min={0}
//       max={2}
//       stepSize={1}
//       majorStepSize={1}
//       value={setting}
//       onValueChange={handleUpdate}
//       rightElement={<Tag>type</Tag>}
//       style={{ width: '100%' }}
//       intent={Intent.PRIMARY}
//     />
//   )
// }

function PreviewTimelineControl() {
  const [setting, set] = useState(getSetting(state => state.visualisationSettings.frameProgress))

  const updatePreviewTimeline = useCallback(factor => {
    set(factor)

    changeState(state => {
      state.visualisationSettings.frameProgress = factor

      // Trigger an update, wrap around
      incrementViewportFrameVersion(state)
    })
  }, [])

  const duration = useSetting(state => state.estimatedDurationByFrame[state.viewportFrame] ?? 0)

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
  const renderablesForFrame = getSetting(state => state.renderablesByFrame[state.viewportFrame]) ?? []

  const blenderCamera = renderablesForFrame.find(isCamera)

  if (sceneCamera && blenderCamera) {
    blenderCamera.alignCamera(sceneCamera)
  }
}

function OrbitCameraToggle() {
  const [enabled, setEnabled] = useState(true)

  const updateChecked: React.FormEventHandler<HTMLInputElement> = useCallback(event => {
    const checked = event.currentTarget.checked
    setEnabled(checked)

    const matchCameraAnimation = !checked

    setSetting(state => {
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
  }, [])

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

  return <BlueprintCheckbox checked={enabled} onChange={updateChecked} />
}

function AnnotateOrderingToggle() {
  const [enabled, setEnabled] = useState(getSetting(state => state.visualisationSettings.annotateDrawOrder))

  const updateChecked: React.FormEventHandler<HTMLInputElement> = useCallback(event => {
    const checked = event.currentTarget.checked
    setEnabled(checked)

    setSetting(state => {
      state.visualisationSettings.annotateDrawOrder = checked
      incrementViewportFrameVersion(state)
    })
  }, [])

  return <BlueprintCheckbox checked={enabled} onChange={updateChecked} />
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
      <NumericInput
        label="Max Speed"
        min={0}
        max={500}
        stepSize={25}
        majorStepSize={100}
        rightText="mm/s"
        selector={state => state.settings.optimisation.maxSpeed}
        writer={(state, value) => (state.settings.optimisation.maxSpeed = value)}
      />
      <NumericInput
        label="Transition Size"
        min={0}
        max={0.5}
        stepSize={0.01}
        minorStepSize={0.001}
        majorStepSize={0.1}
        selector={state => state.settings.optimisation.transitionSize}
        writer={(state, value) => (state.settings.optimisation.transitionSize = value)}
      />
      <NumericInput
        label="Wait before Start"
        min={0}
        max={5000}
        stepSize={100}
        majorStepSize={1000}
        rightText="ms"
        selector={state => state.settings.optimisation.waitAtStartDuration}
        writer={(state, value) => (state.settings.optimisation.waitAtStartDuration = value)}
      />
      <NumericInput
        label="LightFade Offset"
        min={-1000}
        max={1000}
        stepSize={1}
        majorStepSize={10}
        selector={state => state.settings.lightFadeOffset}
        writer={(state, value) => (state.settings.lightFadeOffset = value)}
      />
      <NumericInput
        label="Global Brightness"
        min={0}
        max={1}
        stepSize={0.01}
        majorStepSize={0.1}
        selector={state => state.settings.brightness}
        writer={(state, value) => (state.settings.brightness = value)}
      />
      <Checkbox
        label="Disable Shaped Transitions"
        selector={state => state.settings.optimisation.disableShapedTransitions}
        writer={(state, value) => (state.settings.optimisation.disableShapedTransitions = value)}
      />
    </Composition>
  )
}

function CameraHelpersTab() {
  const cameraDurationOverrideEnabled = useSetting(state => state.cameraOverrideDuration > 0)

  return (
    <Composition templateCols="1fr 200px" gap="1em" alignItems="center">
      <Checkbox
        label="Draw Ruler"
        selector={state => state.settings.objectSettings.camera.drawRulers}
        writer={(state, value) => (state.settings.objectSettings.camera.drawRulers = value)}
      />
      <Checkbox
        label="Draw Frustum Alignment"
        selector={state => state.settings.objectSettings.camera.drawAlignmentHelpers}
        writer={(state, value) => (state.settings.objectSettings.camera.drawAlignmentHelpers = value)}
      />
      <Checkbox
        label="Draw Extrinsic Calibrators"
        selector={state => state.settings.objectSettings.camera.drawExtrinsicCalibrators}
        writer={(state, value) => (state.settings.objectSettings.camera.drawExtrinsicCalibrators = value)}
      />
      <Checkbox
        label="Draw Col Calibration Chart"
        selector={state => state.settings.objectSettings.camera.drawColorCalibrationChart}
        writer={(state, value) => (state.settings.objectSettings.camera.drawColorCalibrationChart = value)}
      />
      <Checkbox
        label="Enable duration override "
        selector={state => state.cameraOverrideDuration > 0}
        writer={(state, value) => (state.cameraOverrideDuration = value ? 10_000 : 0)}
      />
      {cameraDurationOverrideEnabled ? (
        <>
          <NumericInput
            label="Camera Duration"
            min={1}
            max={30_000}
            stepSize={1_000}
            majorStepSize={5_000}
            rightText="ms"
            selector={state => state.cameraOverrideDuration}
            writer={(state, value) => (state.cameraOverrideDuration = value)}
          />
        </>
      ) : null}
    </Composition>
  )
}

function ParticlesTab() {
  return (
    <Composition templateCols="1fr 200px" gap="1em" alignItems="center">
      <NumericInput
        label="Pre Wait"
        min={0}
        max={200}
        stepSize={25}
        majorStepSize={100}
        rightText="ms"
        selector={state => state.settings.objectSettings.particles.preWait}
        writer={(state, value) => (state.settings.objectSettings.particles.preWait = value)}
      />
      <NumericInput
        label="On Duration"
        min={0}
        max={200}
        stepSize={25}
        majorStepSize={100}
        rightText="ms"
        selector={state => state.settings.objectSettings.particles.onDuration}
        writer={(state, value) => (state.settings.objectSettings.particles.onDuration = value)}
      />
      <NumericInput
        label="Post Wait"
        min={0}
        max={200}
        stepSize={25}
        majorStepSize={100}
        rightText="ms"
        selector={state => state.settings.objectSettings.particles.postWait}
        writer={(state, value) => (state.settings.objectSettings.particles.postWait = value)}
      />
      <Checkbox
        label="Draw in Velocity Orientation"
        selector={state => state.settings.objectSettings.particles.drawInVelocityOrientation}
        writer={(state, value) => (state.settings.objectSettings.particles.drawInVelocityOrientation = value)}
      />
      <Checkbox
        label="Hide if Occluded"
        selector={state => state.settings.objectSettings.particles.hideIfOccluded}
        writer={(state, value) => (state.settings.objectSettings.particles.hideIfOccluded = value)}
      />
    </Composition>
  )
}

function LightTab() {
  return (
    <Composition templateCols="1fr 200px" gap="1em" alignItems="center">
      <NumericInput
        label="Pre Wait"
        min={0}
        max={200}
        stepSize={25}
        majorStepSize={100}
        rightText="ms"
        selector={state => state.settings.objectSettings.light.preWait}
        writer={(state, value) => (state.settings.objectSettings.light.preWait = value)}
      />
      <NumericInput
        label="On Duration"
        min={0}
        max={200}
        stepSize={25}
        majorStepSize={100}
        rightText="ms"
        selector={state => state.settings.objectSettings.light.onDuration}
        writer={(state, value) => (state.settings.objectSettings.light.onDuration = value)}
      />
      <NumericInput
        label="Post Wait"
        min={0}
        max={200}
        stepSize={25}
        majorStepSize={100}
        rightText="ms"
        selector={state => state.settings.objectSettings.light.postWait}
        writer={(state, value) => (state.settings.objectSettings.light.postWait = value)}
      />
      <Checkbox
        label="Hide if Black"
        selector={state => state.settings.objectSettings.light.hideIfBlack}
        writer={(state, value) => (state.settings.objectSettings.light.hideIfBlack = value)}
      />
    </Composition>
  )
}

function LineTab() {
  const interlineOptimisationsEnabled = useSetting(state => state.settings.optimisation.smoothInterlineTransitions)

  return (
    <Composition templateCols="1fr 200px" gap="1em" alignItems="center">
      <NumericInput
        label="GPencil simplification tolerance"
        min={0}
        max={10}
        stepSize={0.5}
        minorStepSize={0.1}
        selector={state => state.settings.objectSettings.gpencil.simplificationTolerance}
        writer={(state, value) => (state.settings.objectSettings.gpencil.simplificationTolerance = value)}
      />

      {/* GPencil Output type <GPencilOutputType /> */}
      <Checkbox
        label="Inter-line Optimisations Enabled"
        selector={state => state.settings.optimisation.smoothInterlineTransitions}
        writer={(state, value) => (state.settings.optimisation.smoothInterlineTransitions = value)}
      />
      {interlineOptimisationsEnabled ? (
        <>
          <NumericInput
            label="Inter-line Transition Angle"
            min={0}
            max={360}
            stepSize={1}
            majorStepSize={36}
            rightText="°"
            selector={state => state.settings.optimisation.interLineTransitionAngle}
            writer={(state, value) => (state.settings.optimisation.interLineTransitionAngle = value)}
          />
          <NumericInput
            label="Inter-line Transition Distance"
            min={1}
            max={10}
            stepSize={1}
            majorStepSize={5}
            rightText="mm"
            selector={state => state.settings.optimisation.interLineTransitionShaveDistance}
            writer={(state, value) => (state.settings.optimisation.interLineTransitionShaveDistance = value)}
          />
        </>
      ) : null}

      <NumericInput
        label="Line run up"
        min={0}
        max={100}
        stepSize={1}
        rightText="%"
        selector={state => state.settings.optimisation.lineRunUp * 100}
        writer={(state, value) => (state.settings.optimisation.lineRunUp = value / 100)}
      />
    </Composition>
  )
}

function VisualisationTab() {
  const previewEnabled = useSetting(state => state.visualisationSettings.previewProgress)

  return (
    <Composition templateCols="1fr 200px" gap="1em" alignItems="center">
      <Checkbox
        label="Preview Progress"
        selector={state => state.visualisationSettings.previewProgress}
        writer={(state, value) => (state.visualisationSettings.previewProgress = value)}
      />
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
        <Tab id={TABS.CAMERA_HELPERS} title="CAM" panel={<CameraHelpersTab />} />

        <Tab id={TABS.VISUALISATION} title="VIS" panel={<VisualisationTab />} />
      </Tabs>
    </div>
  )
}
