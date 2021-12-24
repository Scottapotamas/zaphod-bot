import React from 'react'

import { Dropdown } from '@electricui/components-desktop-blueprint'
import { CONTROL_MODES } from '../../../../typedState'
import { Icon, Colors, NonIdealState } from '@blueprintjs/core'
import { useHardwareState } from '@electricui/components-core'

export const ModeSelection = () => {
  const control_mode = useHardwareState(state => state.super.mode)

  return (
    <React.Fragment>
      <Dropdown
        accessor={state => state.req_mode}
        writer={(state, value) => {
            state.req_mode = value
          }}
          placeholder={selectedOption =>
            selectedOption ? `Mode: ${selectedOption.text}` : 'Select Mode'
          }      >
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
