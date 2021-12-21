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
import { RenderInterface } from './RenderInterface'
import { ToolpathVisualisation } from './ToolpathVisualisation'

export const ToolpathViewer = (props: RouteComponentProps) => {
  return (
    <React.Fragment>
      <div
        style={{
          width: '100vw',
          height: '100vh',
          position: 'absolute',
        }}
      >
        <ToolpathVisualisation />
      </div>
      <div style={{ width: 600, position: 'absolute' }}>
        <Button onClick={() => navigate(`/`)} style={{ margin: 10 }}>
          Back to connections screen
        </Button>

        <FolderPathSelector />

        <SettingsInterface />

        <Optimiser />

        <RenderInterface />
      </div>
    </React.Fragment>
  )
}
