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

export const PositionTag = (props: TagProps) => {
  return (
    <Composition templateCols="1fr" gap={props.isLarge ? 10 : 5}>
      <Tag
        minimal
        fill
        large={props.isLarge}
        icon={IconNames.DOUBLE_CARET_HORIZONTAL}
      >
        <div style={{ textAlign: 'right', fontWeight: 'bold' }}>
          <Printer accessor={state => state[MSGID.POSITION_CURRENT].x} precision={2} /> mm
        </div>
      </Tag>
      <Tag
        minimal
        fill
        large={props.isLarge}
        icon={IconNames.DOUBLE_CARET_VERTICAL}
      >
        <div style={{ textAlign: 'right', fontWeight: 'bold' }}>
          <Printer accessor={state => state[MSGID.POSITION_CURRENT].y} precision={2} /> mm
        </div>
      </Tag>
      <Tag minimal fill large={props.isLarge} icon={IconNames.CARET_UP}>
        <div style={{ textAlign: 'right', fontWeight: 'bold' }}>
          <Printer accessor={state => state[MSGID.POSITION_CURRENT].z} precision={2} /> mm
        </div>
      </Tag>
    </Composition>
  )
}
