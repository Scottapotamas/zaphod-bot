import React from 'react'

import { Composition, Box } from 'atomic-layout'

import { CPUTag } from './SummaryTags/CPUTag'
import { QueueChart } from './SummaryTags/QueueChart'

const layoutDescription = `
  CPU Chart
`

export const SystemSummary = () => {

  return (
    <Composition areas={layoutDescription} templateCols="1fr 1fr" gap={10}>
      {Areas => (
        <React.Fragment>
          <Areas.CPU>
            <CPUTag />
          </Areas.CPU>

          <Areas.Chart>
            <QueueChart />
          </Areas.Chart>
        </React.Fragment>
      )}
    </Composition>
  )
}
