import * as React from 'react'
import { useRef, useEffect } from 'react'

import {
  Environment,
  ControlledGroup,
  GLTF,
  OrbitControls,
} from '@electricui/components-desktop-three'
import { useHardwareState } from '@electricui/components-core'


import { useFrame, useThree } from '@react-three/fiber'
import { Mesh, Group } from 'three'

import { kinematics_point_to_angle, CartesianPoint } from './inverse_kinematics'

import DeltaBaseModel from './models/base.glb'
import DeltaBicepModel from './models/bicep.glb'
import DeltaEffectorModel from './models/effector.glb'
import DeltaForearmModel from './models/forearm.glb'

GLTF.preload(DeltaBaseModel)
GLTF.preload(DeltaBicepModel)
GLTF.preload(DeltaEffectorModel)
GLTF.preload(DeltaForearmModel)

const arm_count: number = 3
const bicep_length_mm: number = 180
const minimum_angle: number = 42

function AxisLines() {
  return (
    <group position={[0, 0, 0]} scale={[1, 1, 1]}>
      <mesh>
        <boxBufferGeometry attach="geometry" args={[1, 100, 1]} />
        <meshStandardMaterial attach="material" color="red" roughness={0.6} />
      </mesh>
      <mesh>
        <boxBufferGeometry attach="geometry" args={[100, 1, 1]} />
        <meshStandardMaterial attach="material" color="blue" roughness={0.6} />
      </mesh>
      <mesh>
        <boxBufferGeometry attach="geometry" args={[1, 1, 100]} />
        <meshStandardMaterial attach="material" color="green" roughness={0.6} />
      </mesh>
    </group>
  )
}

// Modify the camera's target position - looking at origin makes composition difficult
function CameraViewTarget() {
  // useFrame(state => {
  //   state.camera.lookAt(0, 50, 0)
  // })

  return (
    <></>
    //   <mesh>
    //     <sphereBufferGeometry attach="geometry" args={[7, 20, 20]} />
    //     <meshStandardMaterial attach="material" color="magenta" roughness={0.6} />
    //   </mesh>
  )
}

interface ForearmHelperProps {
  elbowPosition: number[],
  wristPosition: number[]
}

function ForearmHelper(props: ForearmHelperProps) {
  const ref = useRef<Group>(null!)
  //   console.log('Forearm at:', props.elbowPosition, 'looking at:', props.wristPosition)

  // useFrame(state => {
  //   if (ref && ref.current) {
  //     ref.current.lookAt(
  //       props.wristPosition[0],
  //       props.wristPosition[1],
  //       props.wristPosition[2],
  //     )
  //   }
  // })

  return (
    <group ref={ref} position={[props.elbowPosition[0], props.elbowPosition[1], props.elbowPosition[2]]}>
      {/* Highlight the shoulder joint with red */}
      <mesh>
        <sphereBufferGeometry attach="geometry" args={[7, 20, 20]} />
        <meshStandardMaterial attach="material" color="red" roughness={0.6} />
      </mesh>

      {/* <GLTF
        asset={DeltaForearmModel}
        position={[0, 0, -43]}
        rotation={[0, Math.PI / 1, 0]}
      /> */}
    </group>
  )
}

interface ArmAssemblyProps {
  shoulderAngle: number,
  wristCentroid: number[],
}

function ArmAssembly(props: ArmAssemblyProps) {
  return (
    <group
      position={[-50, 0, 3]}
      rotation={[0, 0, (props.shoulderAngle * Math.PI) / 180.0]}
    >
      <GLTF
        asset={DeltaBicepModel}
        position={[0, 0, -0.5]}
        rotation={[Math.PI / 2, (-27.75 * Math.PI) / 180.0, Math.PI]}
      />

<mesh>
        <sphereBufferGeometry attach="geometry" args={[10, 20, 20]} />
        <meshStandardMaterial attach="material" color="magenta" roughness={0.6} />
      </mesh>


      {/* TODO: Find actual 'effector' mounting location offset */}
      <ForearmHelper
        elbowPosition={[-bicep_length_mm, 0, -25.5]}
        wristPosition={[
          props.wristCentroid[0],
          props.wristCentroid[1],
          props.wristCentroid[2],
        ]}
      />
      <ForearmHelper
        elbowPosition={[-bicep_length_mm, 0, +22.5]}
        wristPosition={[
          props.wristCentroid[0],
          props.wristCentroid[1],
          props.wristCentroid[2],
        ]}
      />
    </group>
  )
}

const RiggedModel = () => {

  const xPos = useHardwareState(state => state.cpos.x)
  const yPos = useHardwareState(state => state.cpos.y)
  const zPos = useHardwareState(state => state.cpos.z)

  let effector_position: CartesianPoint = { x: xPos, y: yPos, z: zPos }

  let arm_angles = kinematics_point_to_angle(effector_position)

  //   arm_angles.forEach( (angle) => {
  //     angle = -1*angle - minimum_angle
  //     });

  // console.log('Arm kinematics:', arm_angles)

  return (
    <div >
      {/* style={{ height: 600, width: 900 }} */}
      <Environment
        camera={{
          fov: 60,
          position: [0, 150, 500],
          
          // debug pos
          //   position: [0, 600, 0],
          //   position: [-300, 300, 4],  // aligned with an arm
          //   position: [100, 100, 150], // aligned with an arm
        }}
        
      >
        <OrbitControls />
        <CameraViewTarget />
        <AxisLines />

        <ambientLight intensity={0.2} />
        <directionalLight intensity={0.4} position={[0, 100, 0]} color="#FFF" />

        <group position={[0, -50, 0]}>
          {/* Base positioned such that threeJS '[0,0,0' is aligned line with the servo shaft center */}
          <GLTF asset={DeltaBaseModel} position={[2.5, -135, -5.5]} />

          {/* End Effector */}
          <group
            position={[
              effector_position.x,
              effector_position.z + 190,
              effector_position.y,
            ]}
          >
            <GLTF
              asset={DeltaEffectorModel}
              position={[-43.25, -3, -0.25]}
              rotation={[0, Math.PI / -2, 0]}
            />
            <mesh>
              <sphereBufferGeometry attach="geometry" args={[3, 20, 20]} />
              <meshStandardMaterial
                attach="material"
                color="orange"
                roughness={0.6}
              />
            </mesh>
          </group>

           <group rotation={[0, 0 * ((2 * Math.PI) / 3), 0]}>
              <ArmAssembly
                shoulderAngle={arm_angles[0]}
                wristCentroid={[effector_position.x, effector_position.z + 190, effector_position.y]}
              />
            </group> 

            <group rotation={[0, 1 * ((2 * Math.PI) / 3), 0]}>
              <ArmAssembly
                shoulderAngle={arm_angles[1]}
                wristCentroid={[effector_position.x, effector_position.z + 190, effector_position.y]}
              />
            </group>

            <group rotation={[0, 2 * ((2 * Math.PI) / 3), 0]}>
              <ArmAssembly
                shoulderAngle={arm_angles[2]}
                wristCentroid={[effector_position.x, effector_position.z + 190, effector_position.y]}
              />
            </group>


          {/* Arms */}
          {/* {Array.from(new Array(arm_count)).map((_, arm_index) => (
            <group rotation={[0, arm_index * ((2 * Math.PI) / 3), 0]}>
              <ArmAssembly
                shoulderAngle={arm_angles[arm_index]}
                wristCentroid={[effector_position.x, effector_position.z + 190, effector_position.y]}
              />
            </group> 
          ))}*/}
        </group>
      </Environment>
    </div>
  )
}

export default RiggedModel
