import React, { useCallback, useState } from 'react'

import {
  IntervalRequester,
  useHardwareState,
} from '@electricui/components-core'

import { Printer } from '@electricui/components-desktop'
import { Composition, Box } from 'atomic-layout'

import {
  Icon,
  Intent,
  Card,
  Callout,
  Tag,
  Collapse,
  Button,
} from '@blueprintjs/core'
import { IconNames } from '@blueprintjs/icons'

import { MSGID } from 'src/application/typedState'

import { FolderPathSelector } from './../../../toolpaths/interface/FolderSelector'
import { Optimiser } from './../../../toolpaths/interface/Optimiser'

import { PlannerSettingsInterface } from 'src/application/pages/toolpaths/interface/PlannerSettingsInterface'
import { ThreeSettingsInterface } from 'src/application/pages/toolpaths/interface/ThreeSettingsInterface'
import { RenderInterface } from './../../../toolpaths/interface/RenderInterface'
import { RenderableTree } from './../../../toolpaths/interface/RenderableTree'
import { MaterialEditor } from './../../../toolpaths/interface/MaterialEditor'

export const EventPalette = () => {
  let [settingsExpanded, setSettingsExpanded] = useState(false)

  const controlSettingsExpansion = useCallback(_ => {
    setSettingsExpanded(enabled => !enabled)
  }, [])

  return (
    <>
      <IntervalRequester
        variables={[MSGID.MOTION, MSGID.QUEUE_INFO]}
        interval={200}
      />

      <Composition gap={10}>
        <Collapse isOpen={settingsExpanded}>
          <Composition templateCols="1fr 1fr" gap={10}>
            <Card>
              <PlannerSettingsInterface />
            </Card>
            <Card>
              <RenderableTree />
              <MaterialEditor />
            </Card>
          </Composition>
        </Collapse>

        <Button
          minimal
          outlined
          intent={Intent.PRIMARY}
          small
          onClick={controlSettingsExpansion}
          icon={
            settingsExpanded ? IconNames.CHEVRON_DOWN : IconNames.CHEVRON_UP
          }
        >
          {/* Consider a label here to explain it's a settings expansion? */}
        </Button>

        <Card>
          <FolderPathSelector />

          <RenderInterface />
        </Card>

        <Optimiser />
      </Composition>

      {/* <ThreeSettingsInterface /> */}

      {/* <Printer accessor={state => JSON.stringify(state[MSGID.MOTION])} /> */}
    </>
  )
}
