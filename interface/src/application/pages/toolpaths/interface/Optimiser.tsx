import { Colors } from '@blueprintjs/core'

import React, { useCallback, useEffect, useRef, useMemo, useState } from 'react'

import {
  ChartContainer,
  BarChart,
  BarChartDomain,
  VerticalAxis,
  HorizontalAxis,
  HorizontalLineAnnotation,
} from '@electricui/components-desktop-charts'

import { FRAME_STATE, FrameProgressUpdate, ToolpathGenerator } from '../optimiser/main'
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

  // Lazily create a single instance of the frameTimeDataSource
  const [frameTimeDataSource] = useState(() => {
    const dataSource = new DataSource<{
      [frameNumber: string]: MinimalFrameDurationUpdate
    }>()

    // Don't keep the data around
    dataSource.setPersistenceEngineFactory(() => new PersistenceEnginePassthrough())

    return dataSource
  })


  const [persistentOptimiser] = useState(() => {

    // Re-use the same frameData object
    const frameData: {
      [frameNumber: string]: MinimalFrameDurationUpdate
    } = {}

    // Create the onFrameStateChange callback
    const onFrameStateChange =  (frameNumber: number, duration: number, frameState: FRAME_STATE) => {
      changeState(state => {
        state.estimatedDurationByFrame[frameNumber] = duration
        state.frameOptimisationState[frameNumber] = frameState
      })

      // Pull this data out to avoid copying anything expensive
      frameData[frameNumber] = {
        duration: duration,
        frameState: frameState
      }

      // Publish a new frameData event with the same object
      const batch = new EventBatch()
      batch.push(timing.now(), frameData)
      frameTimeDataSource.write(batch)

      return
    }

    // and the progress handler
    const onProgress = (progress: FrameProgressUpdate)  => {
      changeState(state => {
        singleton.onProgressUpdate(progress.frameNumber, progress.serialisedTour)

        // Progress updates also trigger frame state updates
        onFrameStateChange(progress.frameNumber, progress.duration, progress.frameState)

        if (state.viewportFrame === progress.frameNumber) {
          incrementViewportFrameVersion(state)
        }
      })
    }

    return new ToolpathGenerator(
      getCurrentSettings(), 
      Math.max(1, Math.round(os.cpus().length / 2) - 1),
      onProgress,
      onFrameStateChange
    )
  })


  const pendingMovementRecalculationCancellationToken = useRef<CancellationToken | null>(null)

  // Setup a subscriber to grab new settings
  useEffect(() => {
    return useStore.subscribe(
      state => state.settings,
      settings => {
        persistentOptimiser.updateSettings(settings)
      },
    )
  }, [])

  // Setup a subscriber to grab frame limits
  useEffect(() => {
    return useStore.subscribe(
      state => state.selectedMinFrame,
      frameNumber => {
        persistentOptimiser.setFrameMinimum(frameNumber)
      },
    )
  }, [])
  useEffect(() => {
    return useStore.subscribe(
      state => state.selectedMaxFrame,
      frameNumber => {
        persistentOptimiser.setFrameMaximum(frameNumber)
      },
    )
  }, [])
  useEffect(() => {
    return useStore.subscribe(
      state => state.priorityFrame,
      frameNumber => {
        persistentOptimiser.setViewedFrame(frameNumber)
      },
    )
  }, [])

  // On unmount, clean up the optimiser
  useEffect(() => {
    // Update the reference to the currently used optimiser in the state
    changeState(state => {
      state.persistentOptimiser = persistentOptimiser
    })

    return () => {
      // Reset our state
      resetStore()

      console.log('tearing down optimiser')

      persistentOptimiser.teardown().then(() => {
        console.log('teardown complete')
      })
    }
  }, [])

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

          const optimiser = persistentOptimiser

          // Start optimising the frames
          optimiser.ingest(imported.movementJSONByFrame, getCurrentSettings())
        })
      },
    )
  }, [])

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
          accessor={(event: { [frameNumber: string]: MinimalFrameDurationUpdate }) => {
            const arr: number[] = []

            for (let index = 0; index < totalFrames; index++) {
              // If the number is non-finite, just pass 0
              const dur = event[index]?.duration ?? 0
              arr[index] = Number.isFinite(event[index]?.duration) ? dur : 0
            }

            return arr
          }}
          colorAccessor={(event: { [frameNumber: string]: MinimalFrameDurationUpdate }) => {
            const arr: string[] = []

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
          tickFormat={tick => msTickFormat(tick)}
          labelPadding={55}
        />

        <HorizontalAxis labelPadding={10}/>

        {cameraOverrideDuration > 0 ? (
          <HorizontalLineAnnotation y={cameraOverrideDuration} color={Colors.RED5} lineWidth={2} affectBounds />
        ) : null}
      </ChartContainer>
    </>
  )
}

function msTickFormat(milliseconds: number) {
  const seconds = Math.floor(milliseconds / 1000);
  const minutes = Math.floor(seconds / 60);
  const remainingSeconds = seconds % 60;

  if (minutes === 0) {
    return `${seconds}s`;
  } else {
    return `${minutes}m${remainingSeconds.toString().padStart(2, '0')}s`;
  }
}

type MinimalFrameDurationUpdate = {
  duration: number
  frameState: FRAME_STATE
}
