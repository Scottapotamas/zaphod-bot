import React, { ReactElement, useState, useEffect, useCallback } from 'react'

import { Button } from '@electricui/components-desktop-blueprint'
import { Printer } from '@electricui/components-desktop'
import {
  Button as BlueprintButton,
  ButtonGroup,
  Card,
  Icon,
  FileInput,
} from '@blueprintjs/core'
import { Grid, Cell } from 'styled-css-grid'
import {
  IntervalRequester,
  useHardwareState,
  useDeviceMetadataKey,
} from '@electricui/components-core'

import { useTriggerAction } from '@electricui/core-actions'

import { useExtractSceneName } from './../../../hooks/useExtractSceneName'
import { useOpenDialogCallFunction } from './../../../hooks/useOpenDialog'

import { CALL_CALLBACK } from '@electricui/core'

const RunSceneButton = () => {
  const filePath = useDeviceMetadataKey('loadedScene')

  const sceneName = useExtractSceneName(filePath)
  const triggerAction = useTriggerAction()!

  return (
    <BlueprintButton
      fill
      large
      onClick={() => {
        console.log('starting action')
        triggerAction('load_scene', { filePath })
          .catch(e => {
            console.error(e)
          })
          .then(() => {
            console.log('action finished')
          })
      }}
      icon="play"
    >
      Run {sceneName}
    </BlueprintButton>
  )
}

const OpenSceneButton = () => {
  const filePath = useDeviceMetadataKey('loadedScene')
  const triggerAction = useTriggerAction()!

  const cb = (selectedFilePath: string) => {
    triggerAction('set_loaded_scene', selectedFilePath)
  }

  const selectFile = useOpenDialogCallFunction('json', 'Open a scene file', cb)

  let runSceneButton: ReactElement | null = null

  if (filePath !== null && filePath !== '') {
    runSceneButton = <RunSceneButton />
  }

  return (
    <ButtonGroup>
      <BlueprintButton fill large onClick={selectFile} icon="document-open">
        Open Scene
      </BlueprintButton>
      {runSceneButton}
    </ButtonGroup>
  )
}

const EventPalette = () => {
  const red_led = useHardwareState(state => state.rgb.red)
  const green_led = useHardwareState(state => state.rgb.green)
  const blue_led = useHardwareState(state => state.rgb.blue)

  const triggerAction = useTriggerAction()

  return (
    <div>
      <IntervalRequester interval={100} variables={['rgb']} />
      <h3>Load Event Sequence from File</h3>
      <OpenSceneButton />

      <br />
      <br />
      <Grid
        columns={'3fr 1fr 1fr'}
        justifyContent="space-around"
        alignContent="space-around"
      >
        <Cell>
          <Button writer={{ stmv: CALL_CALLBACK }}>Start queue</Button>
          <br />
          <br />
          <Button writer={{ clmv: CALL_CALLBACK }}>Clear queue</Button>
          <br />
          <br />
          <BlueprintButton onClick={() => triggerAction('sync', 1)}>
            Sync
          </BlueprintButton>
        </Cell>
        <Cell right middle>
          R: {red_led} <br />
          G: {green_led} <br />
          B: {blue_led} <br />
        </Cell>
        <Cell center middle>
          <Icon
            icon="full-circle"
            iconSize={60}
            color={`rgb(${red_led},${green_led},${blue_led})`}
          />
        </Cell>
      </Grid>
    </div>
  )
}

export default EventPalette
