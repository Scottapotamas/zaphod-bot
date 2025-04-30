import * as React from 'react'
import { useCallback, useRef } from 'react'

import { Canvas } from '@react-three/fiber'

import { OrbitControls, PerspectiveCamera } from '@react-three/drei'

import { OrbitControls as OrbitControlsImpl } from 'three-stdlib'

import { PCFSoftShadowMap, PerspectiveCamera as PerspectiveCameraImpl, SpotLight } from 'three'
import { changeState, getSetting } from '../../toolpaths/interface/state'

import { BridgeContextsContext } from '@electricui/components-desktop-charts'

import { useContextBridge } from '@react-three/drei'

import { GroundPlane } from 'src/application/components/GroundPlane'
import { DeltaAssembly } from 'src/application/components/RiggedModel'
import { ToolpathMovements } from '../../toolpaths/interface/ToolpathVisualisation'

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
