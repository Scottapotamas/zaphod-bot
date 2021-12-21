import {
  Button,
  Card,
  Classes,
  Colors,
  FormGroup,
  Intent,
  Label,
  Slider,
  Tag,
} from '@blueprintjs/core'
import { RouteComponentProps } from '@reach/router'

import React, { useCallback, useEffect, useRef, useState } from 'react'
import { navigate, OPEN_DIALOG_IPC_EVENT } from '@electricui/utility-electron'
import { ipcRenderer, OpenDialogOptions } from 'electron'

import {
  ChartContainer,
  BarChart,
  BarChartDomain,
  VerticalAxis,
  HorizontalAxis,
} from '@electricui/components-desktop-charts'

import classnames from 'classnames'

import { FrameProgressUpdate, ToolpathGenerator } from '../optimiser/main'
import { importFolder } from '../optimiser/files'
import { Vector3 } from 'three'
import { FolderPathSelector } from './FolderSelector'
import { DataSource, Event, EventBatch } from '@electricui/timeseries'
import { timing } from '@electricui/timing'
import { Optimiser } from './Optimiser'
import { Settings } from '../optimiser/settings'
import { getSetting, setSetting } from './state'

function ParticleWaitDurationSlider() {
  const [localStopDelay, setLocalStopDelay] = useState(
    getSetting(state => state.settings.objectSettings.particles.stopDelay) ?? 0,
  )

  const updateStopDelay = useCallback(delay => {
    setSetting(state => {
      state.settings.objectSettings.particles.stopDelay = delay
    })
  }, [])

  return (
    <div style={{ marginLeft: 10, marginRight: 10 }}>
      <Slider
        min={0}
        max={200}
        stepSize={1}
        labelStepSize={50}
        value={localStopDelay}
        labelRenderer={value => `${value}ms`}
        onRelease={updateStopDelay}
        onChange={setLocalStopDelay}
      />
    </div>
  )
}

function MaxSpeedSlider() {
  const [localSpeed, setLocalSpeed] = useState(
    getSetting(state => state.settings.optimisation.maxSpeed),
  )

  const updateMaxSpeed = useCallback(newMaxSpeed => {
    setSetting(state => {
      state.settings.optimisation.maxSpeed = Math.max(newMaxSpeed, 1)
    })
  }, [])

  return (
    <div style={{ marginLeft: 10, marginRight: 10 }}>
      <Slider
        min={0}
        max={300}
        stepSize={1}
        labelStepSize={50}
        value={Math.max(localSpeed, 1)}
        labelRenderer={value => `${value}mm/s`}
        onRelease={updateMaxSpeed}
        onChange={setLocalSpeed}
      />
    </div>
  )
}

function WaitAtStartDurationSlider() {
  const [waitDuration, setWaitDuration] = useState(
    getSetting(state => state.settings.optimisation.waitAtStartDuration),
  )

  const updateWaitDuration = useCallback(waitDuration => {
    setSetting(state => {
      state.settings.optimisation.waitAtStartDuration = waitDuration
    })
  }, [])

  return (
    <div style={{ marginLeft: 10, marginRight: 10 }}>
      <Slider
        min={0}
        max={5000}
        stepSize={100}
        labelStepSize={500}
        value={waitDuration}
        labelRenderer={value => `${Math.round((value / 1000) * 10) / 10}s`}
        onRelease={updateWaitDuration}
        onChange={setWaitDuration}
      />
    </div>
  )
}

export const SettingsInterface = () => {
  return (
    <Card>
      <FormGroup label="Particle wait duration">
        <ParticleWaitDurationSlider />
      </FormGroup>
      <FormGroup label="Maximum speed">
        <MaxSpeedSlider />
      </FormGroup>
      <FormGroup label="Wait at start duration">
        <WaitAtStartDurationSlider />
      </FormGroup>
    </Card>
  )
}
