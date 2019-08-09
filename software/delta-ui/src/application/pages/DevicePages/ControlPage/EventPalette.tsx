import React, { ReactElement, useState, useEffect } from 'react'

import { Button } from '@electricui/components-desktop-blueprint'
import { Printer } from '@electricui/components-desktop'
import { Button as BlueprintButton } from '@blueprintjs/core'
import { Grid, Cell } from 'styled-css-grid'

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
          style={{ marginLeft: 10 }}
        >
          Run {sceneName}
        </BlueprintButton>
      </React.Fragment>
    )
  }

  return (
    <>
      <BlueprintButton onClick={selectFile} style={{ marginLeft: 10 }}>
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
      <BlueprintButton onClick={selectFolder}>
        Select Camera Save Location
      </BlueprintButton>

      {filePathValid ? <OpenSceneButton /> : null}
    </React.Fragment>
  )
}

const EventPalette = () => {
  const [syncID, setSyncID] = useState(1)
  const [heightScale, setHeightScale] = useState(1)
  const triggerAction = useTriggerAction()

  return (
    <div>
      <h3>Event Sequence</h3>
      <SceneSelectionButtons />
      <br />
      <br />
      <br />
      <br />
      <Button writer={{ stmv: CALL_CALLBACK }}>Start queue</Button>
      <Button writer={{ psmv: CALL_CALLBACK }}>Pause queue</Button>
      <Button writer={{ clmv: CALL_CALLBACK }}>Clear queue</Button>
    </div>
  )
}

export default EventPalette
