import React from 'react'

import { Composition, Box } from 'atomic-layout'

import {
  Card,
  Icon,
  Colors,
  Tag,
  Callout,
  Intent,
  IconName,
  Tooltip,
} from '@blueprintjs/core'
import { IconNames } from '@blueprintjs/icons'
import { useHardwareState } from '@electricui/components-core'
import { Printer } from '@electricui/components-desktop'

import { MSGID, CONTROL_MODES, SUPERVISOR_STATES } from '../../../typedState'

const layoutDescription = `
            State XAxis
            State YAxis
            Mode ZAxis
          `

type RobotSummaryProps = {
  isLarge?: boolean
}

export const RobotSummary = (props: RobotSummaryProps) => {
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

  return (
    <React.Fragment>
      <Composition
        areas={layoutDescription}
        templateCols="1fr 100px"
        gap={props.isLarge ? 10 : 5}
      >
        {Areas => (
          <React.Fragment>
            <Areas.State>
              <Tag
                round
                fill
                intent={Intent.NONE}
                icon={IconNames.TICK_CIRCLE}
                style={{ height: '100%' }}
              >
                <div style={{ textAlign: 'center' }}>
                  <h2>{supervisor_text}</h2>
                </div>
              </Tag>
            </Areas.State>

            <Areas.Mode>
              <Tag
                minimal
                round
                fill
                intent={Intent.NONE}
                icon={IconNames.ZOOM_TO_FIT}
                style={{ height: '100%' }}
              >
                <div style={{ textAlign: 'center' }}>
                  <Printer accessor={state => state.super.mode} /> MODE
                </div>
              </Tag>
            </Areas.Mode>

            <Areas.XAxis>
              <Tag minimal fill large={props.isLarge} icon={IconNames.CROSS}>
                <div style={{ textAlign: 'right' }}>
                  <Printer accessor={state => state.cpos.x} precision={2} /> mm
                </div>
              </Tag>
            </Areas.XAxis>
            <Areas.YAxis>
              <Tag minimal fill large={props.isLarge} icon={IconNames.CROSS}>
                <div style={{ textAlign: 'right' }}>
                  <Printer accessor={state => state.cpos.y} precision={2} /> mm
                </div>
              </Tag>
            </Areas.YAxis>
            <Areas.ZAxis>
              <Tag minimal fill large={props.isLarge} icon={IconNames.CROSS}>
                <div style={{ textAlign: 'right' }}>
                  <Printer accessor={state => state.cpos.z} precision={2} /> mm
                </div>
              </Tag>
            </Areas.ZAxis>
          </React.Fragment>
        )}
      </Composition>
    </React.Fragment>
  )
}
