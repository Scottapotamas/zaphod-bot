import React from 'react'
import {
  useDeviceIDByMetadata,
  DeviceIDContextProvider,
  usePollForDevices,
  useDeviceDisconnect,
  useDeviceConnect,
  useDeviceConnectionRequested,
  useDeadline,
  useDeviceHasAcceptableConnection,
} from '@electricui/components-core'
import { TimeSeriesProvider } from '@electricui/core-timeseries'

import { Button, Tag } from '@blueprintjs/core'
import { DeviceID } from '@electricui/core'
import { NumberInput } from '@electricui/components-desktop-blueprint'
import { FXMODE, MODE_SELECT } from '../../../../application/typedState'

function ConnectDisconnectButton(props: { deviceID: DeviceID }) {
  const disconnect = useDeviceDisconnect()
  const connect = useDeviceConnect()
  const connectionRequested = useDeviceConnectionRequested()
  const getDeadline = useDeadline()

  if (connectionRequested) {
    return (
      <Button
        minimal
        fill
        intent="danger"
        icon="cross"
        text="Disconnect"
        onClick={() => {
          disconnect().catch(err => {
            console.warn('Failed to disconnect', err)
          })
        }}
      />
    )
  } else {
    return (
      <Button
        minimal
        fill
        icon="link"
        intent="success"
        text="Connect again"
        onClick={() => {
          const cancellationToken = getDeadline()

          connect(cancellationToken).catch(err => {
            if (cancellationToken.caused(err)) {
              return
            }

            console.warn('Failed to connect', err)
          })
        }}
      />
    )
  }
}

// Finds the deviceID from the metadata and produces an isolated device context for connecting and disconnecting, as well as basic controls.
export function DMXControlTab() {
  let dmxDeviceID = useDeviceIDByMetadata({ device_type: 'dmx' } as any) // TODO: Check after update to latest template

  // const backupID = useDeviceIDByTransportKey('serial') // just grab any ol' eUI device and check if it has a good ID
  // dmxDeviceID = backupID!

  const { poll, polling } = usePollForDevices()

  const hasAcceptableConnection = useDeviceHasAcceptableConnection(
    dmxDeviceID ?? ('iwishwehadoptionalhooks' as DeviceID),
  )

  if (!dmxDeviceID) {
    return (
      <>
        <span>DMX Device Link</span>
        <Button onClick={poll} disabled={polling} fill loading={polling}>
          Device not found, refresh?
        </Button>
      </>
    )
  }

  return (
    <DeviceIDContextProvider deviceID={dmxDeviceID}>
      <TimeSeriesProvider deviceID={dmxDeviceID}>
        <span>DMX Device Link</span> <ConnectDisconnectButton deviceID={dmxDeviceID} />
        <span>DMX Level Override</span>
        <NumberInput
          fill
          min={0}
          max={100}
          stepSize={1}
          majorStepSize={25}
          disabled={!hasAcceptableConnection}
          rightElement={<Tag>{'%'}</Tag>}
          accessor={state =>
            typeof state.fixture !== 'undefined'
              ? Math.round(((state.fixture[0]?.intensity ?? 0) / 255) * 100)
              : 'disconnected'
          }
          writer={(state, val) => {
            if (typeof state.fixture === 'undefined') {
              console.error(`no DMX fixture found in state, can't set intensity`)
              return
            }

            state.fixture[0].intensity = Math.round((val / 100) * 255)
            state.fixture[0].mode = MODE_SELECT.MODE_MANUAL
            state.fixture[0].fxSelect = FXMODE.FX_UNDEFINED
            state.fixture[0].fxFrequency = 0
            state.fixture[0].fxTrigger = 0
          }}
        />
      </TimeSeriesProvider>
    </DeviceIDContextProvider>
  )
}
