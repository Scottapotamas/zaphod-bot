import React from 'react'

import deltaLight from './delta-light.png'
import deltaDark from './delta-dark.png'

import { useDarkMode } from '@electricui/components-desktop'

export const Landing = () => {
  const isDark = useDarkMode()

  if (isDark) {
    return (
      <img
        src={deltaDark}
        style={{
          maxHeight: '100%',
          maxWidth: '100%',
          top: '0',
          bottom: '0',
          position: 'absolute',
          margin: 'auto',
          overflow: 'hidden',
        }}
      />
    )
  }

  return (
    <img
      src={deltaLight}
      style={{
        maxHeight: '100%',
        maxWidth: '100%',
        top: '0',
        bottom: '0',
        position: 'absolute',
        margin: 'auto',
        overflow: 'hidden',
      }}
    />
  )
}
