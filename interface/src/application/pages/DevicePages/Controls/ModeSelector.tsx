import { ButtonGroup, NonIdealState } from '@blueprintjs/core'
import { useHardwareState } from '@electricui/components-core'

import React from 'react'

import { Button } from '@electricui/components-desktop-blueprint'
import { CONTROL_MODES } from '../../../typedState'
import ManualJogPalette from '../ControlPalette/ManualJogPalette'
import TrackPalette from '../ControlPalette/TrackPalette'
import ModeSelection from '../ControlPalette/ModeSelection'


const ControlSurfaceFromMode = () => {
  const control_mode = useHardwareState(state => state.super.mode)

  if (control_mode == CONTROL_MODES[CONTROL_MODES.NONE]) {
    return (
      <NonIdealState
        title="Select desired mode"
        description="Select a control mode before arming"
      />
    )
  } else if (control_mode == CONTROL_MODES[CONTROL_MODES.MANUAL]) {
    return <ManualJogPalette />
  } else if (control_mode == CONTROL_MODES[CONTROL_MODES.EVENT]) {
    // return <EventPalette />
    return (
      <NonIdealState
        title="Event Pannel Broken"
        description="Scott broke this when updating the ui to latest versions"
      />
    )
  } else if (control_mode == CONTROL_MODES[CONTROL_MODES.DEMO]) {
    return (
      <NonIdealState
        title="Demonstration Mode"
        description="Begins automatically once armed"
      />
    )
  } else if (control_mode == CONTROL_MODES[CONTROL_MODES.TRACK]) {
    return <TrackPalette />
  }

  return <NonIdealState title="Changing Mode" description="Please wait" />
}

const ModeSelector = () => {
  return (
    <div style={{ minHeight: '300px' }}>
      <ModeSelection />
      <br />
      <ControlSurfaceFromMode />
    </div>
  )
}

export default ModeSelector
