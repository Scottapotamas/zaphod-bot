import React, { useState } from 'react'

import { ServoDetails } from './ServoDetails'
import { Button, Tooltip, Position, Card } from '@blueprintjs/core'
import { Composition, Box } from 'atomic-layout'

const DataViews = () => {
  return (
    <React.Fragment>
      <Card style={{ height: '100%' }}>
        <ServoDetails />
      </Card>
    </React.Fragment>
  )
}

export default DataViews
