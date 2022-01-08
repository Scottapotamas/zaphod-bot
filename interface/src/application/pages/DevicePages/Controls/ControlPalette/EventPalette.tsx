import React from 'react'

import {
  IntervalRequester,
  useHardwareState,
} from '@electricui/components-core'

import { Printer } from '@electricui/components-desktop'
// import { SceneController } from './SceneControl/SceneController'

import { Icon, Intent } from '@blueprintjs/core'

import { useSetting } from '../../../toolpaths/interface/state'

const QueueText = () => {
  const queue_depth = useHardwareState(state => state.queue.movements)
  const is_moving = useHardwareState(state => state.moStat.pathing_state) == 1
  const queue_depth_ui = useSetting(state => state.movementQueueUI)

  let iconColour: Intent

  if (queue_depth == 0) {
    iconColour = Intent.NONE
  } else if (queue_depth > 0 && queue_depth < 25) {
    if (is_moving) {
      iconColour = Intent.SUCCESS
    } else {
      iconColour = Intent.WARNING
    }
  } else {
    iconColour = Intent.DANGER
  }

  return (
    <div>
      <Icon icon="move" intent={iconColour} /> {queue_depth} ({queue_depth_ui})
    </div>
  )
}

const LEDQueueText = () => {
  const queue_depth = useHardwareState(state => state.queue.lighting)
  const queue_depth_ui = useSetting(state => state.lightQueueUI)

  let iconColour: Intent

  if (queue_depth == 0) {
    iconColour = Intent.NONE
  } else if (queue_depth > 0 && queue_depth < 35) {
    iconColour = Intent.SUCCESS
  } else {
    iconColour = Intent.WARNING
  }

  return (
    <div>
      <Icon icon="lightbulb" intent={iconColour} /> {queue_depth} (
      {queue_depth_ui})
    </div>
  )
}

const CurrentRGB = () => {
  const red_led = useHardwareState(state => state.rgb.red)
  const green_led = useHardwareState(state => state.rgb.green)
  const blue_led = useHardwareState(state => state.rgb.blue)

  return (
    <Icon
      icon="full-circle"
      iconSize={60}
      color={`rgb(${red_led},${green_led},${blue_led})`}
    />
  )
}

import { FolderPathSelector } from './../../../toolpaths/interface/FolderSelector'
import { Optimiser } from './../../../toolpaths/interface/Optimiser'

import { PlannerSettingsInterface } from 'src/application/pages/toolpaths/interface/PlannerSettingsInterface'
import { ThreeSettingsInterface } from 'src/application/pages/toolpaths/interface/ThreeSettingsInterface'
import { RenderInterface } from './../../../toolpaths/interface/RenderInterface'
import { RenderableTree } from './../../../toolpaths/interface/RenderableTree'
import { MaterialEditor } from './../../../toolpaths/interface/MaterialEditor'
import { MSGID } from 'src/application/typedState'

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

      <QueueText />
      <LEDQueueText />
      <IntervalRequester
        variables={[MSGID.MOTION, MSGID.QUEUE_INFO]}
        interval={200}
      />
      <Printer accessor={state => JSON.stringify(state[MSGID.MOTION])} />
    </>
  )
}
