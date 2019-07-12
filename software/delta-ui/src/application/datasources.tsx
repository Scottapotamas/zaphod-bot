import { DeviceID } from '@electricui/core'

import { TimeSeriesFactory, DataSource } from '@electricui/core-timeseries'

/**
 * sourceFactory takes events from the device and turns them into 'data sources'. They are essentially component accessors that happen on the event stream.
 */
export function sourceFactory(device: DeviceID): DataSource[] {
  return [
    /**
     * The LED State
     */
    new DataSource({
      name: 'fan_data',
      filter: message => message.messageID === 'fan',
      columns: ['RPM', 'Setpoint'],
      processor: message => [message.payload.rpm, message.payload.setpoint],
    }),
  ]
}

export function timeseriesFactories(device: DeviceID): TimeSeriesFactory[] {
  return [new TimeSeriesFactory('graph', ['fan_data'])]
}
