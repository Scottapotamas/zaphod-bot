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
import {
  IntervalRequester,
  StateTree,
  useDeviceMetadataKey,
  useHardwareState,
} from '@electricui/components-core'
import React, { ReactElement, useCallback, useEffect, useState } from 'react'

import { Button } from '@electricui/components-desktop-blueprint'
import { CONTROL_MODES } from '../../../../../transport-manager/config/codecs'
import { Composition } from 'atomic-layout'
import { Printer } from '@electricui/components-desktop'
import { SUPERVISOR_STATES } from '../../../../../transport-manager/config/codecs'
import { useExtractSceneName } from './../../../../hooks/useExtractSceneName'
import { useOpenDialogCallFunction } from './../../../../hooks/useOpenDialog'
import { useTriggerAction } from '@electricui/core-actions'

// const filePath = useDeviceMetadataKey('summary_file_path')

const OpenSceneButton = () => {
  const triggerAction = useTriggerAction()!

  const cb = (selectedFilePath: string) => {
    triggerAction('open_scene', selectedFilePath)
  }

  const selectFile = useOpenDialogCallFunction('json', 'Open a scene file', cb)

  return (
    <BlueprintButton
      large
      onClick={selectFile}
      icon="document-open"
      style={{ width: '50%' }}
    >
      Open Scene
    </BlueprintButton>
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

  const collectionOptions =  useDeviceMetadataKey('available_collections') as Array<string> || [] // prettier-ignore
  const collectionSelected = useDeviceMetadataKey('selected_collections') as Array<string> || [] // prettier-ignore

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

const controllerAreas = `
ButtonAreas
CollectionsArea 
FramesArea 
`

function useIsArmed() {
  const supervisor = useHardwareState<string>(state => state.super.supervisor)

  const isArmed = supervisor === SUPERVISOR_STATES[SUPERVISOR_STATES.ARMED]

  return isArmed
}

export const SceneController = () => {
  const availableMin = useDeviceMetadataKey('min_frame') || 1
  const availableMax = useDeviceMetadataKey('max_frame') || 2

  const executing = useDeviceMetadataKey('executing_scene') || false

  const triggerAction = useTriggerAction()!

  const [frameStart, setFrameStart] = useState(availableMin)
  const [frameEnd, setFrameEnd] = useState(availableMax)

  const isArmed = useIsArmed()
  const sceneFilePath = useDeviceMetadataKey('summary_file_path')

  return (
    <Composition areas={controllerAreas} gap={10}>
      {({ ButtonAreas, CollectionsArea, FramesArea, RGBArea }) => (
        <React.Fragment>
          <ButtonAreas>
            <ButtonGroup fill={true}>
              <OpenSceneButton />
              {executing ? (
                <BlueprintButton
                  large
                  intent="danger"
                  style={{ width: '50%' }}
                  icon="stop"
                  onClick={() => {
                    triggerAction('stop_scene_execution', {})
                  }}
                >
                  Stop
                </BlueprintButton>
              ) : (
                <BlueprintButton
                  large
                  intent="success"
                  icon="caret-right"
                  onClick={() => {
                    triggerAction('start_scene_execution', {
                      frameStart,
                      frameEnd,
                    })
                  }}
                  style={{ width: '50%' }}
                  disabled={!isArmed || !sceneFilePath}
                >
                  Start
                </BlueprintButton>
              )}
            </ButtonGroup>
          </ButtonAreas>
          <CollectionsArea>
            <CollectionSelector />
          </CollectionsArea>
          <FramesArea padding={10}>
            <FrameController
              key={`${availableMin}-${availableMax}-${sceneFilePath}`} // we do this so we re-init it on each new scene opening
              frameStart={frameStart}
              frameEnd={frameEnd}
              setFrameStart={setFrameStart}
              setFrameEnd={setFrameEnd}
              disabled={executing}
            />
          </FramesArea>
        </React.Fragment>
      )}
    </Composition>
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
