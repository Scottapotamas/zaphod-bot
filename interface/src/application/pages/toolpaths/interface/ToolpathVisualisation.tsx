import * as React from 'react'
import { useRef } from 'react'

// import // Environment,
// // OrbitControls,
// '@electricui/components-desktop-three'

import { Canvas } from '@react-three/fiber'

import {
  Line,
  CubicBezierLine,
  Segments,
  Segment,
  OrbitControls,
} from '@react-three/drei'

import { useFrame, useThree } from '@react-three/fiber'
import { Mesh, Group } from 'three'
import { useViewportFrameToolpath } from './state'
import { MovementMoveType } from '../optimiser/hardware'
import { Toolpath } from '../optimiser/passes'
import { CatmullRomLine } from './CatmullLine'
import { MovementPoint } from 'src/application/typedState'
import { Line2 } from 'three-stdlib'

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

function convertToThreeCoordinateSystem(
  vector: [number, number, number],
): [number, number, number] {
  return [vector[0], vector[2], -vector[1]]
}

function ToolpathObjects() {
  const toolpath: Toolpath = useViewportFrameToolpath() ?? {
    movementMoves: [],
    lightMoves: [],
  }

  const segments = toolpath.movementMoves
    .filter(movement => movement.type === MovementMoveType.LINE)
    .map(movement => (
      <Segment
        start={convertToThreeCoordinateSystem(movement.points[0]) as any}
        end={convertToThreeCoordinateSystem(movement.points[1]) as any}
        color={'white' as any}
      />
    ))

  const transitions = toolpath.movementMoves
    .filter(movement => movement.type !== MovementMoveType.LINE)
    .map(movement => {
      switch (movement.type) {
        case MovementMoveType.BEZIER_CUBIC:
          return (
            <CubicBezierLine
              key={movement.id}
              start={convertToThreeCoordinateSystem(movement.points[0])} // Starting point
              midA={convertToThreeCoordinateSystem(movement.points[1])} // First control point
              midB={convertToThreeCoordinateSystem(movement.points[2])} // Second control point
              end={convertToThreeCoordinateSystem(movement.points[3])} // Ending point
              color="blue"
              lineWidth={1}
              dashed={true}
            />
          )
        case MovementMoveType.CATMULL_SPLINE:
          return (
            <CatmullRomLine
              key={movement.id}
              points={movement.points.map(point =>
                convertToThreeCoordinateSystem(point),
              )}
              curveType="catmullrom"
              color="blue"
              lineWidth={1}
              dashed={true}
            />
          )

        default:
          return null
      }
    })

  return (
    <>
      <Segments limit={2000} lineWidth={2.0}>
        {segments}
      </Segments>

      {transitions}
    </>
  )
}

export const ToolpathVisualisation = () => {
  return (
    <Canvas
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

      <ToolpathObjects />

      {/* <fog attach="fog" args={['#101010', 600, 3000]} /> */}
    </Canvas>
  )
}
