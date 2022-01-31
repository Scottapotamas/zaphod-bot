import {
  Alignment,
  Button,
  Card,
  Checkbox,
  FormGroup,
  Intent,
  NumericInput,
  Slider,
  Switch,
  Tag,
} from '@blueprintjs/core'
import { IconNames } from '@blueprintjs/icons'

import { Composition, Box } from 'atomic-layout'

import React, { useCallback, useEffect, useState } from 'react'
import { isCamera } from '../optimiser/camera'

import {
  getSetting,
  incrementViewportFrameVersion,
  setSetting,
  useStore,
} from './state'

function setThreeJSCamera() {
  const sceneCamera = getSetting(state => state.camera)
  const renderablesForFrame =
    getSetting(state => state.renderablesByFrame[state.viewportFrame]) ?? []

  const blenderCamera = renderablesForFrame.find(isCamera)

  if (sceneCamera && blenderCamera) {
    blenderCamera.alignCamera(sceneCamera)
  }
}

function OrbitCameraToggle() {
  const [enabled, setEnabled] = useState(true)

  const updateChecked: React.FormEventHandler<HTMLInputElement> = useCallback(
    event => {
      const checked = event.currentTarget.checked
      setEnabled(checked)
      const orbitControls = getSetting(state => state.orbitControls)

      if (orbitControls) {
        orbitControls.enabled = checked
      }

      // Search for the camera for this frame if we've disabled orbitControls
      if (getSetting(state => state.matchCameraAnimation)) {
        setThreeJSCamera()
      }
    },
    [],
  )

  useEffect(() => {
    return useStore.subscribe(
      state => state.viewportFrame,
      frameNumber => {
        if (getSetting(state => state.matchCameraAnimation)) {
          setThreeJSCamera()
        }
      },
    )
  })

  const toggleChecked = useCallback(_ => {
    setEnabled(enabled => !enabled)
  }, [])

  return (
    <div>
      <Switch
        style={{ width: '100%' }}
        inline
        checked={enabled}
        alignIndicator={Alignment.RIGHT}
        onChange={updateChecked}
      >
        Orbit Camera
      </Switch>
      <Button
        fill
        icon={IconNames.CAMERA}
        active={enabled}
        intent={enabled ? Intent.PRIMARY : Intent.NONE}
        onClick={toggleChecked}
      >
        Orbit Camera
      </Button>
    </div>
  )
}

function AnnotateOrderingToggle() {
  const [enabled, setEnabled] = useState(
    getSetting(state => state.visualisationSettings.annotateDrawOrder),
  )

  const updateChecked: React.FormEventHandler<HTMLInputElement> = useCallback(
    event => {
      const checked = event.currentTarget.checked
      setEnabled(checked)

      setSetting(state => {
        state.visualisationSettings.annotateDrawOrder = checked
        incrementViewportFrameVersion(state)
      })
    },
    [],
  )

  const toggleChecked = useCallback(blah => {
    setEnabled(enabled => !enabled)

    setSetting(state => {
      state.visualisationSettings.annotateDrawOrder = !enabled
      incrementViewportFrameVersion(state)
    })
  }, [])

  return (
    <div>
      <Switch
        style={{ width: '100%' }}
        inline
        alignIndicator={Alignment.RIGHT}
        checked={enabled}
        onChange={updateChecked}
      >
        Annotate Ordering
      </Switch>
      <Button
        fill
        icon={IconNames.PATH_SEARCH}
        active={enabled}
        intent={enabled ? Intent.PRIMARY : Intent.NONE}
        onClick={toggleChecked}
      >
        Annotate Ordering
      </Button>
    </div>
  )
}

export const ThreeSettingsInterface = () => {
  return (
    <div>
      <Composition templateCols="1fr 1fr" gap="1em" alignItems="center">
        <OrbitCameraToggle />
        <AnnotateOrderingToggle />
      </Composition>
    </div>
  )
}
