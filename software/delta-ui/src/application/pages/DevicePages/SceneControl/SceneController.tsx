import {
  Button as BlueprintButton,
  ButtonGroup,
  Card,
  Checkbox,
  FileInput,
  Icon,
  Intent,
  MultiSlider,
  NonIdealState,
  NumericInput,
} from '@blueprintjs/core'
import { Cell, Grid } from 'styled-css-grid'
import {
  IntervalRequester,
  StateTree,
  useDeviceMetadataKey,
  useHardwareState,
} from '@electricui/components-core'
import React, { ReactElement, useCallback, useEffect, useState } from 'react'

import { Button } from '@electricui/components-desktop-blueprint'
import { CALL_CALLBACK } from '@electricui/core'
import { CONTROL_MODES } from './../../../../transport-manager/config/codecs'
import { Printer } from '@electricui/components-desktop'
import { useExtractSceneName } from './../../../hooks/useExtractSceneName'
import { useOpenDialogCallFunction } from './../../../hooks/useOpenDialog'
import { useTriggerAction } from '@electricui/core-actions'

// const filePath = useDeviceMetadataKey('summary_file_path')

const OpenSceneButton = () => {
  const triggerAction = useTriggerAction()!

  const cb = (selectedFilePath: string) => {
    triggerAction('open_scene', selectedFilePath)
  }

  const selectFile = useOpenDialogCallFunction('json', 'Open a scene file', cb)

  return (
    <ButtonGroup>
      <BlueprintButton fill large onClick={selectFile} icon="document-open">
        Open Scene
      </BlueprintButton>
    </ButtonGroup>
  )
}

interface FrameControllerProps {
  frameStart: number
  frameEnd: number
  setFrameStart: (val: number) => void
  setFrameEnd: (val: number) => void
  disabled: boolean
}

const FrameController = (props: FrameControllerProps) => {
  const summaryFilePath = useDeviceMetadataKey('summary_file_path')

  let disabled = props.disabled

  if (!summaryFilePath || summaryFilePath === '') {
    disabled = true
  }

  const currentMin = useDeviceMetadataKey('frames_complete_min') || 0
  const currentMax = useDeviceMetadataKey('frames_complete_max') || 0

  const availableMin = useDeviceMetadataKey('min_frame') || 0
  const availableMax = useDeviceMetadataKey('max_frame') || 2

  // There's a bug here, we have to force an internal re-render when we modify the minimum and maximum
  const [count, setCount] = useState(0)

  const range = availableMax - availableMin

  let stepSize = 2

  if (range > 20) {
    stepSize = 4
  }
  if (range > 50) {
    stepSize = 5
  }
  if (range > 100) {
    stepSize = 10
  }

  return (
    <React.Fragment>
      <MultiSlider
        labelStepSize={stepSize}
        min={availableMin}
        max={availableMax}
        disabled={disabled}
      >
        <MultiSlider.Handle
          value={props.frameStart}
          type="start"
          interactionKind="lock"
          intentBefore="none"
          intentAfter={
            props.frameStart < currentMin
              ? 'warning'
              : props.frameStart > currentMax
              ? 'primary'
              : 'success'
          }
          onChange={newMin => {
            props.setFrameStart(newMin)
            setCount(count + 1) // TODO: fix?
          }}
        />
        <MultiSlider.Handle
          value={currentMin}
          type="full"
          interactionKind="none"
          intentBefore="none"
          intentAfter={
            props.frameStart > currentMin
              ? 'none'
              : currentMin < props.frameEnd
              ? 'success'
              : 'none'
          }
        />
        <MultiSlider.Handle
          value={currentMax}
          type="full"
          interactionKind="none"
          intentAfter={
            props.frameStart > currentMax
              ? 'none'
              : currentMax < props.frameEnd
              ? 'primary'
              : 'none'
          }
        />
        <MultiSlider.Handle
          value={props.frameEnd}
          type="end"
          interactionKind="lock"
          intentAfter="none"
          onChange={newMax => {
            props.setFrameEnd(newMax)
            setCount(count + 1) // TODO: fix?
          }}
        />
      </MultiSlider>
    </React.Fragment>
  )
}

const CollectionSelector = () => {
  const summaryFilePath = useDeviceMetadataKey('summary_file_path')

  const collectionOptions =  useDeviceMetadataKey('available_collections') as Array<string> // prettier-ignore
  const collectionSelected = useDeviceMetadataKey('selected_collections') as Array<string> // prettier-ignore

  const triggerAction = useTriggerAction()!

  const setCollectionSelected = (options: Array<string>) => {
    triggerAction('set_selected_collections', options)
  }

  if (!summaryFilePath || summaryFilePath === '') {
    return null
  }

  return (
    <React.Fragment>
      {collectionOptions.map(val => (
        <Checkbox
          key={val}
          checked={collectionSelected.includes(val)}
          onChange={() => {
            const isChecked = collectionSelected.includes(val)

            let newSelected = collectionSelected
            if (isChecked) {
              newSelected = newSelected.filter(newOption => newOption !== val) //
            } else {
              newSelected = [...newSelected, val]
            }

            setCollectionSelected(newSelected)
          }}
        >
          {val}
        </Checkbox>
      ))}
    </React.Fragment>
  )
}

const CurrentRGB = () => {
  const red_led = useHardwareState(state => state.rgb.red)
  const green_led = useHardwareState(state => state.rgb.green)
  const blue_led = useHardwareState(state => state.rgb.blue)

  return (
    <Icon
      icon="full-circle"
      iconSize={60}
      color={`rgb(${red_led},${green_led},${blue_led})`}
    />
  )
}

export const SceneController = () => {
  const availableMin = useDeviceMetadataKey('min_frame') || 1
  const availableMax = useDeviceMetadataKey('max_frame') || 2

  const executing = useDeviceMetadataKey('executing_scene') || false

  const triggerAction = useTriggerAction()!

  const [frameStart, setFrameStart] = useState(availableMin)
  const [frameEnd, setFrameEnd] = useState(availableMax)

  return (
    <div>
      <ButtonGroup>
        <OpenSceneButton />
        {executing ? (
          <BlueprintButton
            fill
            large
            intent="danger"
            icon="stop"
            onClick={() => {
              triggerAction('stop_scene_execution', {})
            }}
          >
            Stop
          </BlueprintButton>
        ) : (
          <BlueprintButton
            fill
            large
            intent="success"
            icon="caret-right"
            onClick={() => {
              triggerAction('start_scene_execution', {
                frameStart,
                frameEnd,
              })
            }}
          >
            Start
          </BlueprintButton>
        )}
        <Button writer={{ clmv: CALL_CALLBACK }} intent="warning">
          Clear queue
        </Button>
      </ButtonGroup>

      <CurrentRGB />

      <label>Collection selection</label>
      <CollectionSelector />
      <label>Frame Selection</label>
      <FrameController
        key={`${availableMin}-${availableMax}`} // we do this so we re-init it on each new scene opening
        frameStart={frameStart}
        frameEnd={frameEnd}
        setFrameStart={setFrameStart}
        setFrameEnd={setFrameEnd}
        disabled={executing}
      />
    </div>
  )
}

/*
  const triggerAction = useTriggerAction()

      <button onClick={() => triggerAction('movement_queue_paused', true)}>
        p
      </button>
      <button onClick={() => triggerAction('movement_queue_paused', false)}>
        up
      </button>

            <button onClick={() => triggerAction('light_queue_paused', true)}>
        p
      </button>
      <button onClick={() => triggerAction('light_queue_paused', false)}>
        up
      </button>

      */
