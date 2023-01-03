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

import { Composition } from 'atomic-layout'
import { MSGID } from 'src/application/typedState'
import { OutlineCard } from 'src/application/components/OutlineCard'

export const DemoPalette = () => {
  return (
    <div>
      <OutlineCard padding="2em">
        <NonIdealState
          title="Demonstration Mode"
        >
          <Dropdown
            fill
            accessor={state => state[MSGID.ATTRACTOR_SPECIES]}
            writer={(state, value) => {
              state[MSGID.ATTRACTOR_SPECIES] = value
            }}
            placeholder={selectedOption =>
              selectedOption
                ? `Mode: ${selectedOption.text}`
                : 'Select a demo sequence'
            }
          >
            <Dropdown.Option value={0} text="Basic Moves" icon={IconNames.SHAPES} />
            <Dropdown.Option value={1} text="Random Points" icon={IconNames.PIVOT} />

            {/* TODO: add a dividier and menu here for "Strange Attractors" */}
            <Dropdown.Option value={10} text="Lorenz" icon={IconNames.DOT} />
            <Dropdown.Option value={11} text="Thomas" icon={IconNames.DOT} />
            <Dropdown.Option value={12} text="Aizawa" icon={IconNames.DOT} />
            <Dropdown.Option value={13} text="Dadras" icon={IconNames.DOT} />
            <Dropdown.Option value={14} text="Chen" icon={IconNames.DOT} />
            <Dropdown.Option value={15} text="Rossler" icon={IconNames.DOT} />
            <Dropdown.Option value={16} text="Halvorsen" icon={IconNames.DOT} />
            <Dropdown.Option value={17} text="Rabinovich-Fabrikant" icon={IconNames.DOT} />
            <Dropdown.Option value={18} text="Three Scroll" icon={IconNames.DOT} />
            <Dropdown.Option value={19} text="Sprott" icon={IconNames.DOT} />
            <Dropdown.Option value={20} text="FourWing" icon={IconNames.DOT} />
          </Dropdown>
        </NonIdealState>
      </OutlineCard>
    </div>
  )
}
