import React from 'react'
import { Composition, Box } from 'atomic-layout'
import { SectionHeading } from '../SectionHeading'

export const ServoConfig = () => {
  return (
    <Composition templateCols="1fr 1fr" justifyItems="center" gap={20}>
      <Box>
        <SectionHeading text="Servomotor" />

        {/* Requires homing */}
        {/* Steps per rotation */}
        {/* Maximum rotational velocity */}
        {/* Requires homing */}

      </Box>
      <Box>
        <SectionHeading text="" />
        
      </Box>
    </Composition>
  )
}
