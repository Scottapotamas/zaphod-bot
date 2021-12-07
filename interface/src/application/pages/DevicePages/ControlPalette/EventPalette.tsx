import React from 'react'

import {
  useDeviceMetadataKey,
  IntervalRequester,
  useHardwareState,
} from '@electricui/components-core'

// import { SceneController } from './SceneControl/SceneController'

import { Icon, Intent } from '@blueprintjs/core'

const QueueText = () => {
  const queue_depth = useHardwareState(state => state.queue.movements)
  const is_moving = useHardwareState(state => state.moStat.pathing_state) == 1
  const queue_depth_ui = useDeviceMetadataKey('uiSideMovementQueueDepth')

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
  const queue_depth_ui = useDeviceMetadataKey('uiSideLightQueueDepth')

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

const EventPalette = () => {
  const sceneFilePath = useDeviceMetadataKey('summary_file_path')

  return (
    <>
      <IntervalRequester interval={100} variables={['rgb']} />
      <h3>Load Event Sequence from File</h3>
      {/* <SceneController key={sceneFilePath} /> */}
      <p>This has been removed during template upgrade!!!</p>
      <CurrentRGB />
      <QueueText />
      <LEDQueueText />
    </>
  )
}

export default EventPalette
