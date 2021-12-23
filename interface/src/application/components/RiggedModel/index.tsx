import * as React from 'react'
import { useRef, useEffect } from 'react'

import {
  Environment,
  ControlledGroup,
  GLTF,
  OrbitControls,
} from '@electricui/components-desktop-three'
import { useHardwareState, useHardwareStateSubscription } from '@electricui/components-core'

import {
  Backdrop,
  ContactShadows,
  Shadow,
  softShadows,
  Stage,
  PresentationControls,
  Environment as DreiEnvironment,
} from '@react-three/drei'

import { useFrame, useThree } from '@react-three/fiber'
import { Euler, Group, Vector3 } from 'three'

import { kinematics_point_to_angle, CartesianPoint } from './inverse_kinematics'

import DeltaBaseModel from './models/base.glb'
import DeltaBicepModel from './models/bicep.glb'
import DeltaEffectorModel from './models/effector.glb'
import DeltaForearmModel from './models/forearm.glb'

GLTF.preload(DeltaBaseModel)
GLTF.preload(DeltaBicepModel)
GLTF.preload(DeltaEffectorModel)
// GLTF.preload(DeltaForearmModel)

softShadows({
  frustum: 3.75, // Frustum width (default: 3.75) must be a float
  size: 0.005, // World size (default: 0.005) must be a float
  near: 9.5, // Near plane (default: 9.5) must be a float
  samples: 17, // Samples (default: 17) must be a int
  rings: 11, // Rings (default: 11) must be a int
})

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

  // const camera = useThree(state => state.camera)

  // useEffect(() => {
  //   camera.lookAt(0, 50, 0)
  // }, [])

  return (
    <></>
    //   <mesh>
    //     <sphereBufferGeometry attach="geometry" args={[7, 20, 20]} />
    //     <meshStandardMaterial attach="material" color="magenta" roughness={0.6} />
    //   </mesh>
  )
}


function ForearmGeometry() {
  return (
    <group>
      {/* 
        GLTF isn't used as _each instance_ is high poly (19k) compared to most other parts 
        Instead, we generate sphere and cylinder geometry with native THREE meshes
      */}

      {/* <GLTF receiveShadow asset={DeltaForearmModel} position={[0, 0, -43]} rotation={[0, Math.PI, 0]} /> */}
      <mesh>
        <sphereBufferGeometry attach="geometry" args={[8, 16, 16]} />
        <meshStandardMaterial attach="material" roughness={0.6} />
      </mesh>
      <mesh position={[0, 0, 340]}>
        <sphereBufferGeometry attach="geometry" args={[8, 16, 16]} />
        <meshStandardMaterial attach="material" roughness={0.6} />
      </mesh>
      <mesh position={[0, 0, 340 / 2]} rotation={[Math.PI / 2, 0, 0]}>
        <cylinderBufferGeometry attach="geometry" args={[4, 4, 340, 32]} />
        <meshStandardMaterial attach="material" roughness={0.6} />
      </mesh>
    </group>
  )
}

interface ForearmHelperProps {
  left: boolean
  armIndex: number
}

const mutableEffector = new Vector3(0, 0, 0)
const mutableOffset = new Vector3(0, 0, 0)
const mutableEuler = new Euler(0, 0, 0)

function calculateLookAt(cpos: { x: number; y: number; z: number }, left: boolean, armIndex: number) {
  mutableEffector.set(cpos.x, cpos.z + 190 + 140, cpos.y)

  mutableEuler.set(0, (armIndex * (-2 * Math.PI)) / 3, 0)

  mutableOffset.set(left ? 22.5 : -22.5, 0, -32).applyEuler(mutableEuler)

  return mutableEffector.add(mutableOffset)
}

function ForearmHelper(props: ForearmHelperProps) {
  const ref = useRef<Group>(null)

  const initialCpos = useHardwareStateSubscription(
    state => state.cpos,
    cpos => {
      if (ref && ref.current) {
        ref.current.lookAt(calculateLookAt(cpos, props.left, props.armIndex))
      }
    },
  )

  useEffect(() => {
    if (ref && ref.current) {
      ref.current.lookAt(calculateLookAt(initialCpos, props.left, props.armIndex))
    }
  }, [])

  return (
    <group ref={ref} position={props.left ? [-bicep_length_mm, 0, -25.5] : [-bicep_length_mm, 0, +22.5]}>
      <ForearmGeometry />
    </group>
  )
}

interface ArmAssemblyProps {
  armIndex: number
}

function ArmAssembly(props: ArmAssemblyProps) {
  return (
    <ControlledGroup
      position={[-50, 0, 3]}
      rotationAccessor={state => {
        let arm_angle = kinematics_point_to_angle(state.cpos, props.armIndex)
        return [0, 0, (arm_angle * Math.PI) / 180]
      }}
    >
      <GLTF
        asset={DeltaBicepModel}
        position={[0, 0, -0.5]}
        rotation={[Math.PI / 2, (-27.75 * Math.PI) / 180.0, Math.PI]}
      />

      {/* TODO: Find actual 'effector' mounting location offset */}
      <ForearmHelper armIndex={props.armIndex} left={true} />
      <ForearmHelper armIndex={props.armIndex} left={false} />
    </ControlledGroup>
  )
}


export const RiggedModel = () => {
  return (
    <div style={{ height: '100%', width: '100%' }}>
      <Environment
        camera={{
          fov: 80,
          position: [0, 150, 500],
          far: 4000,
        }}
        shadows

        // shadows={true}
      >
        <OrbitControls />
        <CameraViewTarget />
        {/* <AxisLines /> */}

        <ambientLight intensity={0.2} />
        <directionalLight position={[-100, 0, -50]} intensity={1} color="red" castShadow />
        <directionalLight position={[-10, -20, -50]} intensity={0.3} color="#0c8cbf" castShadow />

        <spotLight
          position={[400, 20, 400]}
          intensity={2.5}
          penumbra={1}
          angle={0.3}
          color="#0c8cbf"
          castShadow
          shadow-camera-far={8000}
          shadow-camera-near={0}
        />

        {/* <fog attach="fog" args={['#101010', 600, 3000]} /> */}

        <Backdrop floor={2} position={[0, 0, -500]} scale={[6000, 2000, 800]} receiveShadow>
          <meshStandardMaterial color="#353540" envMapIntensity={0.3} />
        </Backdrop>

        <Shadow
          color="black"
          colorStop={0}
          opacity={0.5}
          fog={false} // Reacts to fog (default=false)
        />

        <group position={[0, 140, 0]} castShadow receiveShadow>
          {/* Base positioned such that threeJS '[0,0,0' is aligned line with the servo shaft center */}
          <group rotation={[0, (-90 * Math.PI) / 180, 0]}>
            <GLTF receiveShadow asset={DeltaBaseModel} position={[2.5, -135, -5.5]} />
          </group>
          {/* End Effector */}
          <ControlledGroup
            positionAccessor={state => [state.cpos.x, state.cpos.z + 190, state.cpos.y]}
            rotation={[0, Math.PI / -2, 0]}
          >
            <GLTF
              receiveShadow
              asset={DeltaEffectorModel}
              position={[-43.25, -3, -0.25]}
              rotation={[0, Math.PI / -2, 0]}
            />
            <mesh>
              <sphereBufferGeometry attach="geometry" args={[3, 20, 20]} />
              <meshStandardMaterial attach="material" color="orange" roughness={0.6} />
            </mesh>
          </ControlledGroup>

          <group rotation={[0, Math.PI / -2, 0]}>
            {/* Arms */}
            {[0, 1, 2].map(armIndex => (
              <group rotation={[0, armIndex * ((-2 * Math.PI) / 3), 0]} key={armIndex}>
                <ArmAssembly armIndex={armIndex} />
              </group>
            ))}
          </group>
        </group>
      </Environment>
    </div>
  )
}

export default RiggedModel
