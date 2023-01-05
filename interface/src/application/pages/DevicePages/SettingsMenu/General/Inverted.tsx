import { ButtonGroup, Intent } from '@blueprintjs/core'
import { useHardwareState } from '@electricui/components-core'

import React from 'react'
import { Button } from '@electricui/components-desktop-blueprint'
import { MSGID } from 'src/application/typedState'
import { IconNames } from '@blueprintjs/icons'
import { SectionHeading } from '../SectionHeading'

export const InvertedSetting = () => {
  const inverted_setting = useHardwareState(
    state => state[MSGID.USER_CONFIG].flags.inverted,
  )!

  return (
    <React.Fragment>
      <SectionHeading text="Orientation"/>

      <ButtonGroup>
        <Button
          active={!inverted_setting}
          outlined
          fill
          intent={inverted_setting ? Intent.NONE : Intent.SUCCESS}
          writer={state => {
            state[MSGID.USER_CONFIG].flags.inverted = false
          }}
          icon={IconNames.CARET_UP}
        >
          NORMAL
        </Button>
        <Button
          active={inverted_setting}
          outlined
          fill
          intent={inverted_setting ? Intent.SUCCESS : Intent.NONE}
          writer={state => {
            state[MSGID.USER_CONFIG].flags.inverted = true
          }}
          icon={IconNames.CARET_DOWN}
        >
          INVERTED
        </Button>
      </ButtonGroup>
    </React.Fragment>
  )
}
