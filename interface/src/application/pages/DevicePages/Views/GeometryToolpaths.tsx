import * as React from 'react'
import { useRef, useMemo, useState, forwardRef, useCallback, useEffect } from 'react'

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
import { Mesh, Group, Color, Vector2, PerspectiveCamera as PerspectiveCameraImpl, VSMShadowMap, PCFSoftShadowMap, SpotLight } from 'three'
import { changeState, getSetting, setSetting, useSetting, useStore } from '../../toolpaths/interface/state'

import { BridgeContextsContext } from '@electricui/components-desktop-charts'

import { useContextBridge } from '@react-three/drei'

import { ToolpathMovements, AxisLines } from '../../toolpaths/interface/ToolpathVisualisation'
import { DeltaAssembly } from 'src/application/components/RiggedModel'
import { GroundPlane } from 'src/application/components/GroundPlane'

export const GeometryToolpathViewer = () => {
  const setCameraRef = useCallback((camera: PerspectiveCameraImpl) => {
    if (camera) {
      changeState(state => {
        state.camera = camera
      })
    }
  }, [])

  const setOrbitControlsRef = useCallback((orbitControls: OrbitControlsImpl) => {
    if (orbitControls) {
      changeState(state => {
        state.orbitControls = orbitControls as any
      })

      const cam = getSetting(state => state.camera)
      // Set the camera
      if (cam) {
        orbitControls.object = cam
      }
    }
  }, [])

  const listOfContextsToBridge = React.useContext(BridgeContextsContext)

  const ContextBridge = useContextBridge(...listOfContextsToBridge)

  const spotlightRef = useRef<SpotLight>(null)

  return (
    // PCFShadowMap is default but has artifacts, 
    // PCFSoftShadowMap seems similar
    // VSMShadowMap has different artifacts
    <Canvas linear dpr={[1, 2]} style={{ zIndex: 0 }} shadows={{ enabled: true, type: PCFSoftShadowMap }}>
      <ContextBridge>
        <PerspectiveCamera ref={setCameraRef} makeDefault position={[0, 150, 600]} />
        <OrbitControls ref={setOrbitControlsRef} />
        {/* <AxisLines /> */}

        <ambientLight intensity={0.2} />
        {/* <directionalLight position={[-100, 0, -50]} intensity={1} color="#cee8f2" castShadow/> */}
        {/* <directionalLight position={[-10, -20, -50]} intensity={0.3} color="#cee8f2" castShadow/> */}

        <group position={[300, -200, 250]}>
          <spotLight 
          ref={spotlightRef} 
          intensity={2.5} 
          penumbra={1} 
          angle={Math.PI / 6} 
          castShadow 
          color="#cee8f2" 
          shadow-mapSize-x={2048}
          shadow-mapSize-y={2048}
          shadow-normalBias={1}
          />
        </group>
        
        <ToolpathMovements />
        <DeltaAssembly />

        <GroundPlane />
        <fog attach="fog" args={['#101010', 500, 3000]} />
      </ContextBridge>
    </Canvas>
  )
}
