import React from 'react'

import { Dropdown, Button } from '@electricui/components-desktop-blueprint'
import { MSGID, CONTROL_MODES } from 'src/application/typedState'
import { Icon, Colors, NonIdealState, ButtonGroup } from '@blueprintjs/core'
import { useHardwareState } from '@electricui/components-core'

export const ModeSelection = () => {
  const control_mode = useHardwareState(state => state.super.mode)

  const largeButtons: boolean = false

  return (
    <React.Fragment>
      <ButtonGroup fill>
        <Button
          minimal
          large={largeButtons}
          icon="move"
          writer={state => {
            state[MSGID.MODE_REQUEST] = 1
          }}
          active={control_mode == CONTROL_MODES[CONTROL_MODES.MANUAL]}
        >
          Jog
        </Button>
        <Button
          minimal
          large={largeButtons}
          icon="flow-end"
          writer={state => {
            state[MSGID.MODE_REQUEST] = 3
          }}
          active={control_mode == CONTROL_MODES[CONTROL_MODES.TRACK]}
        >
          Track
        </Button>
        <Button
          minimal
          large={largeButtons}
          icon="projects"
          writer={state => {
            state[MSGID.MODE_REQUEST] = 4
          }}
          active={control_mode == CONTROL_MODES[CONTROL_MODES.DEMO]}
        >
          Demo
        </Button>
        <Button
          minimal
          large={largeButtons}
          icon="document-open"
          writer={state => {
            state[MSGID.MODE_REQUEST] = 2
          }}
          active={control_mode == CONTROL_MODES[CONTROL_MODES.EVENT]}
        >
          Blender
        </Button>
      </ButtonGroup>
    </React.Fragment>
  )
}

export const ModeSelectionDropdown = () => {
  const control_mode = useHardwareState(state => state.super.mode)

  return (
    <React.Fragment>
      <Dropdown
        accessor={state => state[MSGID.MODE_REQUEST]}
        writer={(state, value) => {
          state[MSGID.MODE_REQUEST] = value
        }}
        placeholder={selectedOption =>
          selectedOption ? `Mode: ${selectedOption.text}` : 'Select Mode'
        }
      >
        <Dropdown.Option
          value={1}
          text="Manual"
          labelElement={
            <>
              <Icon icon="move" color={Colors.GREEN4} />
            </>
          }
        />
        {/* CONTROL_MODES[CONTROL_MODES.MANUAL] */}

        <Dropdown.Option
          value={3}
          text="Track"
          labelElement={
            <>
              <Icon icon="flow-end" color={Colors.RED4} />
            </>
          }
        />
        {/* CONTROL_MODES[CONTROL_MODES.TRACK] */}

        <Dropdown.Option
          value={4}
          text="Demo"
          labelElement={
            <>
              <Icon icon="projects" color={Colors.GOLD4} />
            </>
          }
        />
        {/* CONTROL_MODES[CONTROL_MODES.DEMO] */}

        <Dropdown.Option
          value={2}
          text="Run Program"
          labelElement={
            <>
              <Icon icon="document-open" color={Colors.BLUE4} />
            </>
          }
        />
        {/* CONTROL_MODES[CONTROL_MODES.EVENT] */}
      </Dropdown>
    </React.Fragment>
  )
}
