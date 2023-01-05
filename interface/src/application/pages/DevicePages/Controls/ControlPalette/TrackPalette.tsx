import React from 'react'

import { Composition, Box } from 'atomic-layout'
import { MSGID } from 'src/application/typedState'
import { OutlineCard } from 'src/application/components/OutlineCard'
import { TwoAxisPad } from 'src/application/components/TwoAxisPad'

export const TrackPalette = () => {
  return (
    <div style={{ maxWidth: 'fit-content' }}>
      <OutlineCard padding="1rem">
        <Composition gap={10} templateCols="auto 1fr">
          <Box>
          <p style={{ textAlign: 'center', opacity: '0.6' }}>
            XY
          </p>
          <TwoAxisPad
            // TODO: will using actual current position for handle xy make it feel better or worse?
            x={state => state[MSGID.POSITION_TARGET].x}
            y={state => state[MSGID.POSITION_TARGET].y}
            writer={(state, position) => {
              state[MSGID.POSITION_TARGET].x = position.horizontal
              state[MSGID.POSITION_TARGET].y = position.vertical
            }}
            throttleDuration={10}
            padCenterColor="rgba(19, 124, 189, 0.25)"
            padEdgeColor="rgba(19, 124, 189, 0.25)"
            handleColor="rgba(72, 175, 240, 0.6)" // Blue3 + opacity
            // handleSize={10}

            width={200}
            height={200}
            xMin={-100}
            xMax={100}
            yMin={-100}
            yMax={100}
          />
          </Box>
          <Box>
          <p style={{ textAlign: 'center', opacity: '0.6' }}>
            Z
          </p>
          <TwoAxisPad
            // TODO: will using actual current position for handle xy make it feel better or worse?
            x={state => 0}
            y={state => state[MSGID.POSITION_TARGET].z}
            writer={(state, position) => {
              state[MSGID.POSITION_TARGET].z = position.vertical
            }}
            throttleDuration={10}
            padCenterColor="rgba(19, 124, 189, 0.25)"
            padEdgeColor="rgba(19, 124, 189, 0.25)"
            handleColor="rgba(72, 175, 240, 0.6)" // Blue3 + opacity
            // handleSize={10}

            width={50}
            height={200}
            xMin={-1}
            xMax={1}
            yMin={0}
            yMax={100}
          />
          </Box>
        </Composition>
      </OutlineCard>
    </div>
  )
}
