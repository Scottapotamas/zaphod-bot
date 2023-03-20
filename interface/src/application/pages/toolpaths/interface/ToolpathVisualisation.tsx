import * as React from 'react'
import { useRef, useMemo, useState, forwardRef, useCallback, useEffect } from 'react'

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
  MathUtils,
} from 'three'
import {
  changeState,
  getSetting,
  setSetting,
  useSetting,
  useStore,
} from './state'
import { MovementMoveType } from '../optimiser/hardware'
import { sparseToDense } from '../optimiser/passes'
import { CatmullRomLine } from './CatmullLine'
import { MovementPoint } from 'src/application/typedState'
import { Vector3, PCFSoftShadowMap } from 'three'
import {
  deserialiseTour,
  GLOBAL_OVERRIDE_OBJECT_ID,
  Movement,
  RGB,
  RGBA,
  XYZ,
} from '../optimiser/movements'
import { Line2 } from 'three/examples/jsm/lines/Line2'
import { LineMaterial } from 'three/examples/jsm/lines/LineMaterial'
import { LineSegmentsGeometry } from 'three/examples/jsm/lines/LineSegmentsGeometry'
import { MutableLineSegmentGeometry } from './ExternallyStoredLineSegments'
import { getMaterialOverride, importMaterial } from '../optimiser/material'
import { Material } from '../optimiser/materials/Base'
import { DeltaAssembly } from './../../../components/RiggedModel'
import { isCamera } from '../optimiser/camera'
import { useDarkMode } from '@electricui/components-desktop'
import { lerpRGBA } from '../optimiser/materials/utilities'
import { GroundPlane } from 'src/application/components/GroundPlane'

export function AxisLines() {
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

function convertToThreeCoordinateSystem(vector: [number, number, number]): [number, number, number] {
  return [vector[0], vector[2], -vector[1]]
}

export function getOrderedMovementsForFrame(frameNumber: number): Movement[] {
  const unorderedMovementsByFrame = getSetting(
    state => state.unorderedMovementsByFrame,
  )
  const movementOrdering = getSetting(state => state.movementOrdering)

  const movements = unorderedMovementsByFrame[frameNumber] ?? []
  const ordering = movementOrdering[frameNumber] ?? {}

  return deserialiseTour(movements.slice(), ordering)
}

/**
 * This component mounts once, then does all other state management outside of React
 */
export function ToolpathMovements() {
  const [lines] = useState(() => new MutableLineSegmentGeometry(false, 2.0))
  const [transitions] = useState(() => new MutableLineSegmentGeometry(true, 1.0))

  const [customComponents, setComponents] = useState<React.ReactNode[]>([])

  const movementIndexToColouredLine: React.MutableRefObject<Map<string | number, number[]>
  > = useRef(new Map())
  const movementIndexToDottedLine: React.MutableRefObject<Map<string | number, number[]>
  > = useRef(new Map())
  const objectIDToColouredLine: React.MutableRefObject<Map<string | number, number[]>
  > = useRef(new Map())
  const objectIDToDottedLine: React.MutableRefObject<Map<string | number, number[]>
  > = useRef(new Map())

  const backgroundCol = useDarkMode() ? '#191b1d' : '#f5f8fa'
  const backgroundColor = new Color(backgroundCol)
  const backgroundColorAsRGBA: RGBA = [backgroundColor.r, backgroundColor.g, backgroundColor.b, 1]

  useEffect(() => {
    let lineIndex = 0

    const addColouredLine = (start: Vector3, end: Vector3, colorStart: RGBA, colorEnd: RGBA, movementIndex: number, objectID?: string ) => {
      // Do the Blender -> ThreeJS coordinate system transform inline
      lines.positions[lineIndex * 6 + 0] = start.x
      lines.positions[lineIndex * 6 + 1] = start.z
      lines.positions[lineIndex * 6 + 2] = -start.y
      lines.positions[lineIndex * 6 + 3] = end.x
      lines.positions[lineIndex * 6 + 4] = end.z
      lines.positions[lineIndex * 6 + 5] = -end.y

      // Move the colours toward the background colour based on their alpha
      const colStartBlended = lerpRGBA(
        [
          colorStart[0],
          colorStart[1],
          colorStart[2],
          colorStart[3], //
        ],
        backgroundColorAsRGBA,
        1 - colorStart[3], // merge with the background by (1 - alpha)
      )
      const colEndBlended = lerpRGBA(
        [
          colorEnd[0],
          colorEnd[1],
          colorEnd[2],
          colorEnd[3], //
        ],
        backgroundColorAsRGBA,
        1 - colorEnd[3], // merge with the background by (1 - alpha)
      )

      lines.colors[lineIndex * 6 + 0] = colStartBlended[0]
      lines.colors[lineIndex * 6 + 1] = colStartBlended[1]
      lines.colors[lineIndex * 6 + 2] = colStartBlended[2]
      lines.colors[lineIndex * 6 + 3] = colEndBlended[0]
      lines.colors[lineIndex * 6 + 4] = colEndBlended[1]
      lines.colors[lineIndex * 6 + 5] = colEndBlended[2]

      // Create the mapping for movementIndex -> coloured line index
      if (movementIndex !== undefined) {
        if (!movementIndexToColouredLine.current.has(movementIndex)) {
          movementIndexToColouredLine.current.set(movementIndex, [])
        }
        movementIndexToColouredLine.current.get(movementIndex)!.push(lineIndex)
      }

      // Create the mapping for objectID -> coloured line index
      if (objectID !== undefined) {
        if (!objectIDToColouredLine.current.has(objectID)) {
          objectIDToColouredLine.current.set(objectID, [])
        }
        objectIDToColouredLine.current.get(objectID)!.push(lineIndex)
      }

      // console.log(`${lineCounter} [${start.x},${start.y}${start.z}]->[${end.x},${end.y}${end.z}]`)

      lineIndex += 1
    }

    let dottedLineIndex = 0

    const addDottedLine = (start: Vector3, end: Vector3, colorStart: RGBA, colorEnd: RGBA, movementIndex?: number, objectID?: string ) => {
      // Do the Blender -> ThreeJS coordinate system transform inline
      transitions.positions[dottedLineIndex * 6 + 0] = start.x
      transitions.positions[dottedLineIndex * 6 + 1] = start.z
      transitions.positions[dottedLineIndex * 6 + 2] = -start.y
      transitions.positions[dottedLineIndex * 6 + 3] = end.x
      transitions.positions[dottedLineIndex * 6 + 4] = end.z
      transitions.positions[dottedLineIndex * 6 + 5] = -end.y

      transitions.colors[dottedLineIndex * 6 + 0] = colorStart[0]
      transitions.colors[dottedLineIndex * 6 + 1] = colorStart[1]
      transitions.colors[dottedLineIndex * 6 + 2] = colorStart[2]
      transitions.colors[dottedLineIndex * 6 + 3] = colorEnd[0]
      transitions.colors[dottedLineIndex * 6 + 4] = colorEnd[1]
      transitions.colors[dottedLineIndex * 6 + 5] = colorEnd[2]

      // Create the mapping for movementIndex -> dotted line index
      if (movementIndex !== undefined) {
        if (!movementIndexToDottedLine.current.has(movementIndex)) {
          movementIndexToDottedLine.current.set(movementIndex, [])
        }
        movementIndexToDottedLine.current.get(movementIndex)!.push(dottedLineIndex)
      }

      // Create the mapping for objectID -> dotted line index
      if (objectID !== undefined) {
        if (!objectIDToDottedLine.current.has(objectID)) {
          objectIDToDottedLine.current.set(objectID, [])
        }
        objectIDToDottedLine.current.get(objectID)!.push(dottedLineIndex)
      }

      dottedLineIndex += 1
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

        const renderablesForFrame = getSetting(state => state.renderablesByFrame[state.viewportFrame]) ?? []

        const blenderCamera = renderablesForFrame.find(isCamera)

        const cameraPosition = new Vector3(
          blenderCamera?.position[0] ?? 0,
          blenderCamera?.position[1] ?? 0,
          blenderCamera?.position[2] ?? 0,
        )

        // Refresh the line geometries
        lineIndex = 0
        dottedLineIndex = 0

        // Refresh the line mapping
        movementIndexToColouredLine.current = new Map()
        movementIndexToDottedLine.current = new Map()
        objectIDToColouredLine.current = new Map()
        objectIDToDottedLine.current = new Map()

        // Refresh the react components
        setComponents(state => [])

        lines.positions = []
        transitions.positions = []
        lines.colors = []
        transitions.colors = []

        const orderedMovements = getOrderedMovementsForFrame(getSetting(state => state.viewportFrame))
        const settings = getSetting(state => state.settings)
        const visualisationSettings = getSetting(state => state.visualisationSettings)
        const denseMovements = sparseToDense(orderedMovements, settings)

        // Import the global material override if it exists
        const globalMaterialOverride = visualisationSettings.objectMaterialOverrides[GLOBAL_OVERRIDE_OBJECT_ID]
          ? importMaterial(visualisationSettings.objectMaterialOverrides[GLOBAL_OVERRIDE_OBJECT_ID])
          : null

        let durationCounter = 0

        const frameDuration = getSetting(state => state.estimatedDurationByFrame[state.viewportFrame])

        for (let index = 0; index < denseMovements.length; index++) {
          const movement = denseMovements[index]

          // Don't show hidden objects
          if (visualisationSettings.hiddenObjects[movement.objectID]) {
            continue
          }

          const duration = movement.getDuration()

          let renderThisMovementUpTo = 1

          // If doing preview,
          if (visualisationSettings.previewProgress) {
            const thisMovementStart = durationCounter / frameDuration
            const thisMovementEnd = (durationCounter + duration) / frameDuration

            /**
             *  Render up to |
             *   [     ] [     ] [    ]
             */
            if (thisMovementEnd <= visualisationSettings.frameProgress) {
              // render entire movement
              // this is a noop, continue the flow as normal
            } else if (
              thisMovementStart < visualisationSettings.frameProgress &&
              thisMovementEnd > visualisationSettings.frameProgress
            ) {
              // render this movement partially
              renderThisMovementUpTo = MathUtils.mapLinear(
                visualisationSettings.frameProgress,
                thisMovementStart,
                thisMovementEnd,
                0,
                1,
              )

              // Calculate the current position of the delta
              const deltaPos = movement.samplePoint(renderThisMovementUpTo)

              changeState(state => {
                state.endEffector.x = deltaPos.x
                state.endEffector.y = deltaPos.y
                state.endEffector.z = deltaPos.z
              })
            } else {
              // don't render this movement
              continue
            }
          }

          // Update the duration counter
          durationCounter += duration

          // Don't show zero duration moves
          if (duration === 0) {
            continue
          }

          let material: Material = movement.material

          // Global overrides take least precidence
          if (globalMaterialOverride) {
            material = globalMaterialOverride
          }

          // Get the override if it has one
          material = getMaterialOverride(visualisationSettings, material, movement.overrideKeys)
          // If the movement is flipped, reverse the ordering of the material so it stays consistent
          const matStartT = movement.isFlipped ? 1 : 0
          const matendT = movement.isFlipped ? 0 : 1

          // Generate using the
          material.generateThreeJSRepresentation(
            index,
            movement,
            settings,
            visualisationSettings,
            cameraPosition,
            addColouredLine,
            addDottedLine,
            addReactComponent,
            matStartT,
            matendT,
            0,
            renderThisMovementUpTo,
          )
        }

        lines.refreshGeometry(lineIndex)
        transitions.refreshGeometry(dottedLineIndex)

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
      state => state.treeStore.hoveredItems,
      hoveredItems => {
        if (hoveredItems.length === 0) {
          // If nothing is hovered, reset both
          lines.setHoveredIndices([], false, backgroundCol)
          transitions.setHoveredIndices([], false, backgroundCol)
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

        for (const itemID of hoveredItems) {
          let colouredIndices = movementIndexToColouredLine.current.get(itemID)

          if (colouredIndices) {
            for (let i = 0; i < colouredIndices.length; i++) {
              const lineIndex = colouredIndices[i]
              colouredLineIndices.push(lineIndex)
            }
          }

          let invisibleIndices = movementIndexToDottedLine.current.get(itemID)

          if (invisibleIndices) {
            for (let i = 0; i < invisibleIndices.length; i++) {
              const lineIndex = invisibleIndices[i]
              invisibleLineIndices.push(lineIndex)
            }
          }

          colouredIndices = objectIDToColouredLine.current.get(itemID)

          if (colouredIndices) {
            for (let i = 0; i < colouredIndices.length; i++) {
              const lineIndex = colouredIndices[i]
              colouredLineIndices.push(lineIndex)
            }
          }

          invisibleIndices = objectIDToDottedLine.current.get(itemID)

          if (invisibleIndices) {
            for (let i = 0; i < invisibleIndices.length; i++) {
              const lineIndex = invisibleIndices[i]
              invisibleLineIndices.push(lineIndex)
            }
          }
        }

        // If there are hovered objects, but no indices can be found, hide everything.
        lines.setHoveredIndices(colouredLineIndices, colouredLineIndices.length === 0, backgroundCol)
        transitions.setHoveredIndices(invisibleLineIndices, invisibleLineIndices.length === 0, backgroundCol)
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

  const setOrbitControlsRef = useCallback((orbitControls: OrbitControlsImpl) => {
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
  }, [])

  return (
    <Canvas linear dpr={[1, 2]} style={{ zIndex: 0 }} shadows={{ enabled: true, type: PCFSoftShadowMap }}>
      <PerspectiveCamera ref={setCameraRef} makeDefault position={[0, 150, 400]} far={10000} />
      <OrbitControls ref={setOrbitControlsRef} />
      <AxisLines />

      <ambientLight intensity={0.1} />

      {/* Top down */}
      <spotLight 
        position={[0, 100, 0]}
        intensity={1.0} 
        penumbra={1} 
        angle={Math.PI / 3} 
        castShadow 
        color="#cee8f2" 
        shadow-mapSize-x={2048}
        shadow-mapSize-y={2048}
        shadow-normalBias={1}
      />

      {/* From the side */}
      <spotLight 
        position={[300, -200, 300]}
        intensity={1} 
        penumbra={1} 
        angle={Math.PI / 6} 
        castShadow 
        color="#cee8f2" 
        shadow-mapSize-x={2048}
        shadow-mapSize-y={2048}
        shadow-normalBias={1}
      />

      <ToolpathMovements />
      <DeltaAssembly />
      
      <GroundPlane />
      <fog attach="fog" args={['#101010', 500, 3000]} />
    </Canvas>
  )
}
