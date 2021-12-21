import * as React from 'react'

import {
  Environment,
  OrbitControls,
} from '@electricui/components-desktop-three'

import { Line, CubicBezierLine } from '@react-three/drei'

import { useFrame, useThree } from '@react-three/fiber'
import { Mesh, Group } from 'three'
import { useViewportFrameToolpath } from './state'
import { MovementMoveType } from '../optimiser/hardware'
import { Toolpath } from '../optimiser/passes'

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
  const toolpath: Toolpath = useViewportFrameToolpath() ?? {
    movementMoves: [],
    lightMoves: [],
  }

  const toolpathThree = toolpath.movementMoves.map(movement => {
    switch (movement.type) {
      case MovementMoveType.LINE:
        return (
          <Line
            key={movement.id}
            points={movement.points} // Array of points
            color="white" // Default
            lineWidth={2} // In pixels (default)
            dashed={false} // Default
          />
        )
      case MovementMoveType.BEZIER_CUBIC:
        return (
          <CubicBezierLine
            key={movement.id}
            start={movement.points[0]} // Starting point
            midA={movement.points[1]} // First control point
            midB={movement.points[2]} // Second control point
            end={movement.points[3]} // Ending point
            color="blue" // Default
            lineWidth={1} // In pixels (default)
            dashed={true} // Default
          />
        )

      default:
        return null
    }
  })

  return (
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

      {toolpathThree}

      {/* <fog attach="fog" args={['#101010', 600, 3000]} /> */}
    </Environment>
  )
}
