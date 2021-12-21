import {
  Button,
  Card,
  Classes,
  Colors,
  FormGroup,
  Intent,
  Label,
  Tag,
} from '@blueprintjs/core'
import { RouteComponentProps } from '@reach/router'

import React, { useCallback, useEffect, useRef, useState } from 'react'
import { navigate, OPEN_DIALOG_IPC_EVENT } from '@electricui/utility-electron'
import { ipcRenderer, OpenDialogOptions } from 'electron'
import { useImmer } from 'use-immer'

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
import { blankMaterial } from '../optimiser/material'
import { Vector3 } from 'three'
import { FolderPathSelector } from './FolderSelector'
import { DataSource, Event, EventBatch } from '@electricui/timeseries'
import { timing } from '@electricui/timing'
import { Optimiser } from './Optimiser'
import { Settings } from '../optimiser/settings'

import { SettingsInterface } from './SettingsInterface'

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

export const ToolpathViewer = (props: RouteComponentProps) => {
  const [folder, setFolder] = useState<string | null>(null)
  const [settings, setSettings] = useImmer<Settings>(defaultSettings)

  return (
    <React.Fragment>
      <Button onClick={() => navigate(`/`)} style={{ margin: 20 }}>
        Back to connections screen
      </Button>
      <hr />
      <div style={{ maxWidth: 600 }}>
        <FolderPathSelector onFoundFolder={setFolder} />

        <SettingsInterface settings={settings} setSettings={setSettings} />

        <Optimiser folder={folder} settings={settings} />
      </div>
    </React.Fragment>
  )
}
