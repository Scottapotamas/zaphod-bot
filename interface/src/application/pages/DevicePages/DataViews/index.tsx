import React, { useState } from 'react'

import { ThreeD } from './ThreeD'
import { ServoDetails } from './ServoDetails'
import { CoolingDetails } from './CoolingDetails'
import { Button, Tooltip, Position, Card } from '@blueprintjs/core'
import { Composition, Box } from 'atomic-layout'

const DataViews = () => {
  const [page, setPage] = useState(1)

  return (
    <React.Fragment>
      <Card style={{ height: '100%' }}>
        <Composition
          templateCols="1fr auto auto auto"
          justifyItems="center"
          gapCol={10}
        >
          <Box justifyItems="center">
            <h3 style={{ margin: '10px' }}>
              {/* {page === 0 ? 'Hey' : null} */}
              {page === 1 ? 'Servo Feedback' : null}
              {page === 2 ? 'Temperatures & Fan' : null}
            </h3>
          </Box>
          {/* <Box>
            <Tooltip content="3D View" position={Position.BOTTOM}>
              <Button
                minimal
                large
                icon="cube"
                onClick={() => setPage(0)}
                active={page === 0}
              />
            </Tooltip>
          </Box> */}
          <Box>
            <Tooltip content="Servo Health" position={Position.BOTTOM}>
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
            <Tooltip content="Thermals" position={Position.BOTTOM}>
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
        <br />
        {page === 1 ? <ServoDetails /> : null}
        {page === 2 ? <CoolingDetails /> : null}
      </Card>
    </React.Fragment>
  )
}

export default DataViews
