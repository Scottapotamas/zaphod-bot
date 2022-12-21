import { Button, Card } from '@blueprintjs/core'
import { Composition } from 'atomic-layout'
import { RouteComponentProps } from '@reach/router'

import React, { useCallback, useEffect, useRef, useState } from 'react'
import { navigate } from '@electricui/utility-electron'

import { OutlineCard } from 'src/application/components/OutlineCard'

import { FolderPathSelector } from './FolderSelector'
import { Optimiser } from './Optimiser'

import { PlannerSettingsInterface } from './PlannerSettingsInterface'
import { RenderInterface } from './RenderInterface'
import { ToolpathVisualisation } from './ToolpathVisualisation'
import { RenderableTree } from './RenderableTree'
import { MaterialEditorInterface } from './MaterialEditorInterface'

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
        <Button onClick={() => navigate(`/`)} style={{ margin: 10 }} outlined>
          Back to connections screen
        </Button>
        <Composition gap={10} padding={10}>
          <OutlineCard padding="2em">
            <PlannerSettingsInterface />
          </OutlineCard>

          <OutlineCard padding="2em">
              <FolderPathSelector />
              <RenderInterface />
          </OutlineCard>

          <Optimiser />
        </Composition>
      </div>
    </React.Fragment>
  )
}
