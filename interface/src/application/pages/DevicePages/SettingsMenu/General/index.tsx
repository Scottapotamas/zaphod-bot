import React from 'react'
import { Composition, Box } from 'atomic-layout'

import { InvertedSetting } from './Inverted'
import { BoundaryViolations } from './BoundaryViolations'
import { ZAxisOffset } from './ZAxisOffset'
import { KinematicsLimits } from './KinematicsLimits'
import { MiscToggles } from './MiscToggles'

export const GeneralSettings = () => {
  return (
    <Composition templateCols="auto 1fr" justifyItems="center" gap={20}>
      <Box>
        <InvertedSetting />
      </Box>
      <Box>
        <BoundaryViolations />
      </Box>
      <Box>
        <MiscToggles />
      </Box>
      <Box>
        <KinematicsLimits />
      </Box>
      <Box>
        {/* Something else goes here? */}
      </Box>
      <Box style={{ minWidth: '250px' }}>
        <ZAxisOffset />
      </Box>
    </Composition>
  )
}
