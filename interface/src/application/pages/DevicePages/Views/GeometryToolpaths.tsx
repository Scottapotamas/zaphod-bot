import * as React from 'react'
import {
  useRef,
  useMemo,
  useState,
  forwardRef,
  useCallback,
  useEffect,
} from 'react'

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
import {
  getSetting,
  setSetting,
  useSetting,
  useStore,
} from '../../toolpaths/interface/state'

import { BridgeContextsContext } from '@electricui/components-desktop-charts'

import { useContextBridge } from '@react-three/drei'

import {
  ToolpathMovements,
  AxisLines,
} from '../../toolpaths/interface/ToolpathVisualisation'
import { DeltaAssembly } from 'src/application/components/RiggedModel'

export const GeometryToolpathViewer = () => {
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

  const listOfContextsToBridge = React.useContext(BridgeContextsContext)

  const ContextBridge = useContextBridge(...listOfContextsToBridge)

  return (
    <Canvas
      linear
      dpr={[1, 2]}
      // shadows={true}
    >
      <ContextBridge>
        <PerspectiveCamera
          ref={setCameraRef}
          makeDefault
          position={[0, 150, 600]}
        />
        <OrbitControls ref={setOrbitControlsRef} />
        {/* <AxisLines /> */}

        <ambientLight intensity={0.4} />
        <directionalLight position={[-100, 0, -50]} intensity={1} color="#0c8cbf" />
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
        <DeltaAssembly />
        {/* <fog attach="fog" args={['#101010', 600, 3000]} /> */}
      </ContextBridge>
    </Canvas>
  )
}
