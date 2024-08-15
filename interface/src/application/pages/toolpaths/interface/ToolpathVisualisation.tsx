import * as React from 'react'
import { useRef, useState, useCallback, useEffect } from 'react'

// import // Environment,
// // OrbitControls,
// '@electricui/components-desktop-three'

import { Canvas, ReactThreeFiber, extend } from '@react-three/fiber'

import { OrbitControls, PerspectiveCamera } from '@react-three/drei'

import { OrbitControls as OrbitControlsImpl } from 'three-stdlib'

import { useFrame, useThree } from '@react-three/fiber'
import {
  Color,
  PerspectiveCamera as PerspectiveCameraImpl,
  MathUtils,
  AdditiveBlending,
} from 'three'
import { changeState, getSetting, setSetting, useStore } from './state'
import { sparseToDense } from '../optimiser/passes'
import { Vector3, PCFSoftShadowMap } from 'three'
import {
  deserialiseTour,
  GLOBAL_OVERRIDE_OBJECT_ID,
  Movement,
  RGBA,
} from '../optimiser/movements'
import { getMaterialOverride, importMaterial } from '../optimiser/material'
import { Material } from '../optimiser/materials/Base'
import { DeltaAssembly } from './../../../components/RiggedModel'
import { isCamera } from '../optimiser/camera'
import { useDarkMode } from '@electricui/components-desktop'
import { lerpRGBA } from '../optimiser/materials/utilities'
import { GroundPlane } from 'src/application/components/GroundPlane'
import {
  useThickLineMaterial,
  LineSegmentDataStore,
  LineSegments2,
} from '@electricui/charts'

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

function convertToThreeCoordinateSystem(
  vector: [number, number, number],
): [number, number, number] {
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
  // Allocate a 100k line segment block, which is about 4MB
  const [lines] = useState(() => {
    const store = new LineSegmentDataStore(100_000)
    store.setBoundsRequired(false, false, false, false, false, false)
    return store
  })
  // Allocate a second 100k line segment block
  const [transitions] = useState(() => {
    const store = new LineSegmentDataStore(100_000)
    store.setBoundsRequired(false, false, false, false, false, false)
    return store
  })

  const [customComponents, setComponents] = useState<React.ReactNode[]>([])

  const movementIndexToColouredLine: React.MutableRefObject<
    Map<string | number, number[]>
  > = useRef(new Map())
  const movementIndexToDottedLine: React.MutableRefObject<
    Map<string | number, number[]>
  > = useRef(new Map())
  const objectIDToColouredLine: React.MutableRefObject<
    Map<string | number, number[]>
  > = useRef(new Map())
  const objectIDToDottedLine: React.MutableRefObject<
    Map<string | number, number[]>
  > = useRef(new Map())

  useEffect(() => {
    // Keep a vector around for doing distance calculations
    const distScratchVector = new Vector3(0, 0, 0)

    const addColouredLine = (
      start: Vector3,
      end: Vector3,
      colorStart: RGBA,
      colorEnd: RGBA,
      movementIndex: number,
      objectID?: string,
    ) => {
      // Do the Blender -> ThreeJS coordinate system transform inline
      const headIdx = lines.add(
        start.x,
        start.z,
        -start.y,
        1, // Coloured lines aren't dashed, so we don't need to bother with the line distance calculation, just treat them all as '1'
        end.x,
        end.z,
        -end.y,
        1,
        colorStart[0],
        colorStart[1],
        colorStart[2],
        1, // Alpha is determined purely by hover behaviour
        colorEnd[0],
        colorEnd[1],
        colorEnd[2],
        1,
      )

      // Create the mapping for movementIndex -> coloured line index
      if (movementIndex !== undefined && headIdx !== undefined) {
        if (!movementIndexToColouredLine.current.has(movementIndex)) {
          movementIndexToColouredLine.current.set(movementIndex, [])
        }
        movementIndexToColouredLine.current.get(movementIndex)!.push(headIdx)
      }

      // Create the mapping for objectID -> coloured line index
      if (objectID !== undefined && headIdx !== undefined) {
        if (!objectIDToColouredLine.current.has(objectID)) {
          objectIDToColouredLine.current.set(objectID, [])
        }
        objectIDToColouredLine.current.get(objectID)!.push(headIdx)
      }

      // console.log(`${lineCounter} [${start.x},${start.y}${start.z}]->[${end.x},${end.y}${end.z}]`)
    }

    const addDottedLine = (
      start: Vector3,
      end: Vector3,
      colorStart: RGBA,
      colorEnd: RGBA,
      movementIndex?: number,
      objectID?: string,
    ) => {
      // Calculate line distances immediately assuming contiguity
      const d1 = distScratchVector.distanceTo(start)
      const d2 = start.distanceTo(end)
      distScratchVector.set(end.x, end.y, end.z)

      // Do the Blender -> ThreeJS coordinate system transform inline
      const headIdx = transitions.add(
        start.x,
        start.z,
        -start.y,
        d1,
        end.x,
        end.z,
        -end.y,
        d2,
        colorStart[0],
        colorStart[1],
        colorStart[2],
        1, // Alpha is determined purely by hover behaviour
        colorEnd[0],
        colorEnd[1],
        colorEnd[2],
        1,
      )

      // Create the mapping for movementIndex -> dotted line index
      if (movementIndex !== undefined && headIdx !== undefined) {
        if (!movementIndexToDottedLine.current.has(movementIndex)) {
          movementIndexToDottedLine.current.set(movementIndex, [])
        }
        movementIndexToDottedLine.current.get(movementIndex)!.push(headIdx)
      }

      // Create the mapping for objectID -> dotted line index
      if (objectID !== undefined && headIdx !== undefined) {
        if (!objectIDToDottedLine.current.has(objectID)) {
          objectIDToDottedLine.current.set(objectID, [])
        }
        objectIDToDottedLine.current.get(objectID)!.push(headIdx)
      }
    }

    const reactComponents: React.ReactNode[] = []

    // This is expensive, avoid if possible
    const addReactComponent = (component: React.ReactNode) => {
      reactComponents.push(component)
    }

    const flushReactComponents = () => {
      // Make the copy here
      setComponents(state => reactComponents.slice())
      reactComponents.length = 0
    }

    const unsubscribe = useStore.subscribe(
      state => state.viewportFrameVersion,
      () => {
        // On viewport frame change, or toolpath update, regenerate the ordering,
        // and update the lines

        const renderablesForFrame =
          getSetting(state => state.renderablesByFrame[state.viewportFrame]) ??
          []

        const blenderCamera = renderablesForFrame.find(isCamera)

        const cameraPosition = new Vector3(
          blenderCamera?.position[0] ?? 0,
          blenderCamera?.position[1] ?? 0,
          blenderCamera?.position[2] ?? 0,
        )

        if (lines.geometries.length === 0) return
        if (transitions.geometries.length === 0) return

        // Refresh the line geometries
        lines.reset()
        transitions.reset()

        // Refresh the line mapping
        movementIndexToColouredLine.current.clear()
        movementIndexToDottedLine.current.clear()
        objectIDToColouredLine.current.clear()
        objectIDToDottedLine.current.clear()

        // Refresh the react components list
        reactComponents.length = 0

        const orderedMovements = getOrderedMovementsForFrame(
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

        let durationCounter = 0

        const frameDuration = getSetting(
          state => state.estimatedDurationByFrame[state.viewportFrame],
        )

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
          material = getMaterialOverride(
            visualisationSettings,
            material,
            movement.overrideKeys,
          )
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

        // Flush the commits
        lines.commit()
        transitions.commit()
        flushReactComponents()

        // console.log(`built ${lineCounter} lines`, lines.line.visible)
      },
    )

    return () => {
      unsubscribe()

      lines?.unmount()
      transitions?.unmount()
    }
  }, [lines, transitions])

  const { width: boundsWidth, height: boundsHeight } = useThree(
    state => state.size,
  )

  // Extract the materials
  const {
    getPrePassMat: getLinesPrePassMat, //
    getLineMat: getLinesLineMat,
  } = useThickLineMaterial(boundsWidth, boundsHeight, 4, false)

  const {
    getPrePassMat: getTransitionPrePassMat,
    getLineMat: getTransitionLineMat,
  } = useThickLineMaterial(boundsWidth, boundsHeight, 4, true)

  // Set the actual line mats to be additive blend mode
  useEffect(() => {
    getLinesLineMat().blending = AdditiveBlending
    getLinesLineMat().uniformsNeedUpdate = true
    getLinesLineMat().needsUpdate = true

    getTransitionLineMat().blending = AdditiveBlending
    getTransitionLineMat().uniformsNeedUpdate = true
    getTransitionLineMat().needsUpdate = true
  }, [getLinesLineMat(), getTransitionLineMat()])

  // Transition lines should subtly move over time to indicate their direction
  useFrame((_, delta) => {
    const prePassMat = getTransitionPrePassMat()
    const transitionLineMat = getTransitionLineMat()

    // Negative offset moves the line in the direction of movements.
    // prePassMat.uniforms.dashOffset.value -= delta * 2
    // prePassMat.uniformsNeedUpdate = true
    // prePassMat.needsUpdate = true

    //
    // prePassMat.dashed = false
    // prePassMat.needsUpdate = true

    //
    // transitionLineMat.uniforms.dashOffset.value -= delta * 2
    // transitionLineMat.uniformsNeedUpdate = true
    // transitionLineMat.needsUpdate = true
  })

  // On hovering change, update the lines
  useEffect(() => {
    return useStore.subscribe(
      state => state.treeStore.hoveredItems,
      hoveredItems => {
        // Trigger a GPU update
        lines.geometries[0].instanceBuffer.needsUpdate = true
        lines.geometries[0].instanceColorBuffer.needsUpdate = true

        transitions.geometries[0].instanceBuffer.needsUpdate = true
        transitions.geometries[0].instanceColorBuffer.needsUpdate = true

        if (hoveredItems.length === 0) {
          // If nothing is hovered, set everything to 'hovered'
          setAllHoverState(lines, true)
          setAllHoverState(transitions, true)

          return
        }

        // Set everything to unhovered
        setAllHoverState(lines, false)
        setAllHoverState(transitions, false)

        // Set particular ones back to hovered

        for (const itemID of hoveredItems) {
          let colouredIndices = movementIndexToColouredLine.current.get(itemID)

          if (colouredIndices) {
            for (let i = 0; i < colouredIndices.length; i++) {
              const lineIndex = colouredIndices[i]
              setHoverState(lines, lineIndex, true)
            }
          }

          let invisibleIndices = movementIndexToDottedLine.current.get(itemID)

          if (invisibleIndices) {
            for (let i = 0; i < invisibleIndices.length; i++) {
              const lineIndex = invisibleIndices[i]
              setHoverState(transitions, lineIndex, true)
            }
          }

          colouredIndices = objectIDToColouredLine.current.get(itemID)

          if (colouredIndices) {
            for (let i = 0; i < colouredIndices.length; i++) {
              const lineIndex = colouredIndices[i]
              setHoverState(lines, lineIndex, true)
            }
          }

          invisibleIndices = objectIDToDottedLine.current.get(itemID)

          if (invisibleIndices) {
            for (let i = 0; i < invisibleIndices.length; i++) {
              const lineIndex = invisibleIndices[i]
              setHoverState(transitions, lineIndex, true)
            }
          }
        }
      },
    )
  }, [lines, transitions])

  return (
    <>
      {/* Lines */}
      {lines.geometries.map((geometryStruct, index) => (
        <React.Fragment key={index}>
          <lineSegments2
            geometry={geometryStruct.geometry as any}
            material={getLinesPrePassMat()}
            frustumCulled={false}
            matrixAutoUpdate={false}
            renderOrder={0} // Render the pre-pass material first
          ></lineSegments2>
          <lineSegments2
            geometry={geometryStruct.geometry as any}
            material={getLinesLineMat()}
            frustumCulled={false}
            matrixAutoUpdate={false}
            renderOrder={1} // Then render the actual line
          ></lineSegments2>
        </React.Fragment>
      ))}

      {/* Transitions */}
      {transitions.geometries.map((geometryStruct, index) => (
        <React.Fragment key={index}>
          <lineSegments2
            geometry={geometryStruct.geometry as any}
            material={getTransitionPrePassMat()}
            frustumCulled={false}
            matrixAutoUpdate={false}
            renderOrder={2} // Render the pre-pass material first
          ></lineSegments2>
          <lineSegments2
            geometry={geometryStruct.geometry as any}
            material={getTransitionLineMat()}
            frustumCulled={false}
            matrixAutoUpdate={false}
            renderOrder={3} // Then render the actual line
          ></lineSegments2>
        </React.Fragment>
      ))}

      {customComponents}
    </>
  )
}

extend({ LineSegments2 })

declare global {
  namespace JSX {
    interface IntrinsicElements {
      lineSegments2: ReactThreeFiber.Object3DNode<
        LineSegments2,
        typeof LineSegments2
      >
    }
  }
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

  const backgroundCol = useDarkMode() ? '#191b1d' : '#f5f8fa'

  return (
    <Canvas
      linear
      dpr={[1, 2]}
      style={{ zIndex: 0 }}
      shadows={{ enabled: true, type: PCFSoftShadowMap }}
    >
      <color attach="background" args={[backgroundCol]} />
      <PerspectiveCamera
        ref={setCameraRef}
        makeDefault
        position={[0, 150, 400]}
        far={10000}
      />
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
      {/* <fog attach="fog" args={['#101010', 500, 3000]} /> */}
    </Canvas>
  )
}

function setHoverState(
  store: LineSegmentDataStore,
  index: number,
  hovered: boolean,
) {
  // TODO: When they have multiple, this will need to be changed
  const geometry = store.geometries[0]

  const headIdx = index * 10 // 10 floats per struct
  const rgba1Idx = (headIdx << 2) + 16 // 4 bytes per float, 4 f32s along the struct
  const rgba2Idx = (headIdx << 2) + 36 // 4 bytes per float, 9 f32s bytes along the struct

  // TODO: When we do partial updates, this will need to add an update range
  const alpha = hovered ? 1 : 0.2

  geometry.rgba[rgba1Idx + 3] = (alpha * 255) | 0
  geometry.rgba[rgba2Idx + 3] = (alpha * 255) | 0
}

function setAllHoverState(store: LineSegmentDataStore, hovered: boolean) {
  const tail = store.geometries[0].tail
  const head = store.geometries[0].head
  const maxItems = store.geometries[0].maxItems

  if (tail <= head) {
    // one linear pass
    for (let index = tail; index < head; index++) {
      setHoverState(store, index, hovered)
    }
  } else {
    // two passes
    for (let index = tail; index < maxItems; index++) {
      setHoverState(store, index, hovered)
    }
    for (let index = 0; index < head; index++) {
      setHoverState(store, index, hovered)
    }
  }
}
