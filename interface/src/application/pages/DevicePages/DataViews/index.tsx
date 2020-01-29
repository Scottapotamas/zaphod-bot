import React from 'react'

import { ThreeD } from './ThreeD'
import { ServoDetails } from './ServoDetails'
import { CoolingDetails } from './CoolingDetails'
import { Button, Tooltip, Position } from '@blueprintjs/core'

const DataViews = () => {
  return (
    <div>
      <Tooltip content="3D View">
        <Button minimal large icon="cube" />
      </Tooltip>
      <Tooltip content="Servo Health">
        <Button minimal large icon="doughnut-chart" />
      </Tooltip>

      <Tooltip content="Thermals">
        <Button minimal large icon="snowflake" />
      </Tooltip>

      <ThreeD />
      <ServoDetails />
      <CoolingDetails />
    </div>
  )
}

export default DataViews
