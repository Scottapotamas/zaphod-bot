import { Intent, Tag } from '@blueprintjs/core'

import React from 'react'
import { Composition, Box } from 'atomic-layout'
import { NumberInput } from '@electricui/components-desktop-blueprint'
import { MSGID } from 'src/application/typedState'
import { IconNames } from '@blueprintjs/icons'

import { SectionHeading } from '../SectionHeading'

export const KinematicsLimits = () => {
  return (
    <React.Fragment>
      <SectionHeading text="SPEED LIMIT" />
      <NumberInput
        accessor={state => state[MSGID.USER_CONFIG].values.speed_limit}
        writer={(state, value) => {
          state[MSGID.USER_CONFIG].values.speed_limit = value
        }}
        min={0}
        max={750}
        intent={Intent.DANGER}
        rightElement={
          <Tag minimal>
            <b>mm/sec</b>
          </Tag>
        }
      />
      <br />

      <SectionHeading text="RESTRICT VOLUME" />
      <Composition templateCols="1fr" gap={5}>
        <NumberInput
          accessor={state => state[MSGID.USER_CONFIG].values.volume_radius}
          writer={(state, value) => {
            state[MSGID.USER_CONFIG].values.volume_radius = value
          }}
          min={0}
          max={300}
          intent={Intent.WARNING}
          rightElement={
            <Tag minimal>
              <b>RADIUS</b>
            </Tag>
          }
        />

        <NumberInput
          accessor={state => state[MSGID.USER_CONFIG].values.volume_z}
          writer={(state, value) => {
            state[MSGID.USER_CONFIG].values.volume_z = value
          }}
          min={0}
          max={300}
          intent={Intent.PRIMARY}
          rightElement={
            <Tag minimal>
              <b>HEIGHT</b>
            </Tag>
          }
        />
      </Composition>
    </React.Fragment>
  )
}
