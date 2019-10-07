import {
  useDarkMode,
  useDarkModeSystem,
  useSetDarkMode,
} from '@electricui/components-desktop'

import { Button } from '@blueprintjs/core'
import React from 'react'

export const SetDarkModeButton = () => {
  const darkMode = useDarkMode()
  const setDarkMode = useSetDarkMode()
  const system = useDarkModeSystem()

  if (darkMode) {
    return (
      <>
        <Button
          icon="lightbulb"
          onClick={() => setDarkMode(false)}
          style={{ marginLeft: 10 }}
        />
        {system ? null : (
          <Button
            icon="settings"
            onClick={() => setDarkMode(null)}
            style={{ marginLeft: 10 }}
          />
        )}
      </>
    )
  }

  return (
    <>
      <Button
        icon="moon"
        onClick={() => setDarkMode(true)}
        style={{ marginLeft: 10 }}
      />
      {system ? null : (
        <Button
          icon="settings"
          onClick={() => setDarkMode(null)}
          style={{ marginLeft: 10 }}
        />
      )}
    </>
  )
}
