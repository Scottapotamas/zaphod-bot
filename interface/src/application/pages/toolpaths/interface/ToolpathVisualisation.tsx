import * as React from 'react'
import {
  useRef,
  useMemo,
  useState,
  forwardRef,
  useCallback,
  useEffect,
} from 'react'

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
  LineProps,
} from '@react-three/drei'

import { useFrame, useThree } from '@react-three/fiber'
import { Mesh, Group, Color, Vector2 } from 'three'
import {
  getSetting,
  useSetting,
  useStore,
  useViewportFrameToolpath,
} from './state'
import { MovementMoveType } from '../optimiser/hardware'
import { sparseToDense, Toolpath } from '../optimiser/passes'
import { CatmullRomLine } from './CatmullLine'
import { MovementPoint } from 'src/application/typedState'
import { Vector3 } from 'three'
import { Movement, RGBA, XYZ } from '../optimiser/movements'
import { Line2 } from 'three/examples/jsm/lines/Line2'
import { LineMaterial } from 'three/examples/jsm/lines/LineMaterial'
import { LineSegmentsGeometry } from 'three/examples/jsm/lines/LineSegmentsGeometry'
import { MutableLineSegmentGeometry } from './ExternallyStoredLineSegments'

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

function reorderMovementsForFrame(frameNumber: number): Movement[] {
  const orderedMovementsByFrame = getSetting(
    state => state.orderedMovementsByFrame,
  )
  const movementOrdering = getSetting(state => state.movementOrdering)

  const movements = orderedMovementsByFrame[frameNumber] ?? []
  const ordering = movementOrdering[frameNumber] ?? {}
  const movementsCopy = movements.slice()

  movementsCopy.sort((a, b) => {
    const aOrder = ordering[a.interFrameID] ?? 0
    const bOrder = ordering[b.interFrameID] ?? 0

    // Sort in ascending order
    return aOrder - bOrder
  })

  return movementsCopy
}

/**
 * This component mounts once, then does all other state management outside of React
 */
function Movements() {
  const [lines] = useState(() => new MutableLineSegmentGeometry(false, 2.0))
  const [transitions] = useState(
    () => new MutableLineSegmentGeometry(true, 1.0),
  )

  const [customComponents, setComponents] = useState<React.ReactNode[]>([])

  useEffect(() => {
    let lineCounter = 0

    const addColouredLine = (
      start: Vector3,
      end: Vector3,
      colorStart: RGBA,
      colorEnd: RGBA,
    ) => {
      // Do the Blender -> ThreeJS coordinate system transform inline
      lines.positions[lineCounter * 6 + 0] = start.x
      lines.positions[lineCounter * 6 + 1] = start.z
      lines.positions[lineCounter * 6 + 2] = -start.y
      lines.positions[lineCounter * 6 + 3] = end.x
      lines.positions[lineCounter * 6 + 4] = end.z
      lines.positions[lineCounter * 6 + 5] = -end.y

      lines.colors[lineCounter * 6 + 0] = colorStart[0]
      lines.colors[lineCounter * 6 + 1] = colorStart[1]
      lines.colors[lineCounter * 6 + 2] = colorStart[2]
      lines.colors[lineCounter * 6 + 3] = colorEnd[0]
      lines.colors[lineCounter * 6 + 4] = colorEnd[1]
      lines.colors[lineCounter * 6 + 5] = colorEnd[2]

      lineCounter += 1
    }

    let transitionCounter = 0

    const addTransitionLine = (
      start: Vector3,
      end: Vector3,
      colorStart: RGBA,
      colorEnd: RGBA,
    ) => {
      // Do the Blender -> ThreeJS coordinate system transform inline
      transitions.positions[transitionCounter * 6 + 0] = start.x
      transitions.positions[transitionCounter * 6 + 1] = start.z
      transitions.positions[transitionCounter * 6 + 2] = -start.y
      transitions.positions[transitionCounter * 6 + 3] = end.x
      transitions.positions[transitionCounter * 6 + 4] = end.z
      transitions.positions[transitionCounter * 6 + 5] = -end.y

      transitions.colors[transitionCounter * 6 + 0] = colorStart[0]
      transitions.colors[transitionCounter * 6 + 1] = colorStart[1]
      transitions.colors[transitionCounter * 6 + 2] = colorStart[2]
      transitions.colors[transitionCounter * 6 + 3] = colorEnd[0]
      transitions.colors[transitionCounter * 6 + 4] = colorEnd[1]
      transitions.colors[transitionCounter * 6 + 5] = colorEnd[2]

      transitionCounter += 1
    }

    // This is expensive, if we can avoid doing this, do so.
    const addReactComponent = (component: React.ReactNode) => {
      setComponents(state => [...state, component])
    }

    const unsubscribe = useStore.subscribe(
      state => state.viewportFrameVersion,
      () => {
        // On viewport frame change, or toolpath update, regenerate the ordering,
        // and update the lines

        // Refresh the line geometries
        lineCounter = 0
        transitionCounter = 0

        // Refresh the react components
        setComponents(state => [])

        lines.positions.length = 0
        transitions.positions.length = 0

        const orderedMovements = reorderMovementsForFrame(
          useStore.getState().viewportFrame,
        )
        const settings = useStore.getState().settings
        const denseMovements = sparseToDense(orderedMovements, settings)

        for (let index = 0; index < denseMovements.length; index++) {
          const movement = denseMovements[index]

          movement.generateThreeLineSegments(
            addColouredLine,
            addTransitionLine,
            addReactComponent,
          )
        }

        lines.refreshGeometry()
        transitions.refreshGeometry()
      },
    )

    return () => {
      unsubscribe()

      console.log(
        lines.positions,
        transitions.positions,
        lineCounter,
        transitionCounter,
      )

      lines?.dispose()
      transitions?.dispose()
    }
  }, [lines, transitions])

  return (
    <>
      {lines.getReactComponent()}
      {transitions.getReactComponent()}
      {customComponents}
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

      <Movements />

      {/* <fog attach="fog" args={['#101010', 600, 3000]} /> */}
    </Canvas>
  )
}
