import React from 'react'

import { IconNames } from '@blueprintjs/icons'
import { Intent } from '@blueprintjs/core'
import { Button } from '@electricui/components-desktop-blueprint'
import { useHardwareState } from '@electricui/components-core'

import { MSGID, CONTROL_MODES, SUPERVISOR_STATES } from '../../../typedState'

export const EStopButton = () => {
  const supervisor = useHardwareState(
    state => state[MSGID.SUPERVISOR].supervisor,
  )

  const isArmed = supervisor === SUPERVISOR_STATES[SUPERVISOR_STATES.ARMED]

  return (
    <Button
      callback={MSGID.EMERGENCY_STOP}
      intent={Intent.DANGER}
      fill
      large
      minimal
      outlined
      active={isArmed}
    >
      E-STOP
    </Button>
  )
}
