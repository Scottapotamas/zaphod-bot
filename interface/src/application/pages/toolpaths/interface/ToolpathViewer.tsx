import { Button } from '@blueprintjs/core'
import { RouteComponentProps } from '@reach/router'

import React, { useCallback, useEffect, useRef, useState } from 'react'
import { navigate } from '@electricui/utility-electron'

import { FolderPathSelector } from './FolderSelector'
import { Optimiser } from './Optimiser'

import { SettingsInterface } from './SettingsInterface'
import { RenderInterface } from './RenderInterface'
import { ToolpathVisualisation } from './ToolpathVisualisation'
import { RenderableTree } from './RenderableTree'
import { MaterialEditor } from './MaterialEditor'

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

        <RenderableTree />

        <MaterialEditor />

        <Optimiser />

        <RenderInterface />
      </div>
    </React.Fragment>
  )
}
