import React from 'react'
import { RouteComponentProps } from '@reach/router'

import { Card } from '@blueprintjs/core'
import { Composition, Box } from 'atomic-layout'

import SystemController from './ControlPalette/SystemController'
import SystemOverview from './SystemOverview'
import DataViews from './DataViews'

const controlAreas = `
Control Visualisation
SystemInfo Visualisation
`

const MainWindow = (props: RouteComponentProps) => (
  <React.Fragment>
    <Composition areas={controlAreas} gap={10} templateCols="1fr 1fr">
      {Areas => (
        <React.Fragment>
          <Areas.Control>
            <Card>
              <SystemController />
            </Card>
          </Areas.Control>
          <Areas.SystemInfo>
            <Card>
              <SystemOverview />
            </Card>
          </Areas.SystemInfo>
          <Areas.Visualisation>
            <Card>
              <DataViews />
            </Card>
          </Areas.Visualisation>
        </React.Fragment>
      )}
    </Composition>
  </React.Fragment>
)

export default MainWindow
