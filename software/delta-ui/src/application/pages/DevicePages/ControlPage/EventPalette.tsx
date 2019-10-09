import {
  Button as BlueprintButton,
  ButtonGroup,
  Card,
  FileInput,
  Icon,
} from '@blueprintjs/core'
import { Cell, Grid } from 'styled-css-grid'
import {
  IntervalRequester,
  useDeviceMetadataKey,
  useHardwareState,
} from '@electricui/components-core'
import React, { ReactElement, useCallback, useEffect, useState } from 'react'

import { Button } from '@electricui/components-desktop-blueprint'
import { CALL_CALLBACK } from '@electricui/core'
import { Printer } from '@electricui/components-desktop'
import { SceneController } from './../SceneControl/SceneController'
import { useExtractSceneName } from './../../../hooks/useExtractSceneName'
import { useOpenDialogCallFunction } from './../../../hooks/useOpenDialog'
import { useTriggerAction } from '@electricui/core-actions'

const EventPalette = () => {
  const triggerAction = useTriggerAction()

  return (
    <>
      <IntervalRequester interval={100} variables={['rgb']} />
      <h3>Load Event Sequence from File</h3>
      <SceneController />
    </>
  )
}

export default EventPalette
