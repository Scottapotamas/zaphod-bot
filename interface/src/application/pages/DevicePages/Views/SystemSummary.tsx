import React from 'react'

import { Composition, Box } from 'atomic-layout'

import { CPUTag } from './SummaryTags/CPUTag'
import { FanTag } from './SummaryTags/FanTag'
import { QueueChart } from './SummaryTags/QueueChart'
import { EffectorSpeedTag } from './SummaryTags/EffectorSpeedTag'

const layoutDescription = `
  CPU Chart
  Fan Chart
  Speed Chart
`

export const SystemSummary = () => {

  return (
    <Composition areas={layoutDescription} templateCols="1fr 1fr" gap={10}>
      {Areas => (
        <React.Fragment>
          <Areas.CPU>
            <CPUTag />
          </Areas.CPU>

          <Areas.Fan>
            <FanTag />
          </Areas.Fan>

          <Areas.Speed>
            <EffectorSpeedTag isLarge/>
          </Areas.Speed>

          <Areas.Chart>
            <QueueChart />
          </Areas.Chart>
        </React.Fragment>
      )}
    </Composition>
  )
}
