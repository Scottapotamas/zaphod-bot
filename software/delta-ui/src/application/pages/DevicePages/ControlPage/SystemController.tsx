import {
  ProgressBar,
  Slider,
  Statistics,
  Statistic,
  Button,
  Switch,
} from '@electricui/components-desktop-blueprint'
import { Printer } from '@electricui/components-desktop'
import {
  Card,
  Divider,
  ButtonGroup,
  Label,
  Text,
  HTMLTable,
  Button as BlueprintButton,
  NumericInput,
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

const ModeSelectButtons = () => {
  const control_mode = useHardwareState(state => state.super.mode)

  let manualIntent: Intent
  let eventIntent: Intent
  let demoIntent: Intent
  let trackIntent: Intent

  manualIntent =
    control_mode == CONTROL_MODES[CONTROL_MODES.MANUAL]
      ? Intent.PRIMARY
      : Intent.NONE
  eventIntent =
    control_mode == CONTROL_MODES[CONTROL_MODES.EVENT]
      ? Intent.PRIMARY
      : Intent.NONE
  demoIntent =
    control_mode == CONTROL_MODES[CONTROL_MODES.DEMO]
      ? Intent.PRIMARY
      : Intent.NONE
  trackIntent =
    control_mode == CONTROL_MODES[CONTROL_MODES.TRACK]
      ? Intent.PRIMARY
      : Intent.NONE

  return (
    <ButtonGroup fill>
      <Button writer={{ rmanual: CALL_CALLBACK }} intent={manualIntent}>
        Manual
      </Button>
      <Button writer={{ revent: CALL_CALLBACK }} intent={eventIntent}>
        Event
      </Button>
      <Button writer={{ rdemo: CALL_CALLBACK }} intent={demoIntent}>
        Demo
      </Button>
      <Button writer={{ rtrack: CALL_CALLBACK }} intent={trackIntent}>
        Track
      </Button>
    </ButtonGroup>
  )
}

const ControlSurfaceFromMode = () => {
  const control_mode = useHardwareState(state => state.super.mode)

  if (control_mode == CONTROL_MODES[CONTROL_MODES.MANUAL]) {
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
    return (
      <NonIdealState
        title="Track Mode Unavailable"
        description="Locked out until UI developed"
      />
    )
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
