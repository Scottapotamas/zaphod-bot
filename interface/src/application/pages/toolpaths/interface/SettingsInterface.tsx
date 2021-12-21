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

import { Updater } from 'use-immer'

import classnames from 'classnames'

import { FrameProgressUpdate, ToolpathGenerator } from '../optimiser/main'
import { importFolder } from '../optimiser/files'
import { blankMaterial } from '../optimiser/material'
import { Vector3 } from 'three'
import { FolderPathSelector } from './FolderSelector'
import { DataSource, Event, EventBatch } from '@electricui/timeseries'
import { timing } from '@electricui/timing'
import { Optimiser } from './Optimiser'
import { Settings } from '../optimiser/settings'

const defaultSettings = {
  objectSettings: {
    gpencil: {
      breakUpStrokes: true,
    },
    particles: {
      drawInVelocityOrientation: true,
      stopDelay: 1,
    },
  },

  // Do object level overrides here. Particle subsystems can be `object -> subsystem name`
  objectOverrides: {},

  objectToggles: {},

  transitionMaterial: blankMaterial,
  materialOverrides: {
    globalOveride: null,
    objectMaterialOverrides: {},
  },

  optimisation: {
    startingPoint: new Vector3(0, 0, 0),
    endingPoint: new Vector3(0, 0, 0),
    maxSpeed: 100,
    waitAtStartDuration: 1000,
  },
}

interface SettingsInterfaceProps {
  settings: Settings
  setSettings: Updater<Settings>
}

function ParticleWaitDurationSlider(props: SettingsInterfaceProps) {
  const [localStopDelay, setLocalStopDelay] = useState(
    props.settings.objectSettings.particles.stopDelay ?? 0,
  )

  const updateStopDelay = useCallback(
    delay => {
      props.setSettings(settings => {
        settings.objectSettings.particles.stopDelay = delay
      })
    },
    [props.setSettings],
  )

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

function MaxSpeedSlider(props: SettingsInterfaceProps) {
  const [localSpeed, setLocalSpeed] = useState(
    props.settings.optimisation.maxSpeed,
  )

  const updateMaxSpeed = useCallback(
    newMaxSpeed => {
      props.setSettings(settings => {
        settings.optimisation.maxSpeed = Math.max(newMaxSpeed, 1)
      })
    },
    [props.setSettings],
  )

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

function WaitAtStartDurationSlider(props: SettingsInterfaceProps) {
  const [waitDuration, setWaitDuration] = useState(
    props.settings.optimisation.waitAtStartDuration,
  )

  const updateWaitDuration = useCallback(
    waitDuration => {
      props.setSettings(settings => {
        settings.optimisation.waitAtStartDuration = waitDuration
      })
    },
    [props.setSettings],
  )

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

export const SettingsInterface = (props: SettingsInterfaceProps) => {
  return (
    <Card>
      <FormGroup label="Particle wait duration">
        <ParticleWaitDurationSlider
          settings={props.settings}
          setSettings={props.setSettings}
        />
      </FormGroup>
      <FormGroup label="Maximum speed">
        <MaxSpeedSlider
          settings={props.settings}
          setSettings={props.setSettings}
        />
      </FormGroup>
      <FormGroup label="Wait at start duration">
        <WaitAtStartDurationSlider
          settings={props.settings}
          setSettings={props.setSettings}
        />
      </FormGroup>
    </Card>
  )
}
