import React from 'react'
import { Composition, Box, Only } from 'atomic-layout'
import {
  Tag,
} from '@blueprintjs/core'
import { IconNames } from '@blueprintjs/icons'
import { Printer } from '@electricui/components-desktop'
import { MSGID } from 'src/application/typedState'

type TagProps = {
  isLarge?: boolean
}

export const EffectorSpeedTag = (props: TagProps) => {
  return (
      <Tag
        minimal
        fill
        large={props.isLarge}
        icon={IconNames.PIVOT}
      >
        <div style={{ textAlign: 'right' }}>
          <Printer accessor={state => state[MSGID.POSITION_CURRENT].speed} precision={0} /> mm/s
        </div>
      </Tag>
  )
}
