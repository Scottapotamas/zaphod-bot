import { NonIdealState, Callout, Card, Intent } from '@blueprintjs/core'
import { useHardwareState } from '@electricui/components-core'
import { Composition, Box } from 'atomic-layout'

import React from 'react'

import { CONTROL_MODES } from '../../../typedState'
import ManualJogPalette from './ControlPalette/ManualJogPalette'
import ManualLEDPalette from './ControlPalette/ManualLEDPalette'
import { TrackPalette } from './ControlPalette/TrackPalette'
import { EventPalette } from './ControlPalette/EventPalette'

import { MSGID } from '../../../typedState'

export const ControlBlock = () => {
  const control_mode = useHardwareState(state => state[MSGID.SUPERVISOR].mode)

  if (control_mode == CONTROL_MODES[CONTROL_MODES.NONE]) {
    return (
      <Callout icon={null} intent={Intent.WARNING}>
        <NonIdealState
          title="Select desired mode"
          description="Select a control mode before arming"
          action={<p><br/></p>}
        ></NonIdealState>
      </Callout>
    )
  } else if (control_mode == CONTROL_MODES[CONTROL_MODES.MANUAL]) {
    return (
      <div>
        <Card>
          <ManualLEDPalette />
        </Card>
        <br/>
        <Card>
          <ManualJogPalette />
        </Card>
      </div>
    )
  } else if (control_mode == CONTROL_MODES[CONTROL_MODES.EVENT]) {
    return <EventPalette />
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
