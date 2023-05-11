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
import { PositionTag } from './SummaryTags/PositionTag'
import { EffectorSpeedTag } from './SummaryTags/EffectorSpeedTag'
import { HomeButton } from '../Controls/HomeButton'
import { ArmControlTag } from '../Controls/ArmButton'


const layoutDescription = `
            State Home Position
          `

type RobotSummaryProps = {
  isLarge?: boolean
}

export const RobotSummary = (props: RobotSummaryProps) => {
  const supervisor_state = useHardwareState(
    state => state[MSGID.SUPERVISOR].supervisor,
  )
  
  // TODO: update with a new 'is moving' field
  let is_moving = 0 //useHardwareState(state => state[MSGID.MOTION].pathing_state)

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
        templateCols="1fr 40px 125px"
        gap={props.isLarge ? 10 : 5}
      >
        {Areas => (
          <React.Fragment>
            <Areas.State>
              <ArmControlTag />
            </Areas.State>

            <Areas.Mode>
              <Tag
                minimal
                round
                fill
                intent={Intent.NONE}
                style={{ height: '100%' }}
              >
                <div style={{ textAlign: 'center' }}>
                  <Printer accessor={state => state[MSGID.SUPERVISOR].mode} />{' '}
                  MODE
                </div>
              </Tag>
            </Areas.Mode>
            <Areas.Home>
              <HomeButton label="" />
            </Areas.Home>

            <Areas.Position>
              <Composition templateCols="1fr" gap={props.isLarge ? 10 : 5}>
                <PositionTag isLarge={props.isLarge} />
                <EffectorSpeedTag isLarge={props.isLarge} />
              </Composition>
            </Areas.Position>
          </React.Fragment>
        )}
      </Composition>
    </React.Fragment>
  )
}
