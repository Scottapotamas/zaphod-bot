import {
  Button,
  Statistic,
  Statistics,
} from '@electricui/components-desktop-blueprint'
import { CONTROL_MODES, SUPERVISOR_STATES } from '../../typedState'

import {
  IntervalRequester,
  useHardwareState,
} from '@electricui/components-core'

import { Composition, Box } from 'atomic-layout'
import React from 'react'

const SupervisorState = () => {
  const supervisor_state = useHardwareState(state => state.super.supervisor)
  const is_moving = useHardwareState(state => state.moStat.pathing_state)

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
        <Button fill large disabled intent="none" callback="disarm">
          Select a mode before arming
        </Button>
      )
    } else {
      return (
        <Button fill large intent="warning" callback="arm">
          Arm ({control_mode})
        </Button>
      )
    }
  }

  return (
    <Button fill large intent="primary" callback="disarm">
      Disarm
    </Button>
  )
}

const systemOverviewAreas = `
RobotMode PositionOutput
ArmButton HomeButton
`

const SystemController = () => {
  return (
    <div>
      <IntervalRequester variables={['cpos']} interval={50} />
      <Composition areas={systemOverviewAreas} gap={20} templateCols="4fr 3fr">
        {Areas => (
          <>
            <Areas.RobotMode>
              <Statistics>
                <Statistic
                  value={<SupervisorState />}
                  label={
                    <div>
                      <Printer accessor={state => state.super.mode} /> MODE
                    </div>
                  }
                />
              </Statistics>
            </Areas.RobotMode>
            <Areas.PositionOutput>
              <Composition
                templateCols="1fr 3fr"
                placeItems="center end"
                placeContent="space-around"
                gap={10}
                paddingHorizontal={40}
              >
                <Box>
                  <b>X</b>
                </Box>
                <Box>
                  <Printer accessor={state => state.cpos.x} precision={2} /> mm
                </Box>
                <Box>
                  <b>Y</b>
                </Box>
                <Box>
                  <Printer accessor={state => state.cpos.y} precision={2} /> mm
                </Box>
                <Box>
                  <b>Z</b>
                </Box>
                <Box>
                  <Printer accessor={state => state.cpos.z} precision={2} /> mm
                </Box>
              </Composition>
            </Areas.PositionOutput>
            <Areas.HomeButton>
              <Button fill large intent="success" callback="home">
                Home
              </Button>
            </Areas.HomeButton>
            <Areas.ArmButton>
              <ArmControlButton />
            </Areas.ArmButton>
          </>
        )}
      </Composition>
    </div>
  )
}

export default SystemController
