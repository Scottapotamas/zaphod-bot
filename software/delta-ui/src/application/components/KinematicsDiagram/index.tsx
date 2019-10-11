import React from 'react'
import { Printer, useDarkMode } from '@electricui/components-desktop'

import deltaDark from './delta-dark.png'
import deltaLight from './delta-light.png'

interface DiagramProps {
  shoulderDistance: Number
  bicepLength: Number
  forearmLength: Number
  effectorDistance: Number
}

const DeltaMechanismDiagram = (props: DiagramProps) => {
  const isDark = useDarkMode()
  return (
    <div
      style={{
        height: '350px',
        width: '280px',
      }}
    >
      <img
        src={isDark ? deltaDark : deltaLight}
        style={{
          marginTop: '10px',
          maxHeight: '420px',
          position: 'absolute',
          overflow: 'hidden',
        }}
      />
      <div
        style={{
          top: '0px',
          left: '130px',
          position: 'relative',
        }}
      >
        ⌀ <Printer accessor={state => state.kinematics.effector_radius} /> mm
      </div>

      <div
        style={{
          top: '80px',
          left: '20px',
          position: 'relative',
        }}
      >
        <Printer accessor={state => state.kinematics.forearm_length} /> mm
      </div>

      <div
        style={{
          top: '185px',
          left: '30px',
          position: 'relative',
        }}
      >
        <Printer accessor={state => state.kinematics.bicep_length} /> mm
      </div>

      <div
        style={{
          top: '200px',
          left: '20px',
          position: 'relative',
        }}
      >
        ⌀ <Printer accessor={state => state.kinematics.shoulder_radius} /> mm
      </div>
    </div>
  )
}

export default DeltaMechanismDiagram
