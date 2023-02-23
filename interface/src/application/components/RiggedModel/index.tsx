import * as React from 'react'
import { useRef, useEffect } from 'react'

import { GLTF } from '@electricui/components-desktop-three'

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
import { Euler, Group, Mesh, Vector3 } from 'three'

import { kinematics_point_to_angle, CartesianPoint } from './inverse_kinematics'

import DeltaBaseModel from './models/base.glb'
import DeltaBicepModel from './models/bicep.glb'
import DeltaEffectorModel from './models/effector.glb'
import DeltaForearmModel from './models/forearm.glb'
import {
  getSetting,
  setSetting,
  useSetting,
  useStore,
} from './../../pages/toolpaths/interface/state'
import {
  useDeviceID,
  useHardwareStateSubscription,
} from '@electricui/components-core'
import { MSGID } from 'src/application/typedState'

GLTF.preload(DeltaBaseModel)
GLTF.preload(DeltaBicepModel)
GLTF.preload(DeltaEffectorModel)
// GLTF.preload(DeltaForearmModel)

// softShadows({
//   frustum: 3.75, // Frustum width (default: 3.75) must be a float
//   size: 0.005, // World size (default: 0.005) must be a float
//   near: 9.5, // Near plane (default: 9.5) must be a float
//   samples: 17, // Samples (default: 17) must be a int
//   rings: 11, // Rings (default: 11) must be a int
// })

const bicep_length_mm: number = 180

function ForearmGeometry() {
  return (
    <group>
      {/* 
        GLTF isn't used as _each instance_ is high poly (19k) compared to most other parts 
        Instead, we generate sphere and cylinder geometry with native THREE meshes
      */}

      {/* <GLTF receiveShadow asset={DeltaForearmModel} position={[0, 0, -43]} rotation={[0, Math.PI, 0]} /> */}
      <mesh >
        <sphereBufferGeometry attach="geometry" args={[8, 16, 16]} />
        <meshStandardMaterial attach="material" roughness={0.6} />
      </mesh>
      <mesh position={[0, 0, 340]} >
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

const mutableEffector = new Vector3(0, 0, 0)
const mutableOffset = new Vector3(0, 0, 0)
const mutableEuler = new Euler(0, 0, 0)

function calculateLookAt(
  cpos: { x: number; y: number; z: number },
  left: boolean,
  armIndex: number,
) {
  // Delta -> ThreeJS transform
  mutableEffector.set(cpos.x, cpos.z, -cpos.y)

  mutableEuler.set(0, (armIndex * (2 * Math.PI)) / 3, 0)

  mutableOffset.set(left ? 22.5 : -22.5, 0, -32).applyEuler(mutableEuler)

  return mutableEffector.add(mutableOffset)
}

interface ArmAssemblyProps {
  armIndex: number
}

function ArmAssembly(props: ArmAssemblyProps) {
  const ref = useRef<Group>(null)
  const leftForearm = useRef<Group>(null)
  const rightForearm = useRef<Group>(null)

  useEffect(() => {
    if (ref?.current && leftForearm?.current && rightForearm?.current) {
      const initialCpos = getSetting(state => state.endEffector)

      let arm_angle = kinematics_point_to_angle(initialCpos, props.armIndex)
      ref.current.rotation.set(0, 0, (arm_angle * Math.PI) / 180)

      leftForearm.current.lookAt(
        calculateLookAt(initialCpos, true, props.armIndex),
      )

      rightForearm.current.lookAt(
        calculateLookAt(initialCpos, false, props.armIndex),
      )
    }

    return useStore.subscribe(
      state => state.endEffector,
      cpos => {
        if (ref?.current && leftForearm?.current && rightForearm?.current) {
          let arm_angle = kinematics_point_to_angle(cpos, props.armIndex)
          ref.current.rotation.set(0, 0, (arm_angle * Math.PI) / 180)

          leftForearm.current.lookAt(
            calculateLookAt(cpos, true, props.armIndex),
          )

          rightForearm.current.lookAt(
            calculateLookAt(cpos, false, props.armIndex),
          )
        }
      },
    )
  }, [props.armIndex])

  return (
    <group position={[-50, 0, 3]} ref={ref}>
      <GLTF
        asset={DeltaBicepModel}
        position={[0, 0, -0.5]}
        rotation={[Math.PI / 2, (-27.75 * Math.PI) / 180.0, Math.PI]}
      />

      {/* TODO: Find actual 'effector' mounting location offset */}
      <group ref={leftForearm} position={[-bicep_length_mm, 0, -25.5]}>
        <ForearmGeometry />
      </group>
      <group ref={rightForearm} position={[-bicep_length_mm, 0, +22.5]}>
        <ForearmGeometry />
      </group>
    </group>
  )
}

const LIGHT_OFFSET = -10

function EndEffector() {
  const ref = useRef<Group>(null)

  useEffect(() => {
    if (ref && ref.current) {
      const initialCpos = getSetting(state => state.endEffector)

      ref.current.position.set(
        initialCpos.x,
        initialCpos.z + LIGHT_OFFSET, // Delta -> ThreeJS transform
        -initialCpos.y,
      )
    }

    return useStore.subscribe(
      state => state.endEffector,
      cpos => {
        if (ref && ref.current) {
          ref.current.position.set(cpos.x, cpos.z + LIGHT_OFFSET, -cpos.y) // Delta -> ThreeJS transform
        }
      },
    )
  }, [])

  return (
    <>
      <group ref={ref} rotation={[0, Math.PI / -2, 0]}>
        <GLTF
          receiveShadow
          asset={DeltaEffectorModel}
          position={[-43.25, -3, -0.25]}
          rotation={[0, Math.PI / -2, 0]}
        />
        
      </group>
    </>
  )
}

function TargetPositionVisualiser() {

  const ref = useRef<Group>(null)

  useEffect(() => {
    if (ref && ref.current) {
      const initialTpos = getSetting(state => state.targetPosition)

      ref.current.position.set(
        initialTpos.x,
        initialTpos.z + LIGHT_OFFSET, // Delta -> ThreeJS transform
        -initialTpos.y,
      )
    }

    return useStore.subscribe(
      state => state.targetPosition,
      tpos => {
        if (ref && ref.current) {
          ref.current.position.set(tpos.x, tpos.z + LIGHT_OFFSET, -tpos.y) // Delta -> ThreeJS transform
        }
      },
    )
  }, [])

  return (
    <group ref={ref} >
      <mesh>
        <sphereBufferGeometry attach="geometry" args={[3, 20, 20]} />
        <meshStandardMaterial
          attach="material"
          color="orange"
          roughness={0.6}
        />
      </mesh>
    </group>
  )
}

function HardwareConnector() {
  useHardwareStateSubscription(
    state => state[MSGID.POSITION_CURRENT],
    cpos => {
      // On hardware position updates, change the visualiser's position if preview is disabled
      if (
        cpos &&
        !getSetting(state => state.visualisationSettings.previewProgress)
      ) {
        setSetting(state => {
          state.endEffector.x = cpos.x
          state.endEffector.y = cpos.y
          state.endEffector.z = cpos.z
        })
      }
    },
  )

  useHardwareStateSubscription(
    state => state[MSGID.POSITION_TARGET],
    tpos => {
      // On hardware position updates
      if (
        tpos &&
        !getSetting(state => state.visualisationSettings.previewProgress)
      ) {
        setSetting(state => {
          state.targetPosition.x = tpos.x
          state.targetPosition.y = tpos.y
          state.targetPosition.z = tpos.z
        })
      }
    },
  )

  useHardwareStateSubscription(
    state => state[MSGID.SUPERVISOR].mode,
    mode => {
      // On hardware position updates
      if (
        mode &&
        !getSetting(state => state.visualisationSettings.previewProgress)
      ) {
        setSetting(state => {
          state.hardwareMode = mode
        })
      }
    },
  )

  return null
}

export function DeltaAssembly() {
  const deviceID = useDeviceID()

  const ref = useRef<Group>(null)

  useEffect(() => {
    if( ref?.current ) {
      ref.current.traverse(mesh => {
        mesh.castShadow = true
        mesh.receiveShadow = true
        // console.log("Assuming this thing is a mesh...")
      })
    }
  }, [])
    
  return (
    <>
      {/* End effector */}
      <EndEffector />

      <TargetPositionVisualiser/>

      <group position={[0, -190, 0]} ref={ref}>
        {/* Base positioned such that threeJS '[0,0,0' is aligned line with the servo shaft center */}
        <group rotation={[0, (-90 * Math.PI) / 180, 0]}>
          <GLTF
            asset={DeltaBaseModel}
            position={[2.5, -135, -5.5]}
          />
        </group>

        {/* Arm assemblies */}
        <group rotation={[0, Math.PI / -2, 0]}>
          {[0, 1, 2].map(armIndex => (
            <group
              rotation={[0, armIndex * ((2 * Math.PI) / 3), 0]}
              key={armIndex}
            >
              <ArmAssembly armIndex={armIndex} />
            </group>
          ))}
        </group>
      </group>
      {deviceID ? <HardwareConnector /> : null}
    </>
  )
}
