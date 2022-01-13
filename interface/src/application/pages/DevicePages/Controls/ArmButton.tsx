import React, { useCallback } from 'react'

import { IconNames } from '@blueprintjs/icons'
import { Icon, Intent, Tag } from '@blueprintjs/core'

import { Button } from '@electricui/components-desktop-blueprint'
import { Composition, Box } from 'atomic-layout'

import {
  useDeadline,
  useHardwareState,
  useSendCallback,
} from '@electricui/components-core'

import { MSGID, CONTROL_MODES, SUPERVISOR_STATES } from '../../../typedState'

export const ArmControlButton = () => {
  const supervisor = useHardwareState(
    state => state[MSGID.SUPERVISOR].supervisor,
  )
  const control_mode = useHardwareState(state => state[MSGID.SUPERVISOR].mode)

  const modeNotSelected = control_mode === CONTROL_MODES[CONTROL_MODES.NONE]
  const isArmed = supervisor === SUPERVISOR_STATES[SUPERVISOR_STATES.ARMED]

  if (!isArmed) {
    if (modeNotSelected) {
      return (
        <Button
          minimal
          outlined
          fill
          large
          disabled
          intent={Intent.NONE}
          icon={IconNames.WARNING_SIGN}
          callback={MSGID.DISARM}
        >
          Select a mode before arming
        </Button>
      )
    } else {
      return (
        <Button
          minimal
          outlined
          active
          fill
          large
          intent={Intent.DANGER}
          icon={IconNames.PLAY}
          callback={MSGID.ARM}
        >
          ARM ({control_mode})
        </Button>
      )
    }
  }

  return (
    <Button
      minimal
      outlined
      active
      fill
      large
      intent={Intent.WARNING}
      icon={IconNames.STOP}
      callback={MSGID.DISARM}
    >
      DISARM
    </Button>
  )
}

const tagContentLayout = `
  Icon Title
  Icon Subtitle
`

export const ArmControlTag = () => {
  const supervisor = useHardwareState(
    state => state[MSGID.SUPERVISOR].supervisor,
  )
  const control_mode = useHardwareState(state => state[MSGID.SUPERVISOR].mode)

  const modeNotSelected = control_mode === CONTROL_MODES[CONTROL_MODES.NONE]
  const isArmed = supervisor === SUPERVISOR_STATES[SUPERVISOR_STATES.ARMED]

  const supervisor_state = useHardwareState(
    state => state[MSGID.SUPERVISOR].supervisor,
  )
  const is_moving = useHardwareState(state => state[MSGID.MOTION].pathing_state)

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

  const sendCallback = useSendCallback()
  const getDeadline = useDeadline()

  const armButton = useCallback(
    (event: React.MouseEvent<HTMLElement, MouseEvent>) => {
      const cancellationToken = getDeadline()
      sendCallback(MSGID.ARM, cancellationToken, true)
    },
    [],
  )

  const disarmButton = useCallback(
    (event: React.MouseEvent<HTMLElement, MouseEvent>) => {
      const cancellationToken = getDeadline()
      sendCallback(MSGID.DISARM, cancellationToken, true)
    },
    [],
  )

  if (!isArmed) {
    if (modeNotSelected) {
      return (
        <Tag fill minimal intent={Intent.NONE} style={{ height: '100%' }}>
          <div style={{ position: 'absolute', left: '3em', top: '30%' }}>
          </div>

          <div style={{ textAlign: 'center', minWidth: '100px' }}>
          </div>
          <Composition
            areas={tagContentLayout}
            alignItems="center"
            justifyItems="center"
            justifyContent="space-evenly"
          >
            {Areas => (
              <React.Fragment>
                <Areas.Icon>
                <Icon icon={IconNames.SEARCH} size={42} />
                </Areas.Icon>
                <Areas.Title style={{ textAlign: 'center', minWidth: '100px' }}>
                <h2>WAITING <br/> FOR MODE</h2>
                </Areas.Title>

              </React.Fragment>
            )}
          </Composition>

        </Tag>
      )
    } else {
      return (
        <Tag
          fill
          minimal
          interactive
          onClick={armButton}
          intent={Intent.PRIMARY}
          style={{ height: '100%', border: '1px solid rgba(72, 175, 240, 0.6)' }}
        >
          <Composition
            areas={tagContentLayout}
            alignItems="center"
            justifyItems="center"
            justifyContent="space-evenly"
          >
            {Areas => (
              <React.Fragment>
                <Areas.Icon>
                  <Icon icon={IconNames.PLAY} size={42} />
                </Areas.Icon>
                <Areas.Title style={{ textAlign: 'center' }}>
                  <h2 style={{ marginBottom: '0.5em' }}>{supervisor_text}</h2>
                  <h3 style={{ marginTop: '0' }}>({control_mode})</h3>
                </Areas.Title>
                <Areas.Subtitle>
                  <div style={{ opacity: '0.6', textAlign: 'center', minWidth: '100px' }}>
                    CLICK TO ARM
                  </div>
                </Areas.Subtitle>
              </React.Fragment>
            )}
          </Composition>
        </Tag>
      )
    }
  }

  return (
    <Tag
      fill
      minimal
      interactive
      onClick={disarmButton}
      intent={Intent.WARNING}
      style={{ height: '100%',  border: '1px solid rgba(255, 179, 102, 0.6)' }}
    >
      <Composition
        areas={tagContentLayout}
        alignItems="center"
        justifyItems="center"
        justifyContent="space-evenly"
      >
        {Areas => (
          <React.Fragment>
            <Areas.Icon>
              <Icon icon={IconNames.HIGH_PRIORITY} size={42} />
            </Areas.Icon>
            <Areas.Title style={{ textAlign: 'center' }}>
              <h2 style={{ marginBottom: '0.5em' }}>{supervisor_text}</h2>
              <h3 style={{ marginTop: '0' }}>({control_mode})</h3>
            </Areas.Title>
            <Areas.Subtitle>
              <div style={{ opacity: '0.6', textAlign: 'center' }}>
                CLICK TO DISARM
              </div>
            </Areas.Subtitle>
          </React.Fragment>
        )}
      </Composition>
    </Tag>
  )
}
