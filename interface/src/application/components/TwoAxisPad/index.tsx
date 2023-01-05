// Rough mutated version of the ColorPalette component

import React, { useCallback, useMemo, useRef } from 'react'

import { useDarkMode } from '@electricui/components-desktop'
import { Colors } from '@blueprintjs/core'

import {
  Accessor,
  Draft,
  useAsyncThrow,
  useCommitStateStaged,
  useDeadline,
  useDeviceID,
  useContainedStateSubscription,
  usePushMessageIDs,
} from '@electricui/components-core'

import {
  Position,
  performWriteFactory,
  useClickDragWithin,
  writerFactoryWithDefault,
  scaleNormalisedToPosition,
  scalePositionToNormalised,
} from './utils'

import { a, useSpring } from '@react-spring/web'

interface TwoAxisPadProps {
  /**
   * An Accessor for the x axis of the pad.
   */
  x: Accessor

  /**
   * An Accessor for the y axis of the pad.
   */
  y: Accessor

  /**
   * If the accessor is merely a messageID, this Writer is optional.
   * If the accessor is functional, then this writer must be used to mutate the StateTree for writing to the device.
   */
  writer?: (staging: Draft<ElectricUIDeveloperState>, value: Position) => void

  /**
   * Throttle hardware updates to once every n milliseconds, by default 100ms.
   */
  throttleDuration?: number

  /**
   * If this is true, intermediate values while dragging will be added to the UI StateTree but not sent to the device. When the handle is released, the updated state will be written to the device.
   */
  sendOnlyOnRelease?: boolean

  /**
   * Overwrite the width of the component.
   *
   * @default: 350
   */
  width?: number
  /**
   * Overwrite the height of the component.
   *
   * @default: 128
   */
  height?: number
  /**
   * Overwrite the marker size of the component.
   *
   * @default: 18
   */
  handleSize?: number

  padCenterColor: string
  padEdgeColor: string
  handleColor: string

  xMin: number
  xMax: number
  yMin: number
  yMax: number
}

// Pad Helpers
function getPadBackgroundStyle(centerColor: string, edgeColor: string) {
  return `radial-gradient(circle, ${centerColor} 20%, ${edgeColor} 90%)`
}

function relPosToTranslate(relX: number, relY: number, size: number) {
  return `translate3d(${relX - size / 2}px, ${relY - size / 2}px, 0)`
}

// Handle Helpers
function getHandleBackgroundColor(handleColor: string) {
  const css = `${handleColor}`

  return css
}

/**
 * TwoAxisPad
 * @name TwoAxisPad
 * @props TwoAxisPadProps
 */
export function TwoAxisPad(props: TwoAxisPadProps) {
  const deviceID = useDeviceID()

  if (!deviceID) {
    throw new Error('Cannot use a TwoAxisPad outside of a device context')
  }

  const paletteWidth = props.width ?? 350
  const paletteHeight = props.height ?? 128
  const handleSize = props.handleSize ?? 18

  const lastUpdateID = useRef(0)
  const lastPushedUpdateID = useRef(0)
  const isFocused = useRef(false)
  const messageIDsNeedAcking = useRef<Set<string>>(new Set())
  const [generateStaging, commitStaged] = useCommitStateStaged()
  const pushMessageIDs = usePushMessageIDs()
  const getDeadline = useDeadline()

  // Setup our subscription
  const startingPosition = useContainedStateSubscription(
    // Build a custom accessor that grabs everything at the same time.
    state => {
      return {
        x: (typeof props.x === 'string' ? state[props.x] : props.x(state)) ?? 0, // prettier-ignore
        y: (typeof props.y === 'string' ? state[props.y] : props.y(state)) ?? 0, // prettier-ignore
      }
    },
    // Our callback
    cartesianPosition => {
      // if we're focused, don't do anything with this update.
      if (isFocused.current) {
        return
      }

      // We're sure our state exists given that it's a generated combination of things, but this isn't always the case so we
      // must appease the type checker.
      if (cartesianPosition === null) {
        return
      }

      const [x, y] = [
        scalePositionToNormalised(cartesianPosition.x, props.xMin, props.xMax),
        scalePositionToNormalised(cartesianPosition.y, props.yMin, props.yMax),
      ]

      // If everything is the same, don't update.
      if (currentPosition.current.x === x && currentPosition.current.y === y) {
        return
      }

      // do update it?

      // Palette
      animatePalette({
        immediate: true,
        to: {
          background: getPadBackgroundStyle(
            props.padCenterColor,
            props.padEdgeColor,
          ),
        },
      })

      animatePaletteMarker({
        immediate: true,
        to: {
          transform: relPosToTranslate(
            paletteWidth * currentPosition.current.x,
            paletteHeight * (1 - currentPosition.current.y),
            handleSize,
          ),
          background: getHandleBackgroundColor(props.handleColor),
        },
      })
    },
  )

  // Calculate our starting position
  const [startingX, startingY] = startingPosition
    ? [
        scalePositionToNormalised(startingPosition.x, props.xMin, props.xMax),
        scalePositionToNormalised(startingPosition.y, props.yMin, props.yMax),
      ]
    : [0, 0]

  const currentPosition = useRef({
    x: startingX,
    y: startingY,
  })

  // Setup our spring for our palette, calculate our starting style
  const [colorGradientStyle, animatePalette] = useSpring(
    {
      immediate: true,
      background: getPadBackgroundStyle(
        props.padCenterColor,
        props.padEdgeColor,
      ),
      //   aspectRatio: `${props.width}/${props.height}`,
      width: paletteWidth,
      height: paletteHeight,
      display: 'inline-block',
      cursor: 'grab',
      borderStyle: 'solid',
      borderWidth: '2px',
      borderRadius: '6px',
      borderColor: `${props.handleColor}`,
    },
    [],
  )

  // We'll write to state on movement of the palette
  const asyncThrow = useAsyncThrow()

  // the writer
  const writer = writerFactoryWithDefault(props.writer, props.x, props.y)

  const performWrite = performWriteFactory(
    writer,
    asyncThrow,
    generateStaging,
    commitStaged,
    messageIDsNeedAcking,
    lastUpdateID,
    props.sendOnlyOnRelease,
    pushMessageIDs,
    getDeadline,
    lastPushedUpdateID,
    props.throttleDuration,
  )

  // Setup our spring for our palette, calculate our starting style
  const [markerStyle, animatePaletteMarker] = useSpring(
    {
      immediate: true,
      width: handleSize,
      height: handleSize,
      transform: relPosToTranslate(
        paletteWidth * startingX,
        paletteHeight * (1 - startingY),
        handleSize,
      ),
      background: getHandleBackgroundColor(props.handleColor),
      borderRadius: 9,
      border: '2px solid #fff',
      boxShadow: 'rgba(0, 0, 0, 0.5) 1px 1px 4px',
    },
    [],
  )

  const clickDragCallback = useCallback(
    (
      relX: number,
      relY: number,
      scaledX: number,
      scaledY: number,
      mouseDown: boolean,
    ) => {
      // Calculate our colour
      currentPosition.current.x = scaledX
      currentPosition.current.y = 1 - scaledY

      animatePaletteMarker({
        immediate: true,
        to: {
          transform: relPosToTranslate(relX, relY, handleSize),
          background: getHandleBackgroundColor(props.handleColor),
        },
      })

      // Convert into user's space
      const position = [
        scaleNormalisedToPosition(
          currentPosition.current.x,
          props.xMin,
          props.xMax,
        ),
        scaleNormalisedToPosition(
          currentPosition.current.y,
          props.yMin,
          props.yMax,
        ),
      ]

      // Write to hardware!
      performWrite(position, !mouseDown)

      isFocused.current = mouseDown
    },
    [currentPosition, animatePaletteMarker, isFocused],
  )

  // Setup our mouse movement callback
  const markerRef = useRef<HTMLDivElement | null>(null)
  const padRef = useClickDragWithin(clickDragCallback, markerRef)

  return (
    <React.Fragment>
      <a.div style={colorGradientStyle as any} ref={padRef}>
        <a.div style={markerStyle as any} ref={markerRef}></a.div>
      </a.div>
    </React.Fragment>
  )
}
