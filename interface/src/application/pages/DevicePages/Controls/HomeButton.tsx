import { Button } from '@electricui/components-desktop-blueprint'
import { CONTROL_MODES, SUPERVISOR_STATES } from '../../../typedState'

import {
  IntervalRequester,
  useHardwareState,
} from '@electricui/components-core'

import { IconNames } from '@blueprintjs/icons'

import { Composition, Box } from 'atomic-layout'
import React from 'react'

export const HomeButton = () => {
  return (
    <Button fill large intent="success" icon={IconNames.HOME} callback="home">
      Home
    </Button>
  )
}
