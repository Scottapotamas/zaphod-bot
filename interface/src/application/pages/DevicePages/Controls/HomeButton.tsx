import React from 'react'

import { IconNames } from '@blueprintjs/icons'
import { Intent } from '@blueprintjs/core'
import { Button } from '@electricui/components-desktop-blueprint'

import { MSGID, SUPERVISOR_STATES } from '../../../typedState'
import { useHardwareState } from '@electricui/components-core'

type HomeButtonProps = {
  label?: string
}

export const HomeButton = (props: HomeButtonProps) => {

  const supervisor = useHardwareState(
    state => state[MSGID.SUPERVISOR].supervisor,
  )

  const isArmed = supervisor === SUPERVISOR_STATES[SUPERVISOR_STATES.ARMED]


  return (
    <Button
      fill
      large
      minimal
      outlined
      disabled={!isArmed}
      // active={isArmed}
      intent={Intent.SUCCESS}
      icon={IconNames.HOME}
      callback={MSGID.HOME}
      style={{height: '100%'}}
    >
      {props.label}
    </Button>
  )
}
