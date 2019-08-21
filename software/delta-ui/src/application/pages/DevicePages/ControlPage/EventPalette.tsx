import React, { ReactElement, useState, useEffect } from 'react'

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
} from '@electricui/components-core'

import { useTriggerAction } from '@electricui/core-actions'

import { useExtractSceneName } from './../../../hooks/useExtractSceneName'
import { useOpenDialog } from './../../../hooks/useOpenDialog'

import { CALL_CALLBACK } from '@electricui/core'

const OpenSceneButton = () => {
  const [filePath, selectFile] = useOpenDialog('json', 'Open a scene file')
  const sceneName = useExtractSceneName(filePath)
  const triggerAction = useTriggerAction()!

  let RunSceneButton: ReactElement | null = null

  if (filePath !== '') {
    RunSceneButton = (
      <React.Fragment>
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
      </React.Fragment>
    )
  }

  return (
    <>
      <BlueprintButton fill large onClick={selectFile} icon="document-open">
        Open Scene
      </BlueprintButton>
      {RunSceneButton}
    </>
  )
}

const SceneSelectionButtons = () => {
  const [filePath, selectFolder] = useOpenDialog(
    'folder',
    'Select folder for saving',
  )

  const triggerAction = useTriggerAction()

  // Save the save path every time we select a new path
  useEffect(() => {
    triggerAction('set_save_path', filePath)
  }, [filePath])

  let filePathValid = true

  if (filePath === '') {
    filePathValid = false
  }

  return (
    <React.Fragment>
      <br />
      <BlueprintButton fill large onClick={selectFolder} icon="camera">
        Photo Save Location
      </BlueprintButton>

      {filePathValid ? <OpenSceneButton /> : null}
    </React.Fragment>
  )
}

const EventPalette = () => {
  const [syncID, setSyncID] = useState(1)
  const triggerAction = useTriggerAction()

  const red_led = useHardwareState(state => state.rgb.red)
  const green_led = useHardwareState(state => state.rgb.green)
  const blue_led = useHardwareState(state => state.rgb.blue)

  return (
    <div>
      <IntervalRequester interval={100} variables={['rgb']} />
      <h3>Load Event Sequence from File</h3>
      <ButtonGroup>
        <SceneSelectionButtons />
      </ButtonGroup>

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
          <Button writer={{ psmv: CALL_CALLBACK }}>Pause queue</Button>
          <br />
          <br />
          <Button writer={{ clmv: CALL_CALLBACK }}>Clear queue</Button>
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
