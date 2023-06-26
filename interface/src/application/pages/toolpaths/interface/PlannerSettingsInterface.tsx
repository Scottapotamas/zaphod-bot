import {
  Alignment,
  Checkbox as BlueprintCheckbox,
  NumericInput as BlueprintNumericInput,
  Slider,
  Tag,
  Tab,
  Tabs,
  Intent,
  Icon,
} from '@blueprintjs/core'
import { IconName, IconNames } from '@blueprintjs/icons'

import { Tooltip2 } from '@blueprintjs/popover2'

import { Composition } from 'atomic-layout'

import React, { useCallback, useState, useEffect, useRef, useMemo } from 'react'

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

import './styles.css'
import { DMXControlTab } from './DMXControl'
import { getOrderedMovementsForFrame } from './ToolpathVisualisation'
import { sparseToDense } from '../optimiser/passes'
import { Movement } from '../optimiser/movements'

// TODO: How do we get the defaults from here?
// Bool defaults are false, number defaults are 0

interface CheckboxProps {
  selector: StateSelector<Store, boolean | undefined>
  writer: (draft: WritableDraft<Store>, value: boolean) => void
  label: string
  description?: string
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
    // Override the column span since we handle our own label
    <div style={{ gridColumn: 'span 2' }}>
      <Tooltip2
        content={props.description}
        disabled={!Boolean(props.description)}
        intent={Intent.PRIMARY}
        usePortal={false} // Force the rendering of the tooltip to be inline so focus bubbling works correctly
        placement="right"
        popoverClassName="setting-tooltip" // Use width: max-content to make sure the text expands rather than contracts to a couple words of width
        renderTarget={({ ref, isOpen, ...tooltipProps }) => (
          <div {...tooltipProps} ref={ref} className="planner-checkbox">
            {/* Attach to a div surrounding the NumericInput, since the NumericInput is complex and screws with the refs */}
            <BlueprintCheckbox
              checked={setting}
              onChange={onChangeHandler}
              alignIndicator={Alignment.RIGHT}
              label={props.label}
            />
          </div>
        )}
      />
    </div>
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
  description?: string
}

function isValidNumber(num: number) {
  return !isNaN(num)
}

function isNumberInRange(num: number, min: number, max: number) {
  if (num < min) return false
  if (num > max) return false
  return true
}

function stringToNumber(str: string) {
  const trimmed = str.trim()

  if (trimmed === '') {
    return NaN
  }

  return Number(trimmed)
}

function NumericInput(props: NumericInputProps) {
  if (useSetting(props.selector) === undefined) {
    console.warn(`NumericInput setting started undefined`, props.selector)
  }

  const [internalState, setInternalState] = useState(String(getSetting(props.selector) ?? 0))
  const [isNumber, setIsNumber] = useState(true) // it's a valid number by default
  const [inRange, setInRange] = useState(true) // in range by default
  const [errorElement, setErrorElement] = useState(<></>)
  const [hasError, setHasError] = useState(false)

  const validate = useCallback(
    (valueAsString: string) => {
      const value = stringToNumber(valueAsString)
      const valid = isValidNumber(value)
      const withinRange = valid ? isNumberInRange(value, props.min, props.max) : false

      if (valid) {
        if (!withinRange) {
          setErrorElement(
            <>
              {valueAsString} is not within the range of (<b>{props.min}</b> to <b>{props.max}</b>
              {props.rightText ? ` ${props.rightText}` : ''})
            </>,
          )
          setHasError(true)
        } else {
          // setErrorText('') // Don't remove the last error, it just animates away when we hide
          setHasError(false)
        }
      } else {
        if (valueAsString.trim() === '') {
          // valid n
          setErrorElement(<>Empty values are not allowed</>)
          setHasError(true)
        } else {
          setErrorElement(<>"{valueAsString}" is not a valid number</>)
          setHasError(true)
        }
      }

      setIsNumber(valid)
      setInRange(withinRange)

      return valid && withinRange
    },
    [setErrorElement, setHasError],
  )

  const handleOnValueChange = useCallback(
    (value, valueAsString) => {
      setInternalState(valueAsString)
      validate(valueAsString)
    },
    [props.writer, setIsNumber, validate],
  )

  const handleConfirm = useCallback(
    (valueAsString: string) => {
      const validated = validate(valueAsString)

      if (!validated) {
        // Don't send if outside of valid range
        return
      }

      setSetting(state => {
        props.writer(state, stringToNumber(valueAsString))
      })

      // Trim the string if we're confirming
      setInternalState(valueAsString.trim())
    },
    [props.writer, validate],
  )

  const handleKeyDown = useCallback(
    (e: React.KeyboardEvent<HTMLInputElement>) => {
      if (e.key === 'Enter') {
        handleConfirm((e.target as HTMLInputElement).value)
      }
    },
    [handleConfirm],
  )

  const [isFocused, setIsFocused] = useState(false)

  const handleFocus = useCallback(
    (e: React.FocusEvent<HTMLInputElement>) => {
      setIsFocused(true)
    },
    [setIsFocused],
  )

  const handleBlur = useCallback(
    (e: React.FocusEvent<HTMLInputElement>) => {
      handleConfirm((e.target as HTMLInputElement).value)
      setIsFocused(false)
    },
    [handleConfirm, setIsFocused],
  )

  const handleOnButtonClick = useCallback(
    (valueAsNumber: number, valueAsString: string) => {
      setInternalState(valueAsString)
      handleConfirm(valueAsString)
    },
    [handleConfirm],
  )

  return (
    <>
      {props.label}
      <div>
        {/* We render a div here so the grid layout doesn't break when the tooltip is injected */}
        <Tooltip2
          content={
            hasError ? (
              errorElement
            ) : (
              <>
                {props.description} (<b>{props.min}</b> to <b>{props.max}</b>
                {props.rightText ? ` ${props.rightText}` : ''})
              </>
            )
          }
          intent={isNumber ? (inRange ? Intent.PRIMARY : Intent.WARNING) : Intent.DANGER}
          isOpen={hasError || (isFocused && Boolean(props.description))} // Open if there's a description and we're focused or there's an error
          usePortal={false} // Force the rendering of the tooltip to be inline so focus bubbling works correctly
          placement="right"
          popoverClassName="setting-tooltip" // Use width: max-content to make sure the text expands rather than contracts to a couple words of width
          renderTarget={({ ref, isOpen, ...tooltipProps }) => (
            <div {...tooltipProps} ref={ref}>
              {/* Attach to a div surrounding the NumericInput, since the NumericInput is complex and screws with the refs */}
              <BlueprintNumericInput
                fill
                min={props.min}
                max={props.max}
                stepSize={props.stepSize}
                minorStepSize={props.minorStepSize}
                majorStepSize={props.majorStepSize}
                value={String(internalState)}
                onFocus={handleFocus}
                onBlur={handleBlur}
                onKeyDown={handleKeyDown}
                onValueChange={handleOnValueChange}
                onButtonClick={handleOnButtonClick}
                onChange={console.log}
                rightElement={props.rightText ? <Tag>{props.rightText}</Tag> : undefined}
                style={{ width: '100%' }}
                intent={isNumber ? (inRange ? Intent.PRIMARY : Intent.WARNING) : Intent.DANGER}
                allowNumericCharactersOnly={false} // We want to allow the typing of 0.3
                clampValueOnBlur={false}
              />
            </div>
          )}
        />
      </div>
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

function StepThroughMovementsControl() {
  const [movementIndex, set] = useState(0)

  const currentDenseMovements = useRef<Movement[]>([])

  const updateSelectedMovementIndex = useCallback(movementIndex => {
    set(movementIndex)

    changeState(state => {
      state.treeStore.hoveredItems = [movementIndex]

      // console.log(`selected movement #${movementIndex}:`, currentDenseMovements.current[movementIndex])
    })
  }, [])

  // re-render on frame change, memoise the dense movements
  const viewportFrame = useSetting(state => state.viewportFrame)

  const orderedMovements = useMemo(() => {
    const movements = getOrderedMovementsForFrame(viewportFrame)
    currentDenseMovements.current = sparseToDense(
      movements,
      getSetting(state => state.settings),
    )

    return currentDenseMovements.current
  }, [viewportFrame])

  const maxIndex = Math.max(0, orderedMovements.length - 1)

  return (
    <div style={{ marginLeft: 10, marginRight: 10 }}>
      <Slider
        min={0}
        max={maxIndex}
        value={Math.min(movementIndex, maxIndex)} // If the frame changes, just set it to the max index
        onChange={updateSelectedMovementIndex}
        labelValues={[0, maxIndex]}
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
  OPTIMISATION = 'optimisation',
  EFFECTOR = 'effector',
}

function GeneralTab() {
  return (
    <Composition templateCols="1fr 200px" gap="1em" alignItems="center">
      <NumericInput
        label="Max Speed"
        min={25}
        max={500}
        stepSize={25}
        majorStepSize={100}
        rightText="mm/s"
        selector={state => state.settings.optimisation.maxSpeed}
        writer={(state, value) => (state.settings.optimisation.maxSpeed = value)}
        description="The peak movement speed"
      />
      <NumericInput
        label="Speed ramp distance"
        min={0}
        max={500}
        stepSize={5}
        majorStepSize={25}
        rightText="mm"
        selector={state => state.settings.optimisation.rampToMaxSpeedDistance}
        writer={(state, value) => (state.settings.optimisation.rampToMaxSpeedDistance = value)}
        description="The distance required to hit max speed"
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
        description="A scalar of the size of the transition bezier"
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
        description="How many milliseconds to wait per frame before drawing. The first transit move to a known location happens during this time."
      />
      <NumericInput
        label="LightFade Offset"
        min={-1000}
        max={1000}
        stepSize={1}
        majorStepSize={10}
        selector={state => state.settings.lightFadeOffset}
        writer={(state, value) => (state.settings.lightFadeOffset = value)}
        description="The amount in milliseconds to offset the LED from the movements."
      />
      <NumericInput
        label="Global Brightness"
        min={0}
        max={1}
        stepSize={0.01}
        majorStepSize={0.1}
        minorStepSize={0.01}
        selector={state => state.visualisationSettings.brightness}
        writer={(state, value) => (state.visualisationSettings.brightness = value)}
        description="A scalar to multiply the LED brightness"
      />
      <Checkbox
        label="Disable Shaped Transitions"
        selector={state => state.settings.optimisation.disableShapedTransitions}
        writer={(state, value) => (state.settings.optimisation.disableShapedTransitions = value)}
        description="Disables shaped transitions, replacing them with simple transit moves"
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
      <NumericInput
        label="Full Brightness By"
        min={0}
        max={100}
        stepSize={1}
        majorStepSize={10}
        rightText="%"
        selector={state => (state.settings.objectSettings.particles.fullBrightnessBy ?? 1) * 100}
        writer={(state, value) => (state.settings.objectSettings.particles.fullBrightnessBy = value / 100)}
      />
      <NumericInput
        label="Full Brightness Until"
        min={0}
        max={100}
        stepSize={1}
        majorStepSize={10}
        rightText="%"
        selector={state => (state.settings.objectSettings.particles.fullBrightnessUntil ?? 1) * 100}
        writer={(state, value) => (state.settings.objectSettings.particles.fullBrightnessUntil = value / 100)}
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
        label="Merge colinear lines"
        selector={state => state.settings.optimisation.mergeColinearLines}
        writer={(state, value) => (state.settings.optimisation.mergeColinearLines = value)}
      />
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
            description="Below this angle, lines can be smoothed with an inter-line bezier"
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
          <NumericInput
            label="Inter-line Transition-less Angle"
            min={0}
            max={360}
            stepSize={1}
            majorStepSize={36}
            rightText="°"
            selector={state => state.settings.optimisation.interLineTransitionLessAngle}
            writer={(state, value) => (state.settings.optimisation.interLineTransitionLessAngle = value)}
            description="Below this angle, movements don't need a transition at all"
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
  const stepThrough = useSetting(state => state.visualisationSettings.stepThrough)

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
      <Checkbox
        label="Step Through Movements"
        selector={state => state.visualisationSettings.stepThrough}
        writer={(state, value) => {
          state.visualisationSettings.stepThrough = value
          state.treeStore.hoveredItems = []
        }}
      />
      {stepThrough ? (
        <>
          Selected Step <StepThroughMovementsControl />
        </>
      ) : null}
      Annotate Draw Order <AnnotateOrderingToggle />
      Orbit Camera <OrbitCameraToggle />
      {/* <p>Curve Segments</p> <br /> */}
    </Composition>
  )
}

function EffectorTab() {
  return (
    <>
      <Composition templateCols="1fr 200px" gap="1em" alignItems="center">
        <NumericInput
          label="Pre Wait"
          min={0}
          max={1000}
          stepSize={25}
          majorStepSize={100}
          rightText="ms"
          selector={state => state.settings.objectSettings.effector.preWait}
          writer={(state, value) => (state.settings.objectSettings.effector.preWait = value)}
          description="The time to wait stationary before an effector move"
        />
        <NumericInput
          label="Post Wait"
          min={0}
          max={1000}
          stepSize={25}
          majorStepSize={100}
          rightText="ms"
          selector={state => state.settings.objectSettings.effector.postWait}
          writer={(state, value) => (state.settings.objectSettings.effector.postWait = value)}
          description="The time to wait stationary after an effector move"
        />
        <NumericInput
          label="DMX Light Intensity Modifier"
          min={0}
          max={500}
          stepSize={1}
          rightText="%"
          selector={state => (state.settings.objectSettings.effector.dmxValueScaler ?? 1) * 100}
          writer={(state, value) => (state.settings.objectSettings.effector.dmxValueScaler = value / 100)}
          description="A scalar to multiply the DMX brightness"
        />
        <DMXControlTab />
      </Composition>
    </>
  )
}

function OptimisationPassesTab() {
  return (
    <Composition templateCols="1fr 200px" gap="1em" alignItems="center">
      <Checkbox
        label="Seed with greedy search"
        selector={state => state.settings.optimisation.passes.nearestNeighbour}
        writer={(state, value) => (state.settings.optimisation.passes.nearestNeighbour = value)}
      />
      <Checkbox
        label="Brute force groups < 10"
        selector={state => state.settings.optimisation.passes.bruteForceSmall}
        writer={(state, value) => (state.settings.optimisation.passes.bruteForceSmall = value)}
      />
      <Checkbox
        label="2-opt pass"
        selector={state => state.settings.optimisation.passes.twoOpt}
        writer={(state, value) => (state.settings.optimisation.passes.twoOpt = value)}
      />
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
      <Tabs onChange={handleTabChange} selectedTabId={selected} renderActiveTabPanelOnly>
        <Tab id={TABS.GENERAL} title={<TabTitle title="General" icon={IconNames.COG} />} panel={<GeneralTab />} />
        <Tab
          id={TABS.PARTICLES}
          title={<TabTitle title="Particles" icon={IconNames.LAYOUT_SKEW_GRID} />}
          panel={<ParticlesTab />}
        />
        <Tab id={TABS.LIGHT} title={<TabTitle title="Light" icon={IconNames.LIGHTBULB} />} panel={<LightTab />} />
        <Tab id={TABS.LINE} title={<TabTitle title="Grease Pencil" icon={IconNames.DRAW} />} panel={<LineTab />} />
        <Tab id={TABS.EFFECTOR} title={<TabTitle title="Effector" icon={IconNames.TORCH} />} panel={<EffectorTab />} />
        <Tab
          id={TABS.OPTIMISATION}
          title={<TabTitle title="Optimisation Passes" icon={IconNames.DASHBOARD} />}
          panel={<OptimisationPassesTab />}
        />

        <Tabs.Expander />
        <Tab
          id={TABS.GEOMETRY}
          title={<TabTitle title="Geometry" icon={IconNames.POLYGON_FILTER} />}
          panel={
            <div>
              <RenderableTree />
              <MaterialEditorInterface />
            </div>
          }
        />
        <Tab
          id={TABS.CAMERA_HELPERS}
          title={<TabTitle title="Camera Helpers" icon={IconNames.CAMERA} />}
          panel={<CameraHelpersTab />}
        />

        <Tab
          id={TABS.VISUALISATION}
          title={<TabTitle title="Visualisation" icon={IconNames.NUMBERED_LIST} />}
          panel={<VisualisationTab />}
        />
      </Tabs>
    </div>
  )
}

/**
 * Render an icon with a tooltip
 */
function TabTitle(props: { icon: IconName; title: string }) {
  return (
    <Tooltip2
      content={props.title}
      intent={Intent.NONE}
      usePortal={true} // Force the rendering of the tooltip to be inline so focus bubbling works correctly
      placement="top"
      renderTarget={({ ref, isOpen, ...tooltipProps }) => (
        <div {...tooltipProps} ref={ref} className="planner-checkbox">
          {/* Attach to a div surrounding the NumericInput, since the NumericInput is complex and screws with the refs */}

          <Icon icon={props.icon} />
        </div>
      )}
    />
  )
}
