import React from 'react'

import { IconNames } from '@blueprintjs/icons'
import { Intent } from '@blueprintjs/core'
import { Button } from '@electricui/components-desktop-blueprint'

import { MSGID } from '../../../typedState'

export const HomeButton = () => {
  return (
    <Button
      fill
      large
      minimal
      outlined
      active
      intent={Intent.SUCCESS}
      icon={IconNames.HOME}
      callback={MSGID.HOME}
    >
      HOME
    </Button>
  )
}
