import { Button } from '@electricui/components-desktop-blueprint'
import { Printer } from '@electricui/components-desktop'
import {
  Card,
  ButtonGroup,
  Button as BlueprintButton,
  Intent,
  NonIdealState,
} from '@blueprintjs/core'
import { Grid, Cell } from 'styled-css-grid'

import {
  IntervalRequester,
  useHardwareState,
  StateTree,
} from '@electricui/components-core'

import { CALL_CALLBACK } from '@electricui/core'
import { CONTROL_MODES } from './../../../../transport-manager/config/codecs'
import React from 'react'

import ManualJogPalette from './ManualJogPalette'
import EventPalette from './EventPalette'
import TrackPalette from './TrackPalette'

const ModeSelectButtons = () => {
  const control_mode = useHardwareState(state => state.super.mode)

  return (
    <React.Fragment>
      <ButtonGroup fill>
        <Button
          writer={{ req_mode: 1 }}
          intent={
            control_mode == CONTROL_MODES[CONTROL_MODES.MANUAL]
              ? 'primary'
              : 'none'
          }
        >
          Manual
        </Button>
        <Button
          writer={{ req_mode: 2 }}
          intent={
            control_mode == CONTROL_MODES[CONTROL_MODES.EVENT]
              ? 'primary'
              : 'none'
          }
        >
          Event
        </Button>
        <Button
          writer={{ req_mode: 4 }}
          intent={
            control_mode == CONTROL_MODES[CONTROL_MODES.DEMO]
              ? 'primary'
              : 'none'
          }
        >
          Demo
        </Button>
        <Button
          writer={{ req_mode: 3 }}
          intent={
            control_mode == CONTROL_MODES[CONTROL_MODES.TRACK]
              ? 'primary'
              : 'none'
          }
        >
          Track
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
    <Card style={{ minHeight: '300px' }}>
      <ModeSelectButtons />
      <br />
      <ControlSurfaceFromMode />
    </Card>
  )
}

export default SystemController
