import React from 'react'
import { RouteComponentProps } from '@reach/router'
import {
  ProgressBar,
  Slider,
  Statistics,
  Statistic,
  Button,
  Switch,
} from '@electricui/components-desktop-blueprint'
import { Printer } from '@electricui/components-desktop'
import {
  Card,
  Divider,
  ButtonGroup,
  Label,
  Text,
  HTMLTable,
} from '@blueprintjs/core'
import { Grid, Cell } from 'styled-css-grid'
import { Chart } from '@electricui/components-desktop-charts'
import { useDarkMode } from '@electricui/components-desktop'
import {
  IntervalRequester,
  useHardwareState,
  StateTree,
} from '@electricui/components-core'
import { CALL_CALLBACK } from '@electricui/core'
import { SUPERVISOR_STATES } from './../../../transport-manager/config/codecs'

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

const MotorSafetyMode = () => {
  const motor_state = useHardwareState(state => state.super.motors)
  var motors_are_active: string = 'null'

  switch (motor_state) {
    case 0: {
      motors_are_active = 'OFF'
      break
    }
    case 1: {
      motors_are_active = 'ON'
      break
    }
    default: {
      motors_are_active = 'INVALID'
      break
    }
  }

  return <div>{motors_are_active}</div>
}

const ActiveControlMode = () => {
  const mode = useHardwareState(state => state.super.mode)
  var mode_text: string = 'null'

  switch (mode) {
    case 0: {
      mode_text = 'NONE'
      break
    }
    case 1: {
      mode_text = 'EVENT'
      break
    }
    case 2: {
      mode_text = 'TRACK'
      break
    }
    case 3: {
      mode_text = 'DEMO'
      break
    }
    case 3: {
      mode_text = 'CHANGING'
      break
    }
    default: {
      mode_text = 'INVALID'
      break
    }
  }

  return <div>{mode_text} MODE</div>
}

const ArmControlButton = () => {
  const supervisor = useHardwareState<string>(state => state.super.supervisor)

  const isArmed = supervisor === SUPERVISOR_STATES[SUPERVISOR_STATES.ARMED]

  if (!isArmed) {
    return (
      <Button fill large intent="warning" writer={{ arm: CALL_CALLBACK }}>
        Arm
      </Button>
    )
  }

  return (
    <Button fill large intent="primary" writer={{ disarm: CALL_CALLBACK }}>
      Disarm ({supervisor})
    </Button>
  )
}

const ControlPage = (props: RouteComponentProps) => {
  const isDarkMode = useDarkMode()

  return (
    <React.Fragment>
      <IntervalRequester
        interval={125}
        variables={['moStat', 'super', 'cpos']}
      />

      <Grid columns={2}>
        <Cell>
          <Card>
            <Button writer={{ revent: CALL_CALLBACK }}>Event Mode</Button>
            <br />
            <Button writer={{ rdemo: CALL_CALLBACK }}>Demo Mode</Button>
            <br />

            <br />
            <Button
              writer={{
                inmv: {
                  type: 0,
                  reference: 1,
                  id: 0,
                  duration: 400,
                  points: [[0, 0, 5]],
                },
                qumv: CALL_CALLBACK,
              }}
            >
              Up
            </Button>

            <Button
              writer={{
                inmv: {
                  type: 0,
                  reference: 1,
                  id: 0,
                  duration: 400,
                  points: [[0, 0, -5]],
                },
                qumv: CALL_CALLBACK,
              }}
            >
              Down
            </Button>
            <br />
            <Button
              writer={{
                inmv: {
                  type: 0,
                  reference: 1,
                  id: 0,
                  duration: 400,
                  num_points: 1,
                  points: [[5, 0, 0]],
                },
                qumv: CALL_CALLBACK,
              }}
            >
              +x
            </Button>

            <Button
              writer={{
                inmv: {
                  type: 0,
                  reference: 1,
                  id: 0,
                  duration: 400,
                  num_points: 1,
                  points: [[-5, 0, 0]],
                },
                qumv: CALL_CALLBACK,
              }}
            >
              -x
            </Button>
            <br />
            <Button
              writer={{
                inmv: {
                  type: 0,
                  reference: 1,
                  id: 0,
                  duration: 400,
                  num_points: 1,
                  points: [[0, 5, 0]],
                },
                qumv: CALL_CALLBACK,
              }}
            >
              +y
            </Button>
            <Button
              writer={{
                inmv: {
                  type: 0,
                  reference: 1,
                  id: 0,
                  duration: 400,
                  num_points: 1,
                  points: [[0, -5, 0]],
                },
                qumv: CALL_CALLBACK,
              }}
            >
              -y
            </Button>
            <br />
            <br />
            <br />
            <br />
            <br />

            <Button
              writer={{
                inmv: {
                  type: 0, //transit
                  reference: 0, //absolute
                  id: 0,
                  duration: 500,
                  num_points: 1,
                  points: [[+20, +20, 40]],
                },
                qumv: CALL_CALLBACK,
              }}
            >
              Prepare
            </Button>
            <br />

            <Button
              writer={{
                inmv: {
                  type: 2, //catmull
                  reference: 0,
                  id: 0,
                  duration: 1000,
                  num_points: 4,
                  points: [
                    [-20, +20, 40], //D
                    [+20, +20, 40], //A
                    [+20, -20, 80], //B
                    [-20, -20, 80], //C
                  ],
                },
                qumv: CALL_CALLBACK,
              }}
            >
              Catmull 1
            </Button>
            <Button
              writer={{
                inmv: {
                  type: 2, //catmull
                  reference: 0,
                  id: 0,
                  duration: 1000,
                  num_points: 4,
                  points: [
                    [+20, +20, 40], //A
                    [+20, -20, 80], //B
                    [-20, -20, 80], //C
                    [-20, +20, 40], //D
                  ],
                },
                qumv: CALL_CALLBACK,
              }}
            >
              Catmull 2
            </Button>
            <Button
              writer={{
                inmv: {
                  type: 2, //catmull
                  reference: 0,
                  id: 0,
                  duration: 1000,
                  num_points: 4,
                  points: [
                    [+20, -20, 80], //B
                    [-20, -20, 80], //C
                    [-20, +20, 40], //D
                    [+20, +20, 40], //A
                  ],
                },
                qumv: CALL_CALLBACK,
              }}
            >
              Catmull 3
            </Button>
            <Button
              writer={{
                inmv: {
                  type: 2, //catmull
                  reference: 0,
                  id: 0,
                  duration: 1000,
                  num_points: 4,
                  points: [
                    [-20, -20, 80], //C
                    [-20, +20, 40], //D
                    [+20, +20, 40], //A
                    [+20, -20, 80], //B
                  ],
                },
                qumv: CALL_CALLBACK,
              }}
            >
              Catmull 4
            </Button>
          </Card>
        </Cell>
        <Cell>
          <Card>
            <h3>RGB</h3>
          </Card>
        </Cell>
        <Cell>
          <Card>
            <br />
            <br />
            <br />
            <ProgressBar accessor={state => state.moStat.move_progress} />
            <br />
            <Grid columns={2}>
              <Cell center middle>
                <Statistics>
                  <Statistic
                    value={<SupervisorState />}
                    label={<ActiveControlMode />}
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
                <Button
                  fill
                  large
                  intent="success"
                  writer={{ home: CALL_CALLBACK }}
                >
                  Home
                </Button>
              </Cell>
            </Grid>
          </Card>
        </Cell>
      </Grid>
    </React.Fragment>
  )
}

export default ControlPage
