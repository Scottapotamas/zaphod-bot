import { OpenDialogOptions, remote } from 'electron'
import React, { useCallback, useState } from 'react'

const useOpenDialog = (
  extension: string | 'folder' | 'json',
  message: string,
): [string, () => void] => {
  const [filePath, setFilePath] = useState('')

  const select = useCallback(() => {
    const options: OpenDialogOptions = {
      message,
    }

    if (extension === 'folder') {
      options.properties = ['openDirectory', 'createDirectory']
    } else {
      options.filters = [{ name: `.${extension}`, extensions: [extension] }]
      options.properties = ['openFile']
    }

    remote.dialog.showOpenDialog(options, (filepath: string[]) => {
      if (typeof filepath === 'undefined') {
        return
      }

      setFilePath(filepath[0])
    })
  }, [])

  return [filePath, select]
}

const useOpenDialogCallFunction = (
  extension: string | 'folder' | 'json',
  message: string,
  func: (filePath: string) => void,
) => {
  return useCallback(() => {
    const options: OpenDialogOptions = {
      message,
    }

    if (extension === 'folder') {
      options.properties = ['openDirectory', 'createDirectory']
    } else {
      options.filters = [{ name: `.${extension}`, extensions: [extension] }]
      options.properties = ['openFile']
    }

    remote.dialog.showOpenDialog(options, (filepath: string[]) => {
      if (typeof filepath === 'undefined') {
        return
      }

      func(filepath[0])
    })
  }, [])
}

export { useOpenDialog, useOpenDialogCallFunction }
