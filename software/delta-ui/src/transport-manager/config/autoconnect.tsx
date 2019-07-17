import {
  DeviceManager,
  DeviceMetadata,
  DEVICE_EVENTS,
  MANAGER_EVENTS,
  Device,
  isSubset,
} from '@electricui/core'
import { DeviceManagerProxyPlugin } from '@electricui/components-core'

export type AutoConnectMetadata = Array<DeviceMetadata>

export class AutoConnectPlugin extends DeviceManagerProxyPlugin {
  deviceMetadatas: AutoConnectMetadata
  constructor(metadatas: AutoConnectMetadata) {
    super()
    this.deviceMetadatas = metadatas
    this.foundDeviceOrMetadataChange = this.foundDeviceOrMetadataChange.bind(
      this,
    )
  }

  setupProxyHandlers() {
    this.deviceManager!.on(
      MANAGER_EVENTS.FOUND_DEVICE,
      this.foundDeviceOrMetadataChange,
    )
    this.deviceManager!.on(
      MANAGER_EVENTS.DEVICE_METADATA_CHANGE,
      this.foundDeviceOrMetadataChange,
    )
  }

  teardownProxyHandlers() {
    this.deviceManager!.removeListener(
      MANAGER_EVENTS.FOUND_DEVICE,
      this.foundDeviceOrMetadataChange,
    )
    this.deviceManager!.removeListener(
      MANAGER_EVENTS.DEVICE_METADATA_CHANGE,
      this.foundDeviceOrMetadataChange,
    )
  }

  foundDeviceOrMetadataChange(device: Device) {
    const deviceMetadata = device.getMetadata()

    for (const subsetMetadata of this.deviceMetadatas) {
      if (isSubset(deviceMetadata, subsetMetadata)) {
        // this is a device that we want to auto-connect to
        device
          .addUsageRequest('ui', () => {
            // noop?
          })
          .catch(e => {
            console.warn(
              "Wasn't able to autoconnect to device with metadata",
              subsetMetadata,
              'for reason',
              e,
            )
          })
      }
    }
  }
}
