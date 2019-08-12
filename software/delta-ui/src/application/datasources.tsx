import { DeviceID } from '@electricui/core'

import { TimeSeriesFactory, DataSource } from '@electricui/core-timeseries'

/**
 * sourceFactory takes events from the device and turns them into 'data sources'. They are essentially component accessors that happen on the event stream.
 */
export function sourceFactory(device: DeviceID): DataSource[] {
  return [
    /**
     * Thermal sensor data
     */
    new DataSource({
      name: 'fan_data',
      filter: message => message.messageID === 'fan',
      columns: ['RPM', 'Setpoint'],
      processor: message => [message.payload.rpm, message.payload.setpoint],
    }),
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

    new TimeSeriesFactory('thermal_graph', ['fan_data']),
  ]
}
