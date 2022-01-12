import React from 'react'

import { IconNames } from '@blueprintjs/icons'
import { Intent } from '@blueprintjs/core'

import { Button } from '@electricui/components-desktop-blueprint'

import {
  IntervalRequester,
  useHardwareState,
} from '@electricui/components-core'

import { MSGID, CONTROL_MODES, SUPERVISOR_STATES } from '../../../typedState'

export const ArmControlButton = () => {
  const supervisor = useHardwareState(state => state.super.supervisor)
  const control_mode = useHardwareState(state => state.super.mode)

  const modeNotSelected = control_mode === CONTROL_MODES[CONTROL_MODES.NONE]
  const isArmed = supervisor === SUPERVISOR_STATES[SUPERVISOR_STATES.ARMED]

  if (!isArmed) {
    if (modeNotSelected) {
      return (
        <Button
          fill
          large
          disabled
          intent={Intent.NONE}
          icon={IconNames.WARNING_SIGN}
          callback={MSGID.DISARM}
        >
          Select a mode before arming
        </Button>
      )
    } else {
      return (
        <Button
          fill
          large
          intent={Intent.WARNING}
          icon={IconNames.PLAY}
          callback={MSGID.ARM}
        >
          Arm ({control_mode})
        </Button>
      )
    }
  }

  return (
    <Button
      fill
      large
      intent={Intent.PRIMARY}
      icon={IconNames.STOP}
      callback={MSGID.DISARM}
    >
      Disarm
    </Button>
  )
}
