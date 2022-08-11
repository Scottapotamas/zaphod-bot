import { FormGroup } from '@blueprintjs/core'

import React, { useCallback, useEffect, useRef, useState } from 'react'
import { OPEN_DIALOG_IPC_EVENT } from '@electricui/utility-electron'
import { ipcRenderer, OpenDialogOptions } from 'electron'

import classnames from 'classnames'

import fs from 'fs'
import path from 'path'

interface OpenResult {
  canceled: boolean
  filePaths: string[]
}

import { getSetting, setFolder } from './state'

export function FolderPathSelector() {
  const options: OpenDialogOptions = {
    properties: ['openDirectory'],
    message: 'Open animation folder',
  }

  const [selectedFolder, setSelectedFolder] = useState(
    getSetting(state => state.folder) ?? '',
  )
  const [labelText, setLabelText] = useState('Choose folder...')
  const [error, setError] = useState('')

  const retrieveFilePath = useCallback(async () => {
    const result: OpenResult = await ipcRenderer.invoke(
      OPEN_DIALOG_IPC_EVENT,
      options,
    )

    if (result.canceled) return

    // There will only be a singular directory selected
    const folder = result.filePaths[0]

    // Validate it isn't a numbered directory (and therefore we've probably selected a frame folder)
    const reg = new RegExp('^[0-9]+$')

    if (reg.exec(path.basename(folder))) {
      setSelectedFolder('')
      setLabelText('Choose folder...')
      setError('Incorrectly selected frame folder')
      return
    }

    setSelectedFolder(folder)
    setLabelText('Counting frames...')
    setError('')

    try {
      // Just as a heuristic, count the entries within the folder to determine how many frames

      // If we select a frame folder, this
      const frameDirectories = await fs.promises.readdir(folder)

      setLabelText(`${frameDirectories.length} frames selected`)
      setError('')

      // HACK: Set it to null, then the folder in case the user reloads the same folder in an attempt
      // to reload it.
      setFolder(null)
      setFolder(folder)
    } catch (e) {
      console.error(e)
      setLabelText('Error with this directory...')
      setSelectedFolder('')
      setError(e.message)
      setFolder(null)
    }
  }, [setSelectedFolder])

  return (
    <FormGroup helperText={error} intent={error === '' ? 'none' : 'danger'}>
      <div className="bp3-file-input bp3-fill .modifier" onClick={retrieveFilePath}>
        <input type="file" />
        <span
          className={classnames({
            'bp3-file-input-has-selection': selectedFolder !== '',
            'bp3-file-upload-input': true,
          })}
        >
          {labelText}
        </span>
      </div>
    </FormGroup>
  )
}
