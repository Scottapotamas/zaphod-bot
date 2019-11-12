import { Device, DeviceManager } from '@electricui/core'

const deltaName = 'Zaphod Beeblebot'

function getDelta(deviceManager: DeviceManager) {
  const devices = deviceManager.devices

  let delta: Device | null = null

  for (const device of devices.values()) {
    const metadata = device.getMetadata()
    const name = metadata['name']

    if (name === deltaName) {
      delta = device
      break
    }
  }

  if (delta === null) {
    throw new Error('No delta connected?')
  }

  return delta
}

export { getDelta }
