import { Button } from '@electricui/components-desktop-blueprint'
import { CONTROL_MODES, SUPERVISOR_STATES } from '../../../typedState'

import {
  IntervalRequester,
  useHardwareState,
} from '@electricui/components-core'

import { Composition, Box } from 'atomic-layout'
import React from 'react'
import { Printer } from '@electricui/components-desktop'
import { IconNames } from '@blueprintjs/icons'

export const ArmControlButton = () => {
  const supervisor = useHardwareState<string>(state => state.super.supervisor)
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
          intent="none"
          icon={IconNames.WARNING_SIGN}
          callback="disarm"
        >
          Select a mode before arming
        </Button>
      )
    } else {
      return (
        <Button
          fill
          large
          intent="warning"
          icon={IconNames.PLAY}
          callback="arm"
        >
          Arm ({control_mode})
        </Button>
      )
    }
  }

  return (
    <Button fill large intent="primary" icon={IconNames.STOP} callback="disarm">
      Disarm
    </Button>
  )
}
