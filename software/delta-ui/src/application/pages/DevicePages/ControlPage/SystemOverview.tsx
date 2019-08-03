import React from 'react'

import {
  IntervalRequester,
  useHardwareState,
  StateTree,
} from '@electricui/components-core'
import {
  ProgressBar,
  Statistics,
  Statistic,
  Button,
} from '@electricui/components-desktop-blueprint'
import { Printer } from '@electricui/components-desktop'
import { Card, Divider, HTMLTable } from '@blueprintjs/core'
import { Grid, Cell } from 'styled-css-grid'

import { CALL_CALLBACK } from '@electricui/core'
import {
  SUPERVISOR_STATES,
  CONTROL_MODES,
} from './../../../../transport-manager/config/codecs'

const SupervisorState = () => {
  const supervisor_state = useHardwareState(state => state.super.supervisor)
  const is_moving = useHardwareState(state => state.moStat.move_state)

  let supervisor_text: string = 'null'

  if (supervisor_state === SUPERVISOR_STATES[SUPERVISOR_STATES.ARMED]) {
    if (is_moving == 1) {
      supervisor_text = 'MOVING'
    } else {
      supervisor_text = 'ARMED'
    }
  } else {
    supervisor_text = supervisor_state || 'UNKNOWN'
  }

  return <div>{supervisor_text}</div>
}

const ArmControlButton = () => {
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
          writer={{ disarm: CALL_CALLBACK }}
        >
          Select a mode before arming
        </Button>
      )
    } else {
      return (
        <Button fill large intent="warning" writer={{ arm: CALL_CALLBACK }}>
          Arm ({control_mode})
        </Button>
      )
    }
  }

  return (
    <Button fill large intent="primary" writer={{ disarm: CALL_CALLBACK }}>
      Disarm
    </Button>
  )
}

const SystemOverview = () => {
  const control_mode = useHardwareState(state => state.super.mode)

  return (
    <Card>
      <ProgressBar accessor={state => state.moStat.move_progress} />
      <br />
      <Grid columns={2}>
        <Cell center middle>
          <Statistics>
            <Statistic
              value={<SupervisorState />}
              label={<div>{control_mode} MODE</div>}
            />
          </Statistics>
        </Cell>
        <Cell>
          <HTMLTable condensed style={{ minWidth: '100%' }}>
            <tbody>
              <tr>
                <td>X</td>
                <td>
                  <Printer accessor={state => state.cpos[0] / 1000} /> mm
                </td>
              </tr>
              <tr>
                <td>Y</td>
                <td>
                  <Printer accessor={state => state.cpos[1] / 1000} /> mm
                </td>
              </tr>
              <tr>
                <td>Z</td>
                <td>
                  <Printer accessor={state => state.cpos[2] / 1000} /> mm
                </td>
              </tr>
            </tbody>
          </HTMLTable>
        </Cell>
      </Grid>
      <br />
      <Grid columns={2}>
        <Cell>
          <ArmControlButton />
        </Cell>
        <Cell>
          <Button fill large intent="success" writer={{ home: CALL_CALLBACK }}>
            Home
          </Button>
        </Cell>
      </Grid>
    </Card>
  )
}

export default SystemOverview
