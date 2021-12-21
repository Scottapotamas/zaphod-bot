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

export const ToolpathViewer = (props: RouteComponentProps) => {
  return (
    <React.Fragment>
      <Button onClick={() => navigate(`/`)} style={{ margin: 20 }}>
        Back to connections screen
      </Button>
      <hr />
      <div style={{ maxWidth: 600 }}>
        <FolderPathSelector />

        <SettingsInterface />

        <Optimiser />
      </div>
    </React.Fragment>
  )
}
