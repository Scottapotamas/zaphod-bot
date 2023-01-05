import { ButtonGroup, Intent } from '@blueprintjs/core'
import { useHardwareState } from '@electricui/components-core'

import React from 'react'
import { Button } from '@electricui/components-desktop-blueprint'
import { MSGID } from 'src/application/typedState'
import { IconNames, IconName } from '@blueprintjs/icons'
import { SectionHeading } from '../SectionHeading'

type BoundaryButtonProps = {
  text: string
  icon: IconName
  value: number
}

const boundarySettings: BoundaryButtonProps[] = [
  {
    value: 0,
    text: 'Disabled',
    icon: IconNames.CROSS,
  },
  {
    value: 1,
    text: 'Notify',
    icon: IconNames.NOTIFICATIONS_UPDATED,
  },
  {
    value: 2,
    text: 'Halt',
    icon: IconNames.WALK,
  },
  {
    value: 3,
    text: 'Disarm',
    icon: IconNames.ERROR,
  },
]

export const BoundaryViolations = () => {
  const boundary_setting = useHardwareState(
    state => state[MSGID.USER_CONFIG].flags.boundary_violation_mode,
  )

  return (
    <React.Fragment>
      <SectionHeading text="BOUNDARY VIOLATIONS" />

      <ButtonGroup>
        {boundarySettings.map((option, index) => (
          <Button
            key={index}
            active={boundary_setting == option.value}
            outlined
            fill
            intent={
              boundary_setting == option.value ? Intent.WARNING : Intent.NONE
            }
            writer={state => {
              state[MSGID.USER_CONFIG].flags.boundary_violation_mode =
                option.value
            }}
            icon={option.icon}
          >
            {option.text.toUpperCase()}
          </Button>
        ))}
      </ButtonGroup>
    </React.Fragment>
  )
}
