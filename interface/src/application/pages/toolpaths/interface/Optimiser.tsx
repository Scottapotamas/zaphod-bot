import { Colors } from '@blueprintjs/core'

import React, { useCallback, useEffect, useRef, useMemo } from 'react'

import {
  ChartContainer,
  BarChart,
  BarChartDomain,
  VerticalAxis,
  HorizontalAxis,
  HorizontalLineAnnotation,
} from '@electricui/components-desktop-charts'

import { FrameProgressUpdate, ToolpathGenerator } from '../optimiser/main'
import { importFolder } from '../optimiser/files'
import { DataSource, EventBatch, PersistenceEnginePassthrough } from '@electricui/timeseries'
import { timing } from '@electricui/timing'

import {
  getCurrentSettings,
  resetStore,
  setSetting,
  useSetting,
  useStore,
  incrementViewportFrameVersion,
  markClean,
  changeState,
  singleton,
  getSetting,
} from './state'
import { renderablesToSceneTree } from './RenderableTree'

import os from 'os'

import deepmerge from 'deepmerge'
import { CancellationToken } from '@electricui/async-utilities'

const overwriteMerge = (destinationArray: any[], sourceArray: any[]) => sourceArray

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
      persistentOptimiser.current = new ToolpathGenerator(getCurrentSettings(), Math.max(1, Math.round(os.cpus().length / 2) - 1))
    }

    return persistentOptimiser.current
  }

  const pendingMovementRecalculationCancellationToken = useRef<CancellationToken | null>(null)

  // Setup a subscriber to grab new settings
  useEffect(() => {
    return useStore.subscribe(
      state => state.settings,
      settings => {
        getPersistentOptimiser().updateSettings(settings)
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
    changeState(state => {
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
    dataSource.setPersistenceEngineFactory(() => new PersistenceEnginePassthrough())
    return dataSource
  }, [])

  const frameData = useRef<{
    [frameNumber: string]: FrameProgressUpdate
  }>({})

  const onProgress = useCallback(
    (progress: FrameProgressUpdate) => {
      changeState(state => {
        singleton.onProgressUpdate(progress.frameNumber, progress.serialisedTour)
        state.estimatedDurationByFrame[progress.frameNumber] = progress.duration
        state.frameOptimisationState[progress.frameNumber] = progress.frameState

        // console.log(`progress update for frame ${progress.frameNumber}, state now ${progress.frameState}, duration: ${progress.duration}`)

        // Trigger an update if this frame update is for the viewport frame

        if (state.viewportFrame === progress.frameNumber) {
          incrementViewportFrameVersion(state)
        }
      })

      frameData.current[progress.frameNumber] = progress

      // Publish a new frameData event
      const batch = new EventBatch()
      batch.push(timing.now(), frameData.current)
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

        importFolder(folder).then(async imported => {
          // Reset the store when we import a new folder

          const sceneTotalFrames = imported.sceneTotalFrames
          const sceneTree = renderablesToSceneTree(imported.allRenderables)

          setSetting(state => {
            state.sceneMinFrame = imported.minFrame
            state.viewportFrame = imported.minFrame
            state.sceneMaxFrame = imported.maxFrame
            state.selectedMinFrame = imported.minFrame
            state.selectedMaxFrame = imported.maxFrame
            state.sceneTotalFrames = sceneTotalFrames

            state.treeStore.selectedItemID = null
            state.treeStore.tree = sceneTree
            state.arbitrary = imported.frameData

            console.log(`ingesting ${state.sceneTotalFrames} frames`)

            // Merge in the state from the settings file
            state.settings = deepmerge(state.settings, imported.settingsToMerge, { arrayMerge: overwriteMerge })
            state.visualisationSettings = deepmerge(
              state.visualisationSettings,
              imported.visualisationSettingsToMerge,
              { arrayMerge: overwriteMerge },
            )
          })

          // Mark all settings as clean
          markClean()

          singleton.onIngest(imported.renderablesByFrame, imported.minFrame)

          const optimiser = getPersistentOptimiser()

          // Start optimising the frames
          optimiser.ingest(imported.movementJSONByFrame, getCurrentSettings(), onProgress)
        })
      },
    )
  }, [onProgress])

  const folder = useSetting(state => state.folder)

  // Grab the camera override duration
  const cameraOverrideDuration = useSetting(state => state.cameraOverrideDuration)

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
          colorAccessor={(event: { [frameNumber: string]: FrameProgressUpdate }) => {
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
          // label="Render time"
          tickFormat={tick => `${Math.round((tick / 1000) * 100) / 100}s`}
          labelPadding={20}
        />

        <HorizontalAxis labelPadding={10} />

        {cameraOverrideDuration > 0 ? (
          <HorizontalLineAnnotation y={cameraOverrideDuration} color={Colors.RED5} lineWidth={2} affectBounds />
        ) : null}
      </ChartContainer>
    </>
  )
}
