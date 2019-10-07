import { DataSource, TimeSeriesFactory } from '@electricui/core-timeseries'

import { DeviceID } from '@electricui/core'

/**
 * sourceFactory takes events from the device and turns them into 'data sources'. They are essentially component accessors that happen on the event stream.
 */
export function sourceFactory(device: DeviceID): DataSource[] {
  return [
    /**
     * Servo operating info
     */
    new DataSource({
      name: 'servoA_angle',
      filter: message => message.messageID === 'mo1',
      columns: ['A'],
      processor: message => [message.payload.target_angle],
    }),
    new DataSource({
      name: 'servoB_angle',
      filter: message => message.messageID === 'mo2',
      columns: ['B'],
      processor: message => [message.payload.target_angle],
    }),
    new DataSource({
      name: 'servoC_angle',
      filter: message => message.messageID === 'mo3',
      columns: ['C'],
      processor: message => [message.payload.target_angle],
    }),
    new DataSource({
      name: 'servoA_watts',
      filter: message => message.messageID === 'mo1',
      columns: ['A'],
      processor: message => [message.payload.power],
    }),
    new DataSource({
      name: 'servoB_watts',
      filter: message => message.messageID === 'mo2',
      columns: ['B'],
      processor: message => [message.payload.power],
    }),
    new DataSource({
      name: 'servoC_watts',
      filter: message => message.messageID === 'mo3',
      columns: ['C'],
      processor: message => [message.payload.power],
    }),
  ]
}

export function timeseriesFactories(device: DeviceID): TimeSeriesFactory[] {
  return [
    new TimeSeriesFactory('motor_angles', [
      'servoA_angle',
      'servoB_angle',
      'servoC_angle',
    ]),
    new TimeSeriesFactory('motor_power', [
      'servoA_watts',
      'servoB_watts',
      'servoC_watts',
    ]),
  ]
}
