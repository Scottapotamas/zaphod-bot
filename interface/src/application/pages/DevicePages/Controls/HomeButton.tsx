import React from 'react'

import { Composition, Box } from 'atomic-layout'

import { IconNames } from '@blueprintjs/icons'
import { Intent } from '@blueprintjs/core'
import { Button } from '@electricui/components-desktop-blueprint'

import {
  IntervalRequester,
  useHardwareState,
} from '@electricui/components-core'

import { MSGID } from '../../../typedState'

export const HomeButton = () => {
  return (
    <Button
      fill
      large
      intent={Intent.SUCCESS}
      icon={IconNames.HOME}
      callback={MSGID.HOME}
    >
      Home
    </Button>
  )
}
