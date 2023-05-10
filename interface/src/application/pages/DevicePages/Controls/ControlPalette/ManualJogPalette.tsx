import React, { useState } from 'react'

import { Icon, MultiSlider, NumericInput, Tag, Intent } from '@blueprintjs/core'
import { Composition, Only } from 'atomic-layout'

import { Button, NumberInput } from '@electricui/components-desktop-blueprint'
import {
  MovementMoveReference,
  MovementMoveType,
  MSGID,
} from 'src/application/typedState'

const JogButtonAreas = `
XP YP ZP
YM XM ZM
`

interface JogPaletteProps {
  distance: number
  time: number
}

const JogPalette = (props: JogPaletteProps) => {
  return (
    <>
      {/* 
        X axis is red, Y axis is green, z axis is blue
        This matches the Blender color associations 
      */}
      <Composition areas={JogButtonAreas} gap={5} templateCols="auto auto 1fr">
        {({ XP, YP, ZP, XM, YM, ZM }) => (
          <React.Fragment>
            <XP>
              <Button
                large
                outlined
                intent={Intent.DANGER}
                writer={state => {
                  state[MSGID.QUEUE_ADD_MOVE] = {
                    id: 0,
                    type: MovementMoveType.POINT_TRANSIT,
                    reference: MovementMoveReference.RELATIVE,
                    sync_offset: 0,
                    duration: props.time,
                    points: [[props.distance, 0, 0]],
                  }
                }}
              >
                <Icon icon="arrow-top-left" iconSize={50} />
              </Button>
            </XP>
            <YP>
              <Button
                large
                outlined
                intent={Intent.SUCCESS}
                writer={state => {
                  state[MSGID.QUEUE_ADD_MOVE] = {
                    id: 0,
                    type: MovementMoveType.POINT_TRANSIT,
                    reference: MovementMoveReference.RELATIVE,
                    sync_offset: 0,
                    duration: props.time,
                    points: [[0, props.distance, 0]],
                  }
                }}
              >
                <Icon icon="arrow-top-right" iconSize={50} />
              </Button>
            </YP>
            <XM>
              <Button
                large
                outlined
                intent={Intent.DANGER}
                writer={state => {
                  state[MSGID.QUEUE_ADD_MOVE] = {
                    id: 0,
                    type: MovementMoveType.POINT_TRANSIT,
                    reference: MovementMoveReference.RELATIVE,
                    sync_offset: 0,
                    duration: props.time,
                    points: [[-props.distance, 0, 0]],
                  }
                }}
              >
                <Icon icon="arrow-bottom-right" iconSize={50} />
              </Button>
            </XM>
            <YM>
              <Button
                large
                outlined
                intent={Intent.SUCCESS}
                writer={state => {
                  state[MSGID.QUEUE_ADD_MOVE] = {
                    id: 0,
                    type: MovementMoveType.POINT_TRANSIT,
                    reference: MovementMoveReference.RELATIVE,
                    sync_offset: 0,
                    duration: props.time,
                    points: [[0, -props.distance, 0]],
                  }
                }}
              >
                <Icon icon="arrow-bottom-left" iconSize={50} />
              </Button>
            </YM>
            <ZP>
              <Button
                large
                fill
                outlined
                intent={Intent.PRIMARY}
                writer={state => {
                  state[MSGID.QUEUE_ADD_MOVE] = {
                    id: 0,
                    type: MovementMoveType.POINT_TRANSIT,
                    reference: MovementMoveReference.RELATIVE,
                    sync_offset: 0,
                    duration: props.time,
                    points: [[0, 0, props.distance]],
                  }
                }}
              >
                <Icon icon="arrow-up" iconSize={50} />
              </Button>
            </ZP>
            <ZM>
              <Button
                large
                fill
                outlined
                intent={Intent.PRIMARY}
                writer={state => {
                  state[MSGID.QUEUE_ADD_MOVE] = {
                    id: 0,
                    type: MovementMoveType.POINT_TRANSIT,
                    reference: MovementMoveReference.RELATIVE,
                    sync_offset: 0,
                    duration: props.time,
                    points: [[0, 0, -props.distance]],
                  }
                }}
              >
                <Icon icon="arrow-down" iconSize={50} />
              </Button>
            </ZM>
          </React.Fragment>
        )}
      </Composition>
    </>
  )
}

const ManualJogPalette = () => {
  let [jog_speed, setJogSpeed] = useState(50) // mm/second
  let [jog_distance, setJogDistance] = useState(10.0) // mm

  let jog_duration = (jog_distance * 1000) / jog_speed // milliseconds

  return (
    <Composition gap={20} padding={10} alignItems="center">
      <Composition templateCols="200px 200px" gap={20}>
        <NumericInput
          value={jog_speed}
          onValueChange={value => setJogSpeed(value)}
          fill
          intent={Intent.WARNING}
          min={0}
          max={300}
          majorStepSize={25}
          rightElement={
            <Tag minimal intent={Intent.WARNING}>
              SPEED
            </Tag>
          }
        />
        <NumericInput
          value={jog_distance}
          onValueChange={value => setJogDistance(value)}
          fill
          intent={Intent.SUCCESS}
          min={0}
          max={25}
          majorStepSize={5}
          rightElement={
            <Tag minimal intent={Intent.SUCCESS}>
              JOG DISTANCE
            </Tag>
          }
        />
      </Composition>

      <JogPalette distance={jog_distance} time={jog_duration} />
    </Composition>
  )
}

export default ManualJogPalette
