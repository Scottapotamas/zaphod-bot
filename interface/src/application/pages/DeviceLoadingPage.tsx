import React from 'react'

import { NonIdealState, ProgressBar } from '@blueprintjs/core'
import { RouteComponentProps } from '@reach/router'
import { useDeviceHandshakeProgresses } from '@electricui/components-core'

export const DeviceLoadingPage = (props: RouteComponentProps) => {
  const deviceHandshakes = useDeviceHandshakeProgresses()

  return (
    <div style={{ height: '100vh' }}>
      <NonIdealState
        icon="changes"
        title="Connecting"
        description={
          <React.Fragment>
            {deviceHandshakes.map(({ currentTask, complete, total }, i) => (
              <div
                key={i}
                style={{
                  minWidth: 400,
                }}
              >
                {currentTask}
                <br />
                <br />
                <ProgressBar value={total === 0 ? 0 : complete / total} />
              </div>
            ))}
          </React.Fragment>
        }
      />
    </div>
  )
}
