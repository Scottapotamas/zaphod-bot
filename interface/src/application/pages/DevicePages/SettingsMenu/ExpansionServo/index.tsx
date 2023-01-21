import React from 'react'
import { Composition, Box } from 'atomic-layout'

import { ServoConfig } from './ServoConfig'
import { ApplicationConfig } from './ApplicationConfig'

export const ExpansionServoSettings = () => {
  return (
    <Composition templateCols="1fr" justifyItems="center" gap={20}>
      <Box>
        <ServoConfig />
      </Box>
      <Box>
        <ApplicationConfig />
      </Box>
    </Composition>
  )
}
