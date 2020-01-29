import React, { useState } from 'react'

import { Icon, MultiSlider, NumericInput } from '@blueprintjs/core'
import { Composition } from 'atomic-layout'

import { Button } from '@electricui/components-desktop-blueprint'
import { CALL_CALLBACK } from '@electricui/core'

import LEDControlPalette from '../../../components/LEDControlPalette'

const JogButtonAreas = `
XP YP ZP
YM XM ZM
`

interface JogPaletteProps {
  distance: Number
  time: Number
}

const JogPalette = (props: JogPaletteProps) => {
  return (
    <Composition areas={JogButtonAreas} gap={5} templateCols="auto auto 1fr">
      {({ XP, YP, ZP, XM, YM, ZM }) => (
        <React.Fragment>
          <XP>
            <Button
              large
              writer={{
                inmv: {
                  type: 0,
                  reference: 1,
                  id: 0,
                  duration: props.time,
                  points: [[props.distance, 0, 0]],
                },
              }}
            >
              <Icon icon="arrow-top-left" iconSize={50} />
            </Button>
          </XP>
          <YP>
            <Button
              large
              writer={{
                inmv: {
                  type: 0,
                  reference: 1,
                  id: 0,
                  duration: props.time,
                  points: [[0, props.distance, 0]],
                },
              }}
            >
              <Icon icon="arrow-top-right" iconSize={50} />
            </Button>
          </YP>
          <XM>
            <Button
              large
              writer={{
                inmv: {
                  type: 0,
                  reference: 1,
                  id: 0,
                  duration: props.time,
                  points: [[-props.distance, 0, 0]],
                },
              }}
            >
              <Icon icon="arrow-bottom-right" iconSize={50} />
            </Button>
          </XM>
          <YM>
            <Button
              large
              writer={{
                inmv: {
                  type: 0,
                  reference: 1,
                  id: 0,
                  duration: props.time,
                  points: [[0, -props.distance, 0]],
                },
              }}
            >
              <Icon icon="arrow-bottom-left" iconSize={50} />
            </Button>
          </YM>
          <ZP>
            <Button
              large
              fill
              writer={{
                inmv: {
                  type: 0,
                  reference: 1,
                  id: 0,
                  duration: props.time,
                  points: [[0, 0, props.distance]],
                },
              }}
            >
              <Icon icon="arrow-up" iconSize={50} />
            </Button>
          </ZP>
          <ZM>
            <Button
              large
              fill
              writer={{
                inmv: {
                  type: 0,
                  reference: 1,
                  id: 0,
                  duration: props.time,
                  points: [[0, 0, -props.distance]],
                },
              }}
            >
              <Icon icon="arrow-down" iconSize={50} />
            </Button>
          </ZM>
        </React.Fragment>
      )}
    </Composition>
  )
}

const controllerAreas = `
SpeedArea DistanceArea KeypadArea
SpeedArea DistanceArea LightingArea
`

const ManualJogPalette = () => {
  let [jog_speed, setJogSpeed] = useState(0)
  let [jog_distance, setJogDistance] = useState(5.0)

  let jog_duration = 600 * (1 / (1 + jog_speed))

  return (
    <Composition
      areas={controllerAreas}
      gap={30}
      padding={15}
      templateCols="1fr 1fr 2fr"
    >
      {({ SpeedArea, DistanceArea, KeypadArea, LightingArea }) => (
        <React.Fragment>
          <SpeedArea>
            <b>Speed</b>
            <br />
            <br />
            <MultiSlider
              min={-0.75}
              max={1}
              stepSize={0.25}
              vertical
              labelRenderer={val => `${Math.round((val + 1) * 100)}%`}
              labelStepSize={0.5}
              showTrackFill={jog_speed !== 0}
            >
              <MultiSlider.Handle
                value={jog_speed}
                intentAfter={jog_speed < 0 ? 'warning' : undefined}
                intentBefore={jog_speed > 0 ? 'warning' : undefined}
                onChange={value => setJogSpeed(value)}
              />
              <MultiSlider.Handle value={0} interactionKind="none" />
            </MultiSlider>
            <br />
            <NumericInput
              value={jog_speed}
              onValueChange={value => setJogSpeed(value)}
              fill
              intent="warning"
              min={-1}
              max={1}
              stepSize={0.25}
            />
          </SpeedArea>
          <DistanceArea>
            <b>Distance</b>
            <br />
            <br />
            <MultiSlider
              onChange={values => setJogDistance(values[0])}
              min={0}
              max={10}
              stepSize={1}
              vertical
              labelRenderer={val => `${val}mm`}
              labelStepSize={5}
            >
              <MultiSlider.Handle value={jog_distance} intentBefore="success" />
            </MultiSlider>
            <br />
            <NumericInput
              value={jog_distance}
              onValueChange={value => setJogDistance(value)}
              fill
              intent="success"
              min={0}
              max={10}
              stepSize={1}
            />
          </DistanceArea>
          <KeypadArea>
            <JogPalette distance={jog_distance} time={jog_duration} />
          </KeypadArea>
          <LightingArea>
            <LEDControlPalette />
          </LightingArea>
        </React.Fragment>
      )}
    </Composition>
  )
}

export default ManualJogPalette
