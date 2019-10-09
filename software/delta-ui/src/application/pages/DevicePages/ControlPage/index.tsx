import { Composition } from 'atomic-layout'
import { IntervalRequester } from '@electricui/components-core'
import React from 'react'
import { RouteComponentProps } from '@reach/router'
import SystemController from './SystemController'
import SystemOverview from './SystemOverview'
import { ThreeD } from './ThreeD'

const controlAreas = `
ControlArea ThreeDArea
SystemOverviewArea ThreeDArea
`

const ControlPage = (props: RouteComponentProps) => (
  <React.Fragment>
    <IntervalRequester interval={100} variables={['moStat', 'cpos']} />
    <IntervalRequester interval={200} variables={['mo1', 'mo2', 'mo3']} />

    <Composition areas={controlAreas} gap={10} templateCols="1fr 1fr">
      {({ ControlArea, SystemOverviewArea, ThreeDArea }) => (
        <React.Fragment>
          <ControlArea>
            <SystemController />
          </ControlArea>
          <SystemOverviewArea>
            <SystemOverview />
          </SystemOverviewArea>
          <ThreeDArea>
            <ThreeD />
          </ThreeDArea>
        </React.Fragment>
      )}
    </Composition>
  </React.Fragment>
)

export default ControlPage
