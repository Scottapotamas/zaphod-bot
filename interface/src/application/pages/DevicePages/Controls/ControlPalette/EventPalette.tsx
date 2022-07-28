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
import { OutlineCard } from 'src/application/components/OutlineCard'

import { MSGID } from 'src/application/typedState'

import { FolderPathSelector } from './../../../toolpaths/interface/FolderSelector'
import { Optimiser } from './../../../toolpaths/interface/Optimiser'
import { RenderInterface } from './../../../toolpaths/interface/RenderInterface'
import { PlannerSettingsInterface } from 'src/application/pages/toolpaths/interface/PlannerSettingsInterface'

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
          <Composition templateCols="1fr" gap={10}>
            <OutlineCard padding="1em">
              <PlannerSettingsInterface />
            </OutlineCard>
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

        <OutlineCard>
          <Composition templateCols="1fr" gap={10} padding={10}>
            <FolderPathSelector />

            <RenderInterface />

            <Optimiser />
          </Composition>
        </OutlineCard>
      </Composition>
    </>
  )
}
