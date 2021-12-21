import React from 'react'

import deltaLight from './delta-light.png'
import deltaDark from './delta-dark.png'

import { useDarkMode } from '@electricui/components-desktop'

import './background.css'

interface LandingProps {
  children: React.ReactNode
}

export const Landing = (props: LandingProps) => {
  return <div className="landing landing-background">{props.children}</div>
}
