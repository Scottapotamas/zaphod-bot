import React from 'react'
import { useDarkMode } from '@electricui/components-desktop'

import logo from './logo.png'
import logoOrange from './logo-orange.png'

const Logo = () => {
  const isDark = useDarkMode()

  if (isDark) {
    return (
      <img
        src={logoOrange}
        style={{
          maxWidth: 200,
          display: 'block',
          margin: '0 auto',
          paddingTop: '10vh',
        }}
      />
    )
  }

  return (
    <img
      src={logo}
      style={{
        maxWidth: 200,
        display: 'block',
        margin: '0 auto',
        paddingTop: '10vh',
      }}
    />
  )
}

export default Logo
