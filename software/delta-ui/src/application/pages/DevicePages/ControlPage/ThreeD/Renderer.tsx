import * as THREE from 'three'

import {
  Canvas,
  extend,
  useFrame,
  useRender,
  useThree,
} from 'react-three-fiber'
import { CollectionForUI, Vertices, ViewerVertices } from './types'
import {
  IntervalRequester,
  StateTree,
  useDeviceMetadataKey,
  useHardwareState,
} from '@electricui/components-core'
import React, { useEffect, useMemo, useRef, useState } from 'react'

import { PolyLineCurve3 } from './PolyLineCurve'

const meshline = require('three.meshline')

const { OrbitControls } = require('./OrbitControls')

extend(meshline)
extend({ PolyLineCurve3, OrbitControls }) // add a PolyLineCurve3, OrbitControls components to react-three-fibre

interface CurrentToolHeadPositionProps {
  headPosition: [number, number, number]
}

function CurrentToolHeadPosition(props: CurrentToolHeadPositionProps) {
  const [geo, mat] = useMemo(() => {
    const geo = new THREE.SphereBufferGeometry(1, 10, 10)
    const mat = new THREE.MeshBasicMaterial({
      color: new THREE.Color('lightblue'),
    })
    return [geo, mat]
  }, [])
  return (
    <group>
      <mesh geometry={geo} material={mat} position={props.headPosition} />
    </group>
  )
}

interface FatLineProps {
  curve: THREE.Vector3[]
  tex: string
}

function FatLine(props: FatLineProps) {
  const material = useRef()
  const [width] = useState(() => 1)

  const { curve, tex } = props
  const texture = useMemo(() => new THREE.TextureLoader().load(tex), [tex])

  return (
    <mesh>
      <meshLine
        onUpdate={(self: any) => (self.parent.geometry = self.geometry)}
      >
        <geometry onUpdate={(self: any) => self.parent.setGeometry(self)}>
          <polyLineCurve3
            args={[curve]}
            onUpdate={(self: any) =>
              (self.parent.vertices = self.getPoints(curve.length - 1))
            }
          />
        </geometry>
      </meshLine>

      {/** MeshLineMaterial on the other hand is a regular material, so we can just attach it */}
      <meshLineMaterial
        attach="material"
        ref={material}
        transparent
        depthTest={true}
        lineWidth={width}
        useMap={true}
        map={texture}
      />
    </mesh>
  )
}

interface LoadedLineProps {
  collection: CollectionForUI
}

function LoadedLine(props: LoadedLineProps) {
  // only recalculate this when the name or duration changes.
  const processedVertices = useMemo(() => {
    const collectionVertices = props.collection.viewer_vertices.map(
      point => new THREE.Vector3(-point[0], point[2], point[1]),
    )

    return collectionVertices
  }, [props.collection.name, props.collection.duration])

  return <FatLine curve={processedVertices} tex={props.collection.viewer_uv} />
}

interface SceneProps {
  loadedCollectionData: Array<CollectionForUI>
  summaryFilePath: string
  currentFrame: number
}

export function Scene(props: SceneProps) {
  return (
    <group>
      {props.loadedCollectionData.map(collection => (
        <LoadedLine
          collection={collection}
          key={`${props.summaryFilePath}-${props.currentFrame}-${collection.name}`}
        />
      ))}
    </group>
  )
}

interface CameraAndOrbitProps {
  containerRef: HTMLDivElement | null
  loadedCollectionData: Array<CollectionForUI>
  summaryFilePath: string
  currentFrame: number
  headPosition: [number, number, number]
}

function CameraAndOrbitScene(props: CameraAndOrbitProps) {
  const camera = useRef<THREE.PerspectiveCamera>(null)
  const controls = useRef<typeof OrbitControls>(null)

  const { size, setDefaultCamera } = useThree()

  useEffect(() => {
    if (camera.current) {
      setDefaultCamera(camera.current)
    }
  }, [camera.current])
  useFrame(() => {
    if (controls.current) {
      controls.current.update()
    }
  })

  const [position] = useState([-60, -50, 80])

  const customCameraProps = {
    radius: (size.width + size.height) / 4,
  }

  return (
    <React.Fragment>
      <perspectiveCamera
        ref={camera}
        aspect={size.width / size.height}
        fov={75}
        position={position}
        {...(customCameraProps as any)}
        onUpdate={self => self.updateProjectionMatrix()}
      />
      {camera.current && props.containerRef && (
        <group>
          <orbitControls
            ref={controls}
            args={[camera.current, props.containerRef]}
            enableDamping
            dampingFactor={0.1}
            rotateSpeed={0.1}
          />
          <Scene
            key={`${props.summaryFilePath}-${props.currentFrame}`}
            summaryFilePath={props.summaryFilePath}
            currentFrame={props.currentFrame}
            loadedCollectionData={props.loadedCollectionData}
          />
          <CurrentToolHeadPosition headPosition={props.headPosition} />
        </group>
      )}
    </React.Fragment>
  )
}

interface RendererProps {
  loadedCollectionData: Array<CollectionForUI>
  summaryFilePath: string
  currentFrame: number
  headPosition: [number, number, number]
}

export function Renderer(props: RendererProps) {
  const containerRef = useRef<HTMLDivElement>(null)

  return (
    <div ref={containerRef} style={{ width: '100%', height: '100%' }}>
      <Canvas>
        <CameraAndOrbitScene
          containerRef={containerRef.current}
          summaryFilePath={props.summaryFilePath}
          currentFrame={props.currentFrame}
          loadedCollectionData={props.loadedCollectionData}
          headPosition={props.headPosition}
        />
      </Canvas>
    </div>
  )
}
