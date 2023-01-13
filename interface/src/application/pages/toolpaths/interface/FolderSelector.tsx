import { Button, Intent } from '@blueprintjs/core'

import React, { useCallback, useState } from 'react'
import { OPEN_DIALOG_IPC_EVENT } from '@electricui/utility-electron'
import { ipcRenderer, OpenDialogOptions } from 'electron'

import classnames from 'classnames'

import fs from 'fs'
import path from 'path'

interface OpenResult {
  canceled: boolean
  filePaths: string[]
}

import { getSetting, markClean, setFolder, useSetting } from './state'
import { IconNames } from '@blueprintjs/icons'

export function FolderPathSelector() {
  const options: OpenDialogOptions = {
    properties: ['openDirectory'],
    message: 'Open animation folder',
  }

  const [selectedFolder, setSelectedFolder] = useState(getSetting(state => state.folder) ?? '')
  const [labelText, setLabelText] = useState('Choose folder...')
  const [error, setError] = useState('')

  const retrieveFilePath = useCallback(async () => {
    const result: OpenResult = await ipcRenderer.invoke(OPEN_DIALOG_IPC_EVENT, options)

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

  const reload = useCallback(() => {
    const folder = getSetting(state => state.folder)
    setFolder(null)
    setFolder(folder)
  }, [setSelectedFolder])

  const hasFolderSelected = useSetting(state => Boolean(state.folder))
  const isDirty = useSetting(state => state.settingsDirty)

  const [saving, setSaving] = useState(false)

  const save = useCallback(async () => {
    const folder = getSetting(state => state.folder)

    if (!folder) {
      console.error(`Save pressed with no folder selected`)
      return
    }

    const settingsFilePath = path.join(folder, `settings.json`)

    setSaving(true)

    const serialised = {
      type: `root-settings`,
      version: 1,
      settings: getSetting(state => state.settings),
      visualisationSettings: getSetting(state => state.visualisationSettings),
    }

    console.log(`saving`, serialised)

    await fs.promises.writeFile(settingsFilePath, JSON.stringify(serialised), { encoding: 'utf8' })

    // Mark all settings as clean since we just saved them.
    markClean()

    setSaving(false)
  }, [setSelectedFolder, setSaving])

  return (
    <>
      <div style={{ width: '66%', display: 'inline-block' }}>
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
      </div>

      <div style={{ width: '33%', display: 'inline-block', verticalAlign: 'middle' }}>
        <div style={{ paddingTop: 5 }}>
          <Button
            intent={Intent.WARNING}
            disabled={!hasFolderSelected}
            onClick={reload}
            small
            outlined
            style={{ marginRight: 3 }}
            icon={IconNames.REFRESH}
          >
            {/* Reload */}
          </Button>
          <Button
            intent={Intent.SUCCESS}
            disabled={!hasFolderSelected || !isDirty}
            onClick={save}
            loading={saving}
            small
            outlined
            icon={IconNames.FLOPPY_DISK}
          >
            {/* Save Settings */}
          </Button>
        </div>
      </div>
    </>
  )
}
