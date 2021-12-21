import * as React from 'react'
import { useRef, useEffect } from 'react'

import {
  Environment,
  ControlledGroup,
  GLTF,
  OrbitControls,
} from '@electricui/components-desktop-three'
import { useHardwareState } from '@electricui/components-core'

import {
  Backdrop,
  ContactShadows,
  Stage,
  PresentationControls,
  Environment as DreiEnvironment,
} from '@react-three/drei'

import { useFrame, useThree } from '@react-three/fiber'
import { Mesh, Group } from 'three'

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

export const ToolpathVisualisation = () => {
  return (
    <div style={{ height: '100%', width: '100%' }}>
      <Environment
        camera={{
          fov: 80,
          position: [0, 150, 500],
          far: 4000,
        }}

        // shadows={true}
      >
        <OrbitControls />
        <AxisLines />

        <ambientLight intensity={0.2} />
        <directionalLight position={[-100, 0, -50]} intensity={1} color="red" />
        <directionalLight
          position={[-10, -20, -50]}
          intensity={0.3}
          color="#0c8cbf"
        />

        <spotLight
          position={[400, 20, 400]}
          intensity={2.5}
          penumbra={1}
          angle={0.3}
          castShadow
          color="#0c8cbf"
        />

        {/* <fog attach="fog" args={['#101010', 600, 3000]} /> */}
      </Environment>
    </div>
  )
}
