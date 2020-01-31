import React, { useState } from 'react'

import { ThreeD } from './ThreeD'
import { ServoDetails } from './ServoDetails'
import { CoolingDetails } from './CoolingDetails'
import { Button, Tooltip, Position } from '@blueprintjs/core'

const DataViews = () => {
  const [page, setPage] = useState(0)

  return (
    <div>
      <Tooltip content="3D View">
        <Button
          minimal
          large
          icon="cube"
          onClick={() => setPage(0)}
          active={page === 0}
        />
      </Tooltip>
      <Tooltip content="Servo Health">
        <Button
          minimal
          large
          icon="doughnut-chart"
          onClick={() => setPage(1)}
          active={page === 1}
        />
      </Tooltip>

      <Tooltip content="Thermals">
        <Button
          minimal
          large
          icon="snowflake"
          onClick={() => setPage(2)}
          active={page === 2}
        />
      </Tooltip>

      {page === 0 ? <ThreeD /> : null}
      {page === 1 ? <ServoDetails /> : null}
      {page === 2 ? <CoolingDetails /> : null}
    </div>
  )
}

export default DataViews
