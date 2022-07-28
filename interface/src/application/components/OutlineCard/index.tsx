import React from 'react'

import { useDarkMode } from '@electricui/components-desktop'
import { Colors } from '@blueprintjs/core'

interface OutlineCardProps {
  children: React.ReactNode
  padding?: string
}

export const OutlineCard = (props: OutlineCardProps) => {

  const isDark = useDarkMode()
  const color: string = isDark ? Colors.GRAY1 : Colors.GRAY3
  const luma = isDark ? 'brightness(0.7)' : 'brightness(0.98)'
  const padding = props.padding ? props.padding : '0.25em'

  return (
    <div
      style={{
        borderStyle: 'solid',
        borderWidth: '1px',
        borderRadius: '3px',
        borderColor: `${color}`,
        backdropFilter: `blur(10px) ${luma}`,
        padding: `${padding}`
      }}
    >
      {props.children}
    </div>
  )
}
