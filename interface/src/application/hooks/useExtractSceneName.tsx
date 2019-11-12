import React, { useEffect, useState } from 'react'

import fs from 'fs'

const useExtractSceneName = (filePath: string) => {
  const [sceneName, setSceneName] = useState('')

  // Load some metadata from the file
  useEffect(() => {
    if (filePath === '') {
      return
    }

    const sceneData = fs.readFileSync(filePath)
    const json = JSON.parse(sceneData.toString())

    const name = json.metadata.name

    setSceneName(name)
  }, [filePath])

  return sceneName
}

export { useExtractSceneName }
