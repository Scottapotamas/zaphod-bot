import { Button, Card } from '@blueprintjs/core'
import { Composition } from 'atomic-layout'
import { RouteComponentProps } from '@reach/router'

import React, { useCallback, useEffect, useRef, useState } from 'react'
import { navigate } from '@electricui/utility-electron'

import { FolderPathSelector } from './FolderSelector'
import { Optimiser } from './Optimiser'

import { PlannerSettingsInterface } from './PlannerSettingsInterface'
import { ThreeSettingsInterface } from './ThreeSettingsInterface'
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
<Composition gap={10} padding={10}>

          <Card>
            <PlannerSettingsInterface />
          </Card>

          <Card>
            <FolderPathSelector />
            <RenderableTree />
            <br/>
            <MaterialEditor />
            <RenderInterface />
          </Card>

          <Optimiser />


        </Composition>
        <br/><br/><br/><br/>
          {/* TODO: Move these view controls to below the three area */}
          {/* Consider adding button to hide/show the delta model */}
          {/* Consider a 'reset view' button? */}
          <ThreeSettingsInterface />
      </div>
    </React.Fragment>
  )
}
