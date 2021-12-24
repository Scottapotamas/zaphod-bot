import { NonIdealState, Callout, Card, Intent } from '@blueprintjs/core'
import { useHardwareState } from '@electricui/components-core'
import { Composition, Box } from 'atomic-layout'

import React from 'react'

import { CONTROL_MODES } from '../../../typedState'
import ManualJogPalette from './ControlPalette/ManualJogPalette'
import { TrackPalette } from './ControlPalette/TrackPalette'
import { EventPalette } from './ControlPalette/EventPalette'

import { MSGID } from '../../../typedState'
import { NINJA } from '@blueprintjs/icons/lib/esm/generated/iconContents'

export const ControlBlock = () => {
  const control_mode = useHardwareState(state => state.super.mode)

  if (control_mode == CONTROL_MODES[CONTROL_MODES.NONE]) {
    return (
      <Callout icon={null} intent={Intent.WARNING}>
        <NonIdealState
          title="Select desired mode"
          description="Select a control mode before arming"
          action={<p>blah this is a thing</p>}
        >
          </NonIdealState>
      </Callout>
    )
  } else if (control_mode == CONTROL_MODES[CONTROL_MODES.MANUAL]) {
    return (
      <Card>
        <ManualJogPalette />
      </Card>
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
