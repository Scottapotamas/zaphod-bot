import React, { useState } from 'react'

import { Button, Slider, NumberInput } from '@electricui/components-desktop-blueprint'
import {
  NonIdealState,
  Icon,
  MultiSlider,
  NumericInput,
  Callout,
  Intent,
} from '@blueprintjs/core'

import { Composition } from 'atomic-layout'
import { MSGID } from 'src/application/typedState'
import { OutlineCard } from 'src/application/components/OutlineCard'

export const DemoPalette = () => {
  return (
    <div>
      <OutlineCard padding="2em">
        <NonIdealState
          title="Demonstration Stuff goes here"
          description="Begins automatically once armed"
        />
        <br/>
        <NumberInput
          accessor={state => state[MSGID.ATTRACTOR_SPECIES]}
          writer={(state, value) => {
            state[MSGID.ATTRACTOR_SPECIES] = value
          }}
          min={0}
          max={5}
          intent={Intent.PRIMARY}
        />
      </OutlineCard>
    </div>
  )
}
