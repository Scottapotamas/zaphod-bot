import { ButtonGroup, NonIdealState, Navbar } from '@blueprintjs/core'
import { useHardwareState } from '@electricui/components-core'

import React from 'react'

import { Button } from '@electricui/components-desktop-blueprint'
import { CONTROL_MODES } from '../../../../transport-manager/config/codecs'
import EventPalette from './EventPalette'
import ManualJogPalette from './ManualJogPalette'
import TrackPalette from './TrackPalette'

const ModeSelectButtons = () => {
  const control_mode = useHardwareState(state => state.super.mode)

  return (
    <React.Fragment>
      <ButtonGroup fill>
        <Button
          minimal
          large
          icon="move"
          writer={{ req_mode: 1 }}
          active={control_mode == CONTROL_MODES[CONTROL_MODES.MANUAL]}
        >
          Manual Jog
        </Button>
        <Button
          minimal
          large
          icon="flow-end"
          writer={{ req_mode: 3 }}
          active={control_mode == CONTROL_MODES[CONTROL_MODES.TRACK]}
        >
          Track
        </Button>
        <Button
          minimal
          large
          icon="projects"
          writer={{ req_mode: 4 }}
          active={control_mode == CONTROL_MODES[CONTROL_MODES.DEMO]}
        >
          Demo
        </Button>
        <Button
          minimal
          large
          icon="document-open"
          writer={{ req_mode: 2 }}
          active={control_mode == CONTROL_MODES[CONTROL_MODES.EVENT]}
        >
          Run Program
        </Button>
      </ButtonGroup>
    </React.Fragment>
  )
}

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
    return <EventPalette />
  } else if (control_mode == CONTROL_MODES[CONTROL_MODES.DEMO]) {
    return (
      <NonIdealState
        title="Demonstration Mode Unavailable"
        description="The delta-robot controller does not yet support demo mode"
      />
    )
  } else if (control_mode == CONTROL_MODES[CONTROL_MODES.TRACK]) {
    return <TrackPalette />
  }

  return <div>Select a mode to unlock control</div>
}

const SystemController = () => {
  return (
    <div style={{ minHeight: '300px' }}>
      <ModeSelectButtons />
      <br />
      <ControlSurfaceFromMode />
    </div>
  )
}

export default SystemController
