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
  PerspectiveCamera,
} from '@react-three/drei'

import { OrbitControls as OrbitControlsImpl } from 'three-stdlib'

import { useFrame, useThree } from '@react-three/fiber'
import {
  Mesh,
  Group,
  Color,
  Vector2,
  PerspectiveCamera as PerspectiveCameraImpl,
} from 'three'
import { getSetting, setSetting, useSetting, useStore } from './state'
import { MovementMoveType } from '../optimiser/hardware'
import { sparseToDense } from '../optimiser/passes'
import { CatmullRomLine } from './CatmullLine'
import { MovementPoint } from 'src/application/typedState'
import { Vector3 } from 'three'
import {
  GLOBAL_OVERRIDE_OBJECT_ID,
  Movement,
  RGB,
  XYZ,
} from '../optimiser/movements'
import { Line2 } from 'three/examples/jsm/lines/Line2'
import { LineMaterial } from 'three/examples/jsm/lines/LineMaterial'
import { LineSegmentsGeometry } from 'three/examples/jsm/lines/LineSegmentsGeometry'
import { MutableLineSegmentGeometry } from './ExternallyStoredLineSegments'
import { importMaterial } from '../optimiser/material'
import { Material } from '../optimiser/materials/Base'

function AxisLines() {
  return (
    <group position={[0, 0, 0]} scale={[1, 1, 1]}>
      <mesh>
        <boxBufferGeometry attach="geometry" args={[1, 100, 1]} />
        <meshStandardMaterial attach="material" color="blue" roughness={0.6} />
      </mesh>
      <mesh>
        <boxBufferGeometry attach="geometry" args={[100, 1, 1]} />
        <meshStandardMaterial attach="material" color="red" roughness={0.6} />
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
export function ToolpathMovements() {
  const [lines] = useState(() => new MutableLineSegmentGeometry(false, 2.0))
  const [transitions] = useState(
    () => new MutableLineSegmentGeometry(true, 1.0),
  )

  const [customComponents, setComponents] = useState<React.ReactNode[]>([])

  const colouredLineToObjectID: React.MutableRefObject<{
    [objectID: string]: number[]
  }> = useRef({})
  const invisibleLineToObjectID: React.MutableRefObject<{
    [objectID: string]: number[]
  }> = useRef({})

  useEffect(() => {
    let lineIndex = 0

    const addColouredLine = (
      start: Vector3,
      end: Vector3,
      colorStart: RGB,
      colorEnd: RGB,
      objectID: string,
    ) => {
      // Do the Blender -> ThreeJS coordinate system transform inline
      lines.positions[lineIndex * 6 + 0] = start.x
      lines.positions[lineIndex * 6 + 1] = start.z
      lines.positions[lineIndex * 6 + 2] = -start.y
      lines.positions[lineIndex * 6 + 3] = end.x
      lines.positions[lineIndex * 6 + 4] = end.z
      lines.positions[lineIndex * 6 + 5] = -end.y

      lines.colors[lineIndex * 6 + 0] = colorStart[0]
      lines.colors[lineIndex * 6 + 1] = colorStart[1]
      lines.colors[lineIndex * 6 + 2] = colorStart[2]
      lines.colors[lineIndex * 6 + 3] = colorEnd[0]
      lines.colors[lineIndex * 6 + 4] = colorEnd[1]
      lines.colors[lineIndex * 6 + 5] = colorEnd[2]

      // Create the mapping for objectID -> coloured line index
      if (objectID) {
        if (!colouredLineToObjectID.current[objectID]) {
          colouredLineToObjectID.current[objectID] = []
        }
        colouredLineToObjectID.current[objectID].push(lineIndex)
      }

      // console.log(`${lineCounter} [${start.x},${start.y}${start.z}]->[${end.x},${end.y}${end.z}]`)

      lineIndex += 1
    }

    let transitionIndex = 0

    const addDottedLine = (
      start: Vector3,
      end: Vector3,
      colorStart: RGB,
      colorEnd: RGB,
      objectID: string,
    ) => {
      // Do the Blender -> ThreeJS coordinate system transform inline
      transitions.positions[transitionIndex * 6 + 0] = start.x
      transitions.positions[transitionIndex * 6 + 1] = start.z
      transitions.positions[transitionIndex * 6 + 2] = -start.y
      transitions.positions[transitionIndex * 6 + 3] = end.x
      transitions.positions[transitionIndex * 6 + 4] = end.z
      transitions.positions[transitionIndex * 6 + 5] = -end.y

      transitions.colors[transitionIndex * 6 + 0] = colorStart[0]
      transitions.colors[transitionIndex * 6 + 1] = colorStart[1]
      transitions.colors[transitionIndex * 6 + 2] = colorStart[2]
      transitions.colors[transitionIndex * 6 + 3] = colorEnd[0]
      transitions.colors[transitionIndex * 6 + 4] = colorEnd[1]
      transitions.colors[transitionIndex * 6 + 5] = colorEnd[2]

      // Create the mapping for objectID -> coloured line index
      if (objectID) {
        if (!invisibleLineToObjectID.current[objectID]) {
          invisibleLineToObjectID.current[objectID] = []
        }
        invisibleLineToObjectID.current[objectID].push(transitionIndex)
      }

      transitionIndex += 1
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
        lineIndex = 0
        transitionIndex = 0

        // Refresh the line mapping
        colouredLineToObjectID.current = {}
        invisibleLineToObjectID.current = {}

        // Refresh the react components
        setComponents(state => [])

        lines.positions = []
        transitions.positions = []
        lines.colors = []
        transitions.colors = []

        const orderedMovements = reorderMovementsForFrame(
          getSetting(state => state.viewportFrame),
        )
        const settings = getSetting(state => state.settings)
        const visualisationSettings = getSetting(
          state => state.visualisationSettings,
        )
        const denseMovements = sparseToDense(orderedMovements, settings)

        // Import the global material override if it exists
        const globalMaterialOverride = visualisationSettings
          .objectMaterialOverrides[GLOBAL_OVERRIDE_OBJECT_ID]
          ? importMaterial(
              visualisationSettings.objectMaterialOverrides[
                GLOBAL_OVERRIDE_OBJECT_ID
              ],
            )
          : null

        for (let index = 0; index < denseMovements.length; index++) {
          const movement = denseMovements[index]

          let material: Material = movement.material

          const movementMaterialOverride =
            visualisationSettings.objectMaterialOverrides[movement.objectID]

          // Global overrides take least precidence
          if (globalMaterialOverride) {
            material = globalMaterialOverride
          }

          // Specific movement overrides take highest precidence
          if (movementMaterialOverride) {
            material = importMaterial(movementMaterialOverride)
          }

          // Generate using the
          material.generateThreeJSRepresentation(
            index,
            movement,
            settings,
            visualisationSettings,
            addColouredLine,
            addDottedLine,
            addReactComponent,
          )
        }

        lines.refreshGeometry(lineIndex)
        transitions.refreshGeometry(transitionIndex)

        // console.log(`built ${lineCounter} lines`, lines.line.visible)
      },
    )

    return () => {
      unsubscribe()

      lines?.dispose()
      transitions?.dispose()
    }
  }, [lines, transitions])

  // Transition lines should subtly move over time to indicate their direction
  useFrame((_, delta) => {
    // Negative offset moves the line in the direction of movements.
    transitions.material.uniforms.dashOffset.value -= delta * 2
    transitions.material.uniformsNeedUpdate = true
    // transitions.material.needsUpdate = true
  })

  // On hovering change, update the lines
  useEffect(() => {
    return useStore.subscribe(
      state => state.treeStore.hoveredObjectIDs,
      hoveredObjectIDs => {
        if (hoveredObjectIDs.length === 0) {
          // If nothing is hovered, reset both
          lines.setHoveredIndices([])
          transitions.setHoveredIndices([])
          return
        }

        // // If we're hovering over transition
        // if (hoveredObjectIDs.includes('transition')) {
        //   // Display all transitions
        //   transitions.setHoveredIndices([])
        // } else {
        //   // Hide all transitions
        //   transitions.setHoveredIndices([], true)
        // }

        // Use the mapping to calculate the line indices
        const colouredLineIndices: number[] = []
        const invisibleLineIndices: number[] = []

        for (const objectID of hoveredObjectIDs) {
          const colouredIndices = colouredLineToObjectID.current[objectID]

          if (colouredIndices) {
            for (let i = 0; i < colouredIndices.length; i++) {
              const lineIndex = colouredIndices[i]
              colouredLineIndices.push(lineIndex)
            }
          }

          const invisibleIndices = invisibleLineToObjectID.current[objectID]

          if (invisibleIndices) {
            for (let i = 0; i < invisibleIndices.length; i++) {
              const lineIndex = invisibleIndices[i]
              invisibleLineIndices.push(lineIndex)
            }
          }
        }

        // If there are hovered objects, but no indices can be found, hide everything.
        lines.setHoveredIndices(
          colouredLineIndices,
          colouredLineIndices.length === 0,
        )
        transitions.setHoveredIndices(
          invisibleLineIndices,
          invisibleLineIndices.length === 0,
        )
      },
    )
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
  const setCameraRef = useCallback((camera: PerspectiveCameraImpl) => {
    if (camera) {
      setSetting(state => {
        state.camera = camera
      })
    }
  }, [])

  const setOrbitControlsRef = useCallback(
    (orbitControls: OrbitControlsImpl) => {
      if (orbitControls) {
        setSetting(state => {
          state.orbitControls = orbitControls as any
        })

        const cam = getSetting(state => state.camera)
        // Set the camera
        if (cam) {
          orbitControls.object = cam
        }
      }
    },
    [],
  )

  return (
    <Canvas
      linear
      // shadows={true}
    >
      <PerspectiveCamera
        ref={setCameraRef}
        makeDefault
        position={[0, 150, 400]}
      />
      <OrbitControls ref={setOrbitControlsRef} />
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

      <ToolpathMovements />

      {/* <fog attach="fog" args={['#101010', 600, 3000]} /> */}
    </Canvas>
  )
}
