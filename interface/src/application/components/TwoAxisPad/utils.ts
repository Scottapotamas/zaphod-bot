import { Accessor, Draft } from '@electricui/components-core'
import { CommitStaging, GenerateStaging } from '@electricui/components-core'
import {
  MutableRefObject,
  useCallback,
  useEffect,
  useMemo,
  useRef,
  useState,
} from 'react'

import { CancellationToken } from '@electricui/async-utilities'

import throttle from 'lodash.throttle'

export type HTMLElementOrNull = HTMLElement | null
export type CallbackRef = (node: HTMLElementOrNull) => any
export type AnyRef = CallbackRef | MutableRefObject<HTMLElementOrNull>

export function shallowObjectEquality(
  a: Record<string, any>,
  b: Record<string, any>,
) {
  for (const key in a) {
    if (!(key in b) || a[key] !== b[key]) {
      return false
    }
  }
  for (const key in b) {
    if (!(key in a) || a[key] !== b[key]) {
      return false
    }
  }
  return true
}

function getBoundingClientRect(element: HTMLElement): DOMRect {
  return element.getBoundingClientRect()
}

function between(value: number, low: number, high: number) {
  return value >= low && value <= high
}

function globalXYToRelative(
  clientX: number,
  clientY: number,
  rect: ClientRect | DOMRect,
) {
  const relX = clientX - rect.left
  const relY = clientY - rect.top

  const clampedX = Math.max(Math.min(relX, rect.width), 0)
  const clampedY = Math.max(Math.min(relY, rect.height), 0)

  // Scaled 0-1
  const scaledX = clampedX / rect.width
  const scaledY = clampedY / rect.height

  return [clampedX, clampedY, scaledX, scaledY] as [
    number,
    number,
    number,
    number,
  ]
}

export function scaleNormalisedToPosition(
  input: number,
  min: number,
  max: number,
) {
  let output: number = input * (max - min) + min

  return output
}

export function scalePositionToNormalised(
  input: number,
  min: number,
  max: number,
) {
  let output: number = (input - min) / (max - min)

  return output
}

/**
 * Takes a callback and calls it when the mouse is clicked or dragged through the dom element specified by the ref
 * @param callback
 */
export function useClickDragWithin(
  callback: (
    relX: number,
    relY: number,
    scaledX: number,
    scaledY: number,
    mouseDown: boolean,
  ) => void,
  markerRef: React.MutableRefObject<HTMLDivElement | null>,
) {
  const [node, setNode] = useState<HTMLElementOrNull>(null)

  const ref = useCallback((refNode: HTMLElement | null) => {
    setNode(refNode)
  }, [])

  // Are we mousing at the moment?
  const isSelecting = useRef(false)

  // Setup mouse move handlers
  const onMouseDown = useCallback(
    (event: MouseEvent) => {
      // Validate our dom node is rendered
      if (!node) {
        return
      }

      // Only start the move if we're clicking on our explicit target,
      // don't let us click through a modal for example.
      if (node !== event.target && event.target !== markerRef.current) {
        return
      }

      const rect = getBoundingClientRect(node)

      // Validate we're within the box
      if (
        !between(event.clientX, rect.left, rect.left + rect.width) ||
        !between(event.clientY, rect.top, rect.top + rect.height)
      ) {
        return
      }

      // We are selecting
      isSelecting.current = true

      // Calculate our position
      const pos = globalXYToRelative(event.clientX, event.clientY, rect)

      // Send it to the callback
      callback(pos[0], pos[1], pos[2], pos[3], true)

      // Stop the event continuing up the tree
      event.stopPropagation()
    },
    [node],
  )
  const onMouseUp = useCallback(
    (event: MouseEvent) => {
      if (!isSelecting.current) {
        return
      }

      // Validate our dom node is rendered
      if (!node) {
        return
      }

      const rect = getBoundingClientRect(node)

      // Calculate our position
      const pos = globalXYToRelative(event.clientX, event.clientY, rect)

      // Send it to the callback
      callback(pos[0], pos[1], pos[2], pos[3], false)

      isSelecting.current = false

      // Stop the event continuing up the tree
      event.stopPropagation()
    },
    [node],
  )
  const onMouseMove = useCallback(
    (event: MouseEvent) => {
      if (!isSelecting.current) {
        return
      }

      // Validate our dom node is rendered
      if (!node) {
        return
      }

      const rect = getBoundingClientRect(node)

      // Calculate our position
      const pos = globalXYToRelative(event.clientX, event.clientY, rect)

      // Send it to the callback
      callback(pos[0], pos[1], pos[2], pos[3], true)

      // Stop the event continuing up the tree
      event.stopPropagation()
    },
    [node],
  )

  // Setup our global event listeners
  useEffect(() => {
    document.addEventListener('mousedown', onMouseDown)
    document.addEventListener('mouseup', onMouseUp)
    document.addEventListener('mousemove', onMouseMove)
    return () => {
      document.removeEventListener('mousedown', onMouseDown)
      document.removeEventListener('mouseup', onMouseUp)
      document.removeEventListener('mousemove', onMouseMove)
    }
  }, [onMouseDown, onMouseUp, onMouseMove])

  return ref
}

export function generateWriteErrHandler(
  asyncThrow: (error: Error) => void,
  cancellationToken: CancellationToken,
) {
  return (err: Error) => {
    if (cancellationToken.caused(err)) {
      return
    }

    asyncThrow(err)
  }
}

function convertArrayValuesToHashmap(xy: [number, number]) {
  return { horizontal: xy[0], vertical: xy[1] }
}

export interface Position {
  horizontal: number
  vertical: number
}

export function performWriteFactory(
  writer: (staging: Draft<ElectricUIDeveloperState>, value: Position) => void,
  asyncThrow: (err: Error) => void,
  generateStaging: GenerateStaging,
  commitStaged: CommitStaging,
  messageIDsNeedAcking: React.MutableRefObject<Set<string>>,
  lastUpdateID: React.MutableRefObject<number>,
  sendOnlyOnRelease: boolean | undefined,
  pushMessageIDs: (
    messageIDs: string[],
    ack: boolean,
    cancellationToken: CancellationToken,
  ) => Promise<any>,
  getDeadline: () => CancellationToken,
  lastPushedUpdateID: React.MutableRefObject<number>,
  throttleDuration: number | undefined,
) {
  const performWrite = useCallback(
    throttle(
      (handle: [number, number], release: boolean) => {
        const handleObject = convertArrayValuesToHashmap(handle)

        const staging = generateStaging() // Generate the staging
        writer(staging, handleObject) // The writer mutates the staging into a 'staged'

        const messageIDs = commitStaged(staging)
        // If the handle is held down for more than `props.throttleDuration`ms, then the committed state
        // will be modified twice with the same values, the second time, no changes will be detected
        // but the message would not have been acked.
        // remembering the messageIDs that changed, we can force an ack later.
        for (const messageID of messageIDs) {
          messageIDsNeedAcking.current.add(messageID)
        }

        // Increment the last push ID
        lastUpdateID.current++

        if (sendOnlyOnRelease && !release) {
          return
        }

        // Capture a copy of it
        const thisPushID = lastUpdateID.current

        // Create our cancellation token
        const cancellationToken = getDeadline()

        // Only ack on release
        const pushPromise = pushMessageIDs(
          Array.from(messageIDsNeedAcking.current),
          release,
          cancellationToken,
        ).catch(generateWriteErrHandler(asyncThrow, cancellationToken))

        // Clear the messageIDs to ack on release
        if (release) {
          messageIDsNeedAcking.current.clear()

          // We already caught the promise, don't catch it again
          // eslint-disable-next-line promise/catch-or-return
          pushPromise.finally(() => {
            // When it's received by hardware, update our last push finished ID asynchronously, hold the focussed state until then
            lastPushedUpdateID.current = thisPushID
          })
        }
      },
      // throttle options
      throttleDuration ?? 32,
      { leading: true, trailing: true },
    ),
    // callback deps
    [writer, throttleDuration, sendOnlyOnRelease],
  )

  return performWrite
}

export function writerFactoryWithDefault(
  writer:
    | ((staging: Draft<ElectricUIDeveloperState>, value: Position) => void)
    | undefined,
  x: Accessor,
  y: Accessor,
) {
  // the writer
  return useMemo(() => {
    if (!writer && typeof x === 'string' && typeof y === 'string') {
      return (staging: Draft<ElectricUIDeveloperState>, value: Position) => {
        staging[x] = value.horizontal
        staging[y] = value.vertical
      }
    }

    if (!writer) {
      throw new Error(
        "If the TwoAxisPads's accessors aren't MessageID strings, a writer must be provided",
      )
    }

    return writer
  }, [writer, x, y])
}
