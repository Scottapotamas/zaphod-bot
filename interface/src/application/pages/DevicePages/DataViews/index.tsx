import React, { useState } from 'react'

import { ThreeD } from './ThreeD'
import { ServoDetails } from './ServoDetails'
import { CoolingDetails } from './CoolingDetails'
import { Button, Tooltip, Position } from '@blueprintjs/core'
import { Composition, Box } from 'atomic-layout'

const DataViews = () => {
  const [page, setPage] = useState(0)

  return (
    <div>
      <Composition
        templateCols="1fr auto auto auto"
        justifyItems="center"
        gapCol={10}
      >
        <Box justifyItems="center">
          <h3 style={{ margin: '10px' }}>
            {page === 0 ? 'Hey' : null}
            {page === 1 ? 'Servo Feedback' : null}
            {page === 2 ? 'Internal Temperatures & Fan Control' : null}
          </h3>
        </Box>
        <Box>
          <Tooltip content="3D View">
            <Button
              minimal
              large
              icon="cube"
              onClick={() => setPage(0)}
              active={page === 0}
            />
          </Tooltip>
        </Box>
        <Box>
          <Tooltip content="Servo Health">
            <Button
              minimal
              large
              icon="doughnut-chart"
              onClick={() => setPage(1)}
              active={page === 1}
            />
          </Tooltip>
        </Box>
        <Box>
          <Tooltip content="Thermals">
            <Button
              minimal
              large
              icon="snowflake"
              onClick={() => setPage(2)}
              active={page === 2}
            />
          </Tooltip>
        </Box>
      </Composition>

      {page === 0 ? <ThreeD /> : null}
      {page === 1 ? <ServoDetails /> : null}
      {page === 2 ? <CoolingDetails /> : null}
    </div>
  )
}

export default DataViews
