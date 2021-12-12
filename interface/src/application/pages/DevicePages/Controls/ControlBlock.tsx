import { NonIdealState, Card } from '@blueprintjs/core'
import { useHardwareState } from '@electricui/components-core'
import { Composition, Box } from 'atomic-layout'

import React from 'react'

import { CONTROL_MODES } from '../../../typedState'
import ManualJogPalette from './ControlPalette/ManualJogPalette'
import { TrackPalette } from './ControlPalette/TrackPalette'
import { LEDControlPalette } from './LEDControlPalette'

export const ControlBlock = () => {
  const control_mode = useHardwareState(state => state.super.mode)

  if (control_mode == CONTROL_MODES[CONTROL_MODES.NONE]) {
    return (
      <NonIdealState
        title="Select desired mode"
        description="Select a control mode before arming"
      />
    )
  } else if (control_mode == CONTROL_MODES[CONTROL_MODES.MANUAL]) {
    return (
      <Card>
        <ManualJogPalette />
      </Card>
    )
  } else if (control_mode == CONTROL_MODES[CONTROL_MODES.EVENT]) {
    // return <EventPalette />
    return (
      <Card>
        <NonIdealState
          title="Event Pannel Broken"
          description="Scott broke this when updating the ui to latest versions"
        />
      </Card>
    )
  } else if (control_mode == CONTROL_MODES[CONTROL_MODES.DEMO]) {
    return (
      <Card>
        <NonIdealState
          title="Demonstration Mode"
          description="Begins automatically once armed"
        />
      </Card>
    )
  } else if (control_mode == CONTROL_MODES[CONTROL_MODES.TRACK]) {
    return <TrackPalette />
  }

  return (
    <Card>
      <NonIdealState title="Changing Mode" description="Please wait" />
    </Card>
  )
}
