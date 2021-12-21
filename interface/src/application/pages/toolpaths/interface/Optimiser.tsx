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
import { importFolder } from '../optimiser/files'
import { blankMaterial } from '../optimiser/material'
import { Vector3 } from 'three'
import {
  DataSource,
  Event,
  EventBatch,
  PersistenceEnginePassthrough,
} from '@electricui/timeseries'
import { timing } from '@electricui/timing'
import { Settings } from '../optimiser/settings'

import { getCurrentSettings, setSetting, useSetting, useStore } from './state'

export function Optimiser() {
  // Establish a mutable reference to the setTotalFrames state setter so we can do it asyncronously
  const [totalFrames, setTotalFrames] = useState(0)
  const currentSetTotalFrames = useRef<
    React.Dispatch<React.SetStateAction<number>>
  >(setTotalFrames)
  useEffect(() => {
    currentSetTotalFrames.current = setTotalFrames
  }, [setTotalFrames])

  const persistentOptimiser = useRef<ToolpathGenerator | null>(null)

  /**
   * Lazily create and hold a persistent optimiser
   */
  function getPersistentOptimiser() {
    if (persistentOptimiser.current === null) {
      persistentOptimiser.current = new ToolpathGenerator(
        getCurrentSettings(),
        4,
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
      },
    )
  }, [])

  // On unmount, clean up the optimiser
  useEffect(() => {
    return () => {
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
      // setFrameStates(prev => ({
      //   ...prev,
      //   [progress.frameNumber]: progress.frameState,
      // }))

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
          currentSetTotalFrames.current(
            total => Object.keys(imported.movementJSONByFrame).length,
          )

          const optimiser = getPersistentOptimiser()

          optimiser.ingest(
            imported.movementJSONByFrame,
            getCurrentSettings(),
            onProgress,
          )
        })
      },
    )
  }, [onProgress])

  const folder = useSetting(state => state.folder)

  if (folder === null) {
    return <>Please select a folder </>
  }

  return (
    <>
      {folder}
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
        <BarChartDomain />

        <VerticalAxis
          label="Render time"
          tickFormat={tick => `${Math.round((tick / 1000) * 100) / 100}s`}
          labelPadding={50}
        />
        <HorizontalAxis label="Frame number" />
      </ChartContainer>
      {/* <div
          style={{
            display: 'grid',
            gridGap: 10,
            gridTemplateColumns: 'repeat(10, 1fr)',
          }}
        >
          {Object.keys(frameTimings).map(frameNumber => (
            <Tag
              key={frameNumber}
              intent={stateLookup.get(frameStates[frameNumber])}
            >
              <div style={{ display: 'inline-flex', width: '100%' }}>
                <span style={{ width: '100%', alignSelf: 'start' }}>
                  #{frameNumber}
                </span>
                <span style={{ width: '100%', alignSelf: 'end' }}>
                  {Math.round(frameTimings[frameNumber])}ms
                </span>
              </div>
            </Tag>
          ))}
        </div> */}
      <br />
      {/* Total duration:{' '}
        {Math.round(
          Object.values(frameTimings).reduce((acc, cur) => acc + cur, 0),
        ) / 1000}{' '}
        seconds */}
    </>
  )
}
