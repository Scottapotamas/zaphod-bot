import { DeviceManager, Message, Device } from '@electricui/core'
import { Action, RunActionFunction } from '@electricui/core-actions'
import path from 'path'
import os from 'os'
import fs from 'fs'

export type CameraSettings = {
  iso: number
  shutter: number
  aperture: number
}

export type CameraTriggerOptions = {
  filePath: string
}

function getCamera(deviceManager: DeviceManager) {
  const devices = deviceManager.devices

  let camera: Device | null = null

  for (const device of devices.values()) {
    const metadata = device.getMetadata()
    const type = metadata['type']

    if (type === 'Camera') {
      camera = device
      break
    }
  }

  if (camera === null) {
    console.warn('No camera connected?')
    return null
  }

  return camera
}

/**
 * Sets camera settings
 */
const cameraSettings = new Action(
  'camera_settings',
  async (
    deviceManager: DeviceManager,
    runAction: RunActionFunction,
    options: CameraSettings,
  ) => {
    const camera = getCamera(deviceManager)
    let localSavePath: string | null = null

    if (!camera) {
      return
    }

    const { iso, aperture, shutter } = options

    const isoMsg = new Message('/imgsettings/iso', String(iso)) // prettier-ignore
    const apertureMsg = new Message('/capturesettings/aperture', String(aperture)) // prettier-ignore
    const shutterMsg = new Message('/capturesettings/shutterspeed', String(shutter)) // prettier-ignore

    await camera.write(isoMsg)
    await camera.write(apertureMsg)
    await camera.write(shutterMsg)
  },
)

const cameraTrigger = new Action(
  'capture',
  async (
    deviceManager: DeviceManager,
    runAction: RunActionFunction,
    options: CameraTriggerOptions,
  ) => {
    const camera = getCamera(deviceManager)
    let localSavePath: string | null = null

    if (!camera) {
      return
    }

    const metadata = camera.getMetadata()

    if (metadata['localSavePath'] && metadata['localSavePath'] !== '') {
      localSavePath = metadata['localSavePath']
    }

    if (localSavePath === null) {
      console.warn("Camera doesn't have a save path")
      return
    }

    const fileSavePath = path.join(localSavePath, options.filePath)
    // create the folders required
    const dir = path.dirname(fileSavePath)

    if (!fs.existsSync(dir)) {
      fs.mkdirSync(dir, { recursive: true })
    }

    const captureMessage = new Message('__capture', { savePath: fileSavePath })

    camera.write(captureMessage)
  },
)

/**
 * Sets the save path for all relative files after this
 */
const setLocalSavePath = new Action(
  'set_save_path',
  async (
    deviceManager: DeviceManager,
    runAction: RunActionFunction,
    savePath: string,
  ) => {
    const camera = getCamera(deviceManager)

    if (!camera) {
      return
    }

    camera.addMetadata({
      localSavePath: savePath,
    })

    console.log('Set save path to ', savePath)
  },
)

export { cameraSettings, cameraTrigger, setLocalSavePath }
