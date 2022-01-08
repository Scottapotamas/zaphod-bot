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
  return (
    <>
      <FolderPathSelector />

      <PlannerSettingsInterface />
      <ThreeSettingsInterface />

      <RenderableTree />

      <MaterialEditor />

      <Optimiser />

      <RenderInterface />

      {/* <Printer accessor={state => JSON.stringify(state[MSGID.MOTION])} /> */}
      <IntervalRequester
        variables={[MSGID.MOTION, MSGID.QUEUE_INFO]}
        interval={200}
      />
      <Printer accessor={state => JSON.stringify(state[MSGID.MOTION])} />
    </>
  )
}
