import React, { useState } from 'react'

import {
  Dropdown,
  Button,
  Slider,
  NumberInput,
} from '@electricui/components-desktop-blueprint'
import {
  NonIdealState,
  Icon,
  MultiSlider,
  NumericInput,
  Callout,
  Intent,
} from '@blueprintjs/core'
import { IconNames } from '@blueprintjs/icons'

import { Composition, Box } from 'atomic-layout'
import { MSGID } from 'src/application/typedState'
import { OutlineCard } from 'src/application/components/OutlineCard'
import { useHardwareState } from '@electricui/components-core'

type AttractorProps = {
  name: string
  seqNum: number
}

const strangeAttractors: AttractorProps[] = [
  { name: 'Lorenz', seqNum: 10 },
  { name: 'Thomas', seqNum: 11 },
  { name: 'Aizawa', seqNum: 12 },
  { name: 'Dadras', seqNum: 13 },
  { name: 'Chen', seqNum: 14 },
  { name: 'Rossler', seqNum: 15 },
  { name: 'Halvorsen', seqNum: 16 },
  { name: 'Three Scroll', seqNum: 18 },
  { name: 'Sprott', seqNum: 19 },
  { name: 'FourWing', seqNum: 20 },
  { name: 'Rabinovich-Fabrikant', seqNum: 17 },
]

export const DemoPalette = () => {
  const selected_demo = useHardwareState(
    state => state[MSGID.ATTRACTOR_SPECIES],
  )

  return (
    <div>
      <OutlineCard padding="2em">
        <Composition gap={20} justifyItems="center">
          <p style={{ marginBottom: '0', opacity: '0.6' }}>
            DEMONSTRATION PROGRAMS
          </p>
          <Composition
            templateCols="1fr 1fr"
            gap={10}
            style={{ width: '100%' }}
          >
            <Button
              active={selected_demo == 0}
              outlined
              large
              fill
              intent={selected_demo == 0 ? Intent.WARNING : Intent.NONE}
              writer={state => {
                state[MSGID.ATTRACTOR_SPECIES] = 0
              }}
              icon={IconNames.SHAPES}
            >
              BASIC MOVES
            </Button>

            <Button
              active={selected_demo == 1}
              outlined
              large
              fill
              intent={selected_demo == 1 ? Intent.WARNING : Intent.NONE}
              writer={state => {
                state[MSGID.ATTRACTOR_SPECIES] = 1
              }}
              icon={IconNames.PIVOT}
            >
              RANDOM POINTS
            </Button>
          </Composition>

          <p style={{ marginBottom: '0', opacity: '0.6' }}>
            STRANGE ATTRACTORS
          </p>
          <Composition templateCols="1fr 1fr 1fr" gap={10} style={{ width: '100%' }}>
            {strangeAttractors.map((attractor, index )=> (
              <Button
                key={index}
                active={selected_demo == attractor.seqNum}
                outlined
                fill
                intent={
                  selected_demo == attractor.seqNum
                    ? Intent.WARNING
                    : Intent.NONE
                }
                writer={state => {
                  state[MSGID.ATTRACTOR_SPECIES] = attractor.seqNum
                }}
                {...{
                  style: {
                    gridColumnEnd: attractor.seqNum == 17 ? 'span 2' : '',
                  },
                }}
              >
                {attractor.name.toLocaleUpperCase()}
              </Button>
            ))}
          </Composition>
        </Composition>
      </OutlineCard>
    </div>
  )
}
