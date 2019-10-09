import * as THREE from 'three'

import {
  Canvas,
  extend,
  useFrame,
  useRender,
  useThree,
} from 'react-three-fiber'
import {
  IntervalRequester,
  StateTree,
  useDeviceMetadataKey,
  useHardwareState,
} from '@electricui/components-core'
import React, { useEffect, useRef } from 'react'

import { PolyLineCurve3 } from './PolyLineCurve'

const meshline = require('three.meshline')

const { OrbitControls } = require('./OrbitControls')

extend(meshline)
extend({ PolyLineCurve3, OrbitControls }) // add a PolyLineCurve3, OrbitControls components to react-three-fibre

function Thing() {
  const ref = useRef<THREE.Mesh>()

  useFrame(() => {
    if (!ref || !ref.current) {
      return
    }

    ref.current.rotation.z += 0.01
  })

  return (
    <mesh
      ref={ref}
      onClick={e => console.log('click')}
      onPointerOver={e => console.log('hover')}
      onPointerOut={e => console.log('unhover')}
    >
      <planeBufferGeometry attach="geometry" args={[1, 1]} />
      <meshBasicMaterial
        attach="material"
        color="hotpink"
        opacity={0.5}
        transparent
      />
    </mesh>
  )
}
interface CameraAndOrbitProps {
  containerRef: HTMLDivElement
}

function CameraAndOrbit(props: CameraAndOrbitProps) {
  const camera = useRef<THREE.PerspectiveCamera>(null)
  const controls = useRef<typeof OrbitControls>(null)

  const { size, setDefaultCamera } = useThree()

  useEffect(() => {
    if (camera.current) {
      setDefaultCamera(camera.current)
    }
  }, [])
  useFrame(() => {
    if (controls.current) {
      controls.current.update()
    }
  })

  //  radius={(size.width + size.height) / 4}
  return (
    <React.Fragment>
      <perspectiveCamera
        ref={camera}
        aspect={size.width / size.height}
        fov={75}
        position={[-3, 0, 8]}
        onUpdate={self => self.updateProjectionMatrix()}
      />
      {camera.current && (
        <group>
          <orbitControls
            ref={controls}
            args={[camera.current, props.containerRef]}
            enableDamping
            dampingFactor={0.1}
            rotateSpeed={0.1}
          />
          <Thing />
        </group>
      )}
    </React.Fragment>
  )
}

export function Renderer() {
  const containerRef = useRef<HTMLDivElement>(null)

  return (
    <div ref={containerRef} style={{ width: '100%', height: '100%' }}>
      <Canvas>
        {containerRef.current && (
          <CameraAndOrbit containerRef={containerRef.current} />
        )}
      </Canvas>
    </div>
  )
}
