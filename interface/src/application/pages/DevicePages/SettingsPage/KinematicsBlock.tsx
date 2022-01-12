import {
  Alignment,
  Button as BlueprintButton,
  ButtonGroup,
  Navbar,
  Popover,
  Position,
  Tooltip,
  HTMLTable,
} from '@blueprintjs/core'
import {
  IntervalRequester,
  useDeviceConnect,
  useDeviceConnectionRequested,
  useDeviceDisconnect,
  useHardwareState,
} from '@electricui/components-core'

import React from 'react'
import { Composition, Box } from 'atomic-layout'
import {
  Statistic,
  Button,
  NumberInput,
  Slider,
} from '@electricui/components-desktop-blueprint'
import { Printer } from '@electricui/components-desktop'

import KinematicsDiagram from './KinematicsDiagram'

const KinematicsAreas = `
KinematicsLimitsArea KinematicsValuesArea
`

export const KinematicsBlock = () => {
  const shoulder = useHardwareState(state => state.kinematics.shoulder_radius)
  const bicep = useHardwareState(state => state.kinematics.bicep_length)
  const forearm = useHardwareState(state => state.kinematics.forearm_length)
  const eff = useHardwareState(state => state.kinematics.effector_radius)

  return (
    <Composition
      areas={KinematicsAreas}
      gap={30}
      padding={20}
      templateCols="1fr auto"
    >
      {({ KinematicsLimitsArea, KinematicsValuesArea }) => (
        <React.Fragment>
          <KinematicsLimitsArea>
            <h2>Kinematics Overview</h2>
            <HTMLTable striped style={{ minWidth: '100%' }}>
              <thead>
                <td></td>
                <td>Min</td>
                <td>Max</td>
                <td>Inverted</td>
              </thead>
              <tbody>
                <tr>
                  <td>Radius</td>
                  <td></td>
                  <td>
                    <Printer
                      accessor={state => state.kinematics.limit_radius}
                    />
                  </td>
                  <td></td>
                </tr>
                <tr>
                  <td>Z</td>
                  <td>
                    <Printer accessor={state => state.kinematics.limit_z_min} />
                  </td>
                  <td>
                    <Printer accessor={state => state.kinematics.limit_z_max} />
                  </td>
                  <td>
                    <Printer accessor={state => state.kinematics.flip_z} />
                  </td>
                </tr>
              </tbody>
            </HTMLTable>
            <br />
            <h3>Adjust Z axis alignment</h3>
            <Slider
              min={0}
              max={360}
              labelStepSize={45}
              stepSize={15}
              sendOnlyOnRelease
              labelRenderer={val => `${val}º`}
            >
              <Slider.Handle accessor="rotZ" />
            </Slider>
          </KinematicsLimitsArea>
          <KinematicsValuesArea>
            <KinematicsDiagram
              shoulderDistance={shoulder}
              bicepLength={bicep}
              forearmLength={forearm}
              effectorDistance={eff}
            />
          </KinematicsValuesArea>
        </React.Fragment>
      )}
    </Composition>
  )
}
