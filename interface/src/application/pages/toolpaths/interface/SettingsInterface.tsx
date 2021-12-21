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
    <Slider
      min={0}
      max={300}
      stepSize={1}
      labelStepSize={50}
      value={localSpeed}
      labelRenderer={value => `${value}mm/s`}
      onRelease={updateMaxSpeed}
      onChange={setLocalSpeed}
    />
  )
}

export const SettingsInterface = (props: SettingsInterfaceProps) => {
  return (
    <Card>
      <FormGroup label="Maximum speed">
        <MaxSpeedSlider
          settings={props.settings}
          setSettings={props.setSettings}
        />
      </FormGroup>
    </Card>
  )
}
