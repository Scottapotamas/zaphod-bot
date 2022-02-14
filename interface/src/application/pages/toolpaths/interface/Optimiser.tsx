import { Colors } from '@blueprintjs/core'

import React, { useCallback, useEffect, useRef, useState, useMemo } from 'react'

import {
  ChartContainer,
  BarChart,
  BarChartDomain,
  VerticalAxis,
  HorizontalAxis,
} from '@electricui/components-desktop-charts'

import { FrameProgressUpdate, ToolpathGenerator } from '../optimiser/main'
import { importFolder, renderablesToMovements } from '../optimiser/files'
import {
  DataSource,
  Event,
  EventBatch,
  PersistenceEnginePassthrough,
} from '@electricui/timeseries'
import { timing } from '@electricui/timing'
import { Settings } from '../optimiser/settings'

import {
  getCurrentSettings,
  resetStore,
  setSetting,
  getSetting,
  useSetting,
  useStore,
  incrementViewportFrameVersion,
} from './state'
import { Movement, SerialisedTour } from '../optimiser/movements'
import { Renderable } from '../optimiser/import'
import { renderablesToSceneTree } from './RenderableTree'

import os from 'os'

function recalculateMovementsPerFrame() {
  const settings = getSetting(state => state.settings)
  const renderablesByFrame = getSetting(state => state.renderablesByFrame)

  const unorderedMovementsByFrame: {
    [frameNumber: number]: Movement[]
  } = {}

  for (const fN of Object.keys(renderablesByFrame)) {
    const frameNumber = Number(fN)

    const renderables = renderablesByFrame[frameNumber]

    const movements = renderablesToMovements(renderables, settings)

    unorderedMovementsByFrame[frameNumber] = movements
  }

  setSetting(state => {
    state.unorderedMovementsByFrame = unorderedMovementsByFrame
  })
}

export function Optimiser() {
  // Establish a mutable reference to the setTotalFrames state setter so we can do it asyncronously
  const totalFrames = useStore(state => state.sceneTotalFrames)
  const selectedMinFrame = useStore(state => state.selectedMinFrame)
  const selectedMaxFrame = useStore(state => state.selectedMaxFrame)

  const persistentOptimiser = useRef<ToolpathGenerator | null>(null)

  /**
   * Lazily create and hold a persistent optimiser
   */
  function getPersistentOptimiser() {
    if (persistentOptimiser.current === null) {
      persistentOptimiser.current = new ToolpathGenerator(
        getCurrentSettings(),
        Math.max(1, os.cpus().length / 2 - 1), // assume hyperthreading
      )
    }

    return persistentOptimiser.current
  }

  // Setup a subscriber to grab new settings
  useEffect(() => {
    return useStore.subscribe(
      state => state.settings,
      settings => {
        getPersistentOptimiser().updateSettings(settings)

        // Movements must be recalculated on settings update
        recalculateMovementsPerFrame()
      },
    )
  }, [])

  // Setup a subscriber to grab frame limits
  useEffect(() => {
    return useStore.subscribe(
      state => state.selectedMinFrame,
      frameNumber => {
        getPersistentOptimiser().setFrameMinimum(frameNumber)
      },
    )
  }, [])
  useEffect(() => {
    return useStore.subscribe(
      state => state.selectedMaxFrame,
      frameNumber => {
        getPersistentOptimiser().setFrameMaximum(frameNumber)
      },
    )
  }, [])
  useEffect(() => {
    return useStore.subscribe(
      state => state.priorityFrame,
      frameNumber => {
        getPersistentOptimiser().setViewedFrame(frameNumber)
      },
    )
  }, [])

  // On unmount, clean up the optimiser
  useEffect(() => {
    // Update the reference to the currently used optimiser in the state
    setSetting(state => {
      state.persistentOptimiser = getPersistentOptimiser()
    })

    return () => {
      // Reset our state
      resetStore()

      if (persistentOptimiser.current === null) {
        return
      }

      console.log('tearing down optimiser')

      persistentOptimiser.current.teardown().then(() => {
        console.log('teardown complete')
      })

      persistentOptimiser.current = null
    }
  }, [])

  const frameTimeDataSource = useMemo(() => {
    const dataSource = new DataSource<{
      [frameNumber: string]: FrameProgressUpdate
    }>()
    dataSource.setPersistenceEngineFactory(
      () => new PersistenceEnginePassthrough(),
    )
    return dataSource
  }, [])

  const frameData = useRef<{
    [frameNumber: string]: FrameProgressUpdate
  }>({})

  const onProgress = useCallback(
    (progress: FrameProgressUpdate) => {
      setSetting(state => {
        state.movementOrdering[progress.frameNumber] = progress.serialisedTour
        state.estimatedDurationByFrame[progress.frameNumber] = progress.duration
        state.frameOptimisationState[progress.frameNumber] = progress.frameState

        // Trigger an update if this frame update is for the viewport frame

        if (state.viewportFrame === progress.frameNumber) {
          incrementViewportFrameVersion(state)
        }
      })

      frameData.current[progress.frameNumber] = progress

      // Publish a new frameData event
      const batch = new EventBatch()
      batch.push(new Event(timing.now(), frameData.current))
      frameTimeDataSource.write(batch)

      return
    },
    [frameTimeDataSource],
  )

  // The main injestion
  useEffect(() => {
    return useStore.subscribe(
      state => state.folder,
      folder => {
        if (folder === null) {
          return
        }

        importFolder(folder).then(imported => {
          // Reset the store when we import a new folder

          const sceneTotalFrames = Object.keys(
            imported.movementJSONByFrame,
          ).length
          const sceneTree = renderablesToSceneTree(imported.allRenderables)

          setSetting(state => {
            state.sceneMinFrame = imported.minFrame
            state.viewportFrame = imported.minFrame
            state.sceneMaxFrame = imported.maxFrame
            state.selectedMinFrame = imported.minFrame
            state.selectedMaxFrame = imported.maxFrame
            state.sceneTotalFrames = sceneTotalFrames
            state.currentlyOptimising = true
            state.allRenderables = imported.allRenderables
            state.renderablesByFrame = imported.renderablesByFrame

            state.treeStore.selectedItemID = null
            state.treeStore.tree = sceneTree
            state.arbitrary = imported.frameData

            console.log(`injested ${state.sceneTotalFrames} frames`)

            incrementViewportFrameVersion(state)
          })

          // Recalculate the movements for each frame
          recalculateMovementsPerFrame()

          const optimiser = getPersistentOptimiser()

          // Start optimising the frames
          optimiser.ingest(
            imported.movementJSONByFrame,
            getCurrentSettings(),
            onProgress,
          )
          // When it's done, mark it as complete
          optimiser.onComplete().then(() => {
            setSetting(state => {
              state.currentlyOptimising = false
            })
          })
        })
      },
    )
  }, [onProgress])

  const folder = useSetting(state => state.folder)

  if (folder === null) {
    return null
  }

  return (
    <>
      <ChartContainer>
        <BarChart
          dataSource={frameTimeDataSource}
          key={totalFrames}
          columns={totalFrames}
          accessor={(event: { [frameNumber: string]: FrameProgressUpdate }) => {
            const arr: number[] = new Array(totalFrames)

            for (let index = 0; index < totalFrames; index++) {
              arr[index] = event[index]?.duration ?? 0
            }

            return arr
          }}
          colorAccessor={(event: {
            [frameNumber: string]: FrameProgressUpdate
          }) => {
            const arr: string[] = new Array(totalFrames)

            for (let index = 0; index < totalFrames; index++) {
              switch (event[index]?.frameState) {
                case 0: // OPTIMISING_PARTIALLY
                  arr[index] = Colors.BLUE3
                  break
                case 1: // OPTIMISING_FULLY
                  arr[index] = Colors.GREEN3
                  break
                case 2: // UNOPTIMISED
                  arr[index] = Colors.BLACK
                  break
                case 3: // OPTIMISED_PARTIALLY
                  arr[index] = Colors.BLUE5
                  break
                case 4: // OPTIMISED_FULLY
                  arr[index] = Colors.GREEN5
                  break
                case 5: // ERRORED
                  arr[index] = Colors.RED3
                  break

                default:
                  arr[index] = Colors.BLACK
                  break
              }
            }

            return arr
          }}
        />
        <BarChartDomain xMin={selectedMinFrame} xMax={selectedMaxFrame} />

        <VerticalAxis
          label="Render time"
          tickFormat={tick => `${Math.round((tick / 1000) * 100) / 100}s`}
          labelPadding={50}
        />
        <HorizontalAxis label="Frame number" />
      </ChartContainer>
    </>
  )
}
