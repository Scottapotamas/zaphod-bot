import React from 'react'

import { useDarkMode } from '@electricui/components-desktop'
import { Colors } from '@blueprintjs/core'

interface OutlineCardProps {
  children: React.ReactNode
}

export const OutlineCard = (props: OutlineCardProps) => {
  return (
    <div
      style={{
        borderStyle: 'solid',
        borderWidth: '1px',
        borderRadius: '3px',
        borderColor: Colors.GRAY1,
        backdropFilter: 'blur(10px)',
        padding: '0.25em'
      }}
    >
      {props.children}
    </div>
  )
}
