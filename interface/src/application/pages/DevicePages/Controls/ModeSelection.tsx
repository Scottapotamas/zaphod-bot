import React from 'react'

import { Button } from '@electricui/components-desktop-blueprint'
import { Intent, IconName } from '@blueprintjs/core'
import { useHardwareState } from '@electricui/components-core'
import { Composition } from 'atomic-layout'

import { MSGID, CONTROL_MODES } from 'src/application/typedState'

interface ModeButton {
  label: string
  icon: IconName  // TODO: consider using the properly typed icons
  value: CONTROL_MODES
}

// Each of the 4 user buttons to change mode
const buttons: ModeButton[] = [
  { label: 'Jog', icon: 'move', value: CONTROL_MODES.MANUAL },
  { label: 'Track', icon: 'flow-end', value: CONTROL_MODES.TRACK },
  { label: 'Demo', icon: 'projects', value: CONTROL_MODES.DEMO },
  { label: 'Blender', icon: 'document-open', value: CONTROL_MODES.EVENT },
]

interface ModeButtonProps {
  config: ModeButton
  activeState: string
}

const ModeButtonComponent = (props: ModeButtonProps) => {
  return (
    <Button
      active={props.activeState == CONTROL_MODES[props.config.value]}
      outlined
      intent={
        props.activeState == CONTROL_MODES[props.config.value]
          ? Intent.PRIMARY
          : Intent.NONE
      }
      icon={props.config.icon}
      writer={state => {
        state[MSGID.MODE_REQUEST] = props.config.value
      }}
    >
      {props.config.label.toUpperCase()}
    </Button>
  )
}

// The 4-buttons in a formatted row
export const ModeSelection = () => {
  const control_mode =
    useHardwareState(state => state[MSGID.SUPERVISOR].mode) ??
    CONTROL_MODES[CONTROL_MODES.NONE]

  return (
    <React.Fragment>
      <Composition templateCols="1fr 1fr 1fr 1fr" gapCol={10}>
        {buttons.map(buttonConfig => (
          <ModeButtonComponent
            config={buttonConfig}
            activeState={control_mode}
            key={buttonConfig.value}
          />
        ))}
      </Composition>
    </React.Fragment>
  )
}
