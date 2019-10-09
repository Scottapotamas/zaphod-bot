import * as THREE from 'three'
import * as meshline from 'three.meshline'

import { Canvas, apply, useRender, useThree } from 'react-three-fiber'
import React, {
  useCallback,
  useContext,
  useEffect,
  useMemo,
  useRef,
  useState,
} from 'react'

import Datauri from 'datauri'
import { OrbitControls } from './OrbitControls'
import { PolyLineCurve3 } from './PolyLineCurve'
import fs from 'fs'
import path from 'path'

apply(meshline)
apply({ PolyLineCurve3, OrbitControls }) // add a PolyLineCurve3, OrbitControls components to react-three-fibre

const colors = ['#A2CCB6', '#FCEEB5', '#EE786E', '#EE786E']

function Fatline(props) {
  const material = useRef()
  const [width] = useState(() => 0.01)

  const { curve, tex, curveType } = props
  const texture = useMemo(() => new THREE.TextureLoader().load(tex), [tex])

  let geo = null

  switch (curveType) {
    case 'poly':
      geo = (
        <geometry onUpdate={self => self.parent.setGeometry(self)}>
          <polyLineCurve3
            args={[curve]}
            onUpdate={self =>
              (self.parent.vertices = self.getPoints(curve.length - 1))
            }
          />
        </geometry>
      )
      break

    case 'nurbs':
      geo = (
        <geometry onUpdate={self => self.parent.setGeometry(self)}>
          <catmullRomCurve3
            args={[curve]}
            onUpdate={self =>
              (self.parent.vertices = self.getPoints(curve.length * 10))
            }
          />
        </geometry>
      )
      break
    default:
      console.log('No geometry mode available for', props)
      break
  }

  return (
    <mesh>
      {/** MeshLine and CMRCurve are a OOP factories, not scene objects, hence all the imperative code in here :-( */}
      <meshLine onUpdate={self => (self.parent.geometry = self.geometry)}>
        {geo}
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

function parse(relFilePath) {
  const filePath = path.join(__dirname, relFilePath)
  const contents = fs.readFileSync(filePath)
  const parsed = JSON.parse(contents)

  console.log('loaded', filePath)

  let uvPath = path.join(path.dirname(filePath), parsed.uv)

  if (!fs.existsSync(uvPath)) {
    uvPath = path.join(__dirname, './Collection/defaultUV.png')
  }

  const uvDataURI = new Datauri(uvPath)
  const uv = uvDataURI.content

  console.log('loaded uv', uvPath)

  let pointsList = parsed.points

  // if it's cyclic, add a point at the end
  if (parsed.cyclic) {
    pointsList.push(pointsList[0])
  }

  let points = []
  const curveType = parsed.type

  // convert to the THREE type
  switch (curveType) {
    case 'poly':
      points = pointsList.map(
        point => new THREE.Vector3(-point[0], point[2], point[1]),
      )
    case 'nurbs':
      points = pointsList.map(
        point => new THREE.Vector3(-point[0], point[2], point[1]),
      )
      break

    default:
      break
  }

  return {
    curveType,
    points,
    uv,
  }
}

function LoadedLine({ relFilePath }) {
  const parsed = useMemo(() => parse(relFilePath), [relFilePath])

  return (
    <Fatline
      curve={parsed.points}
      tex={parsed.uv}
      curveType={parsed.curveType}
    />
  )
}

function Scene() {
  let lineGroup = useRef()
  // Hook into the render loop and rotate the scene a bit

  return (
    <group ref={lineGroup}>
      {/* <LoadedLine relFilePath={'./Collection/Arrow_0.json'} />
      <LoadedLine relFilePath={'./Collection/DisconnectedSplines_0.json'} />
      <LoadedLine relFilePath={'./Collection/DisconnectedSplines_1.json'} />
      <LoadedLine relFilePath={'./Collection/DisconnectedSplines_2.json'} />
      <LoadedLine relFilePath={'./Collection/Torus_0.json'} /> */}
    </group>
  )
}

function CameraAndOrbit({ containerRef }) {
  const camera = useRef()
  const controls = useRef()

  const { size, setDefaultCamera } = useThree()

  useEffect(() => void setDefaultCamera(camera.current), [])
  useRender(() => controls.current && controls.current.update())

  return (
    <React.Fragment>
      <perspectiveCamera
        ref={camera}
        aspect={size.width / size.height}
        radius={(size.width + size.height) / 4}
        fov={75}
        position={[-3, 0, 8]}
        onUpdate={self => self.updateProjectionMatrix()}
      />
      {camera.current && (
        <group>
          <orbitControls
            ref={controls}
            args={[camera.current, containerRef.current]}
            enableDamping
            dampingFactor={0.1}
            rotateSpeed={0.1}
          />
          <Scene />
        </group>
      )}
    </React.Fragment>
  )
}

function Renderer() {
  const containerRef = useRef()
  return (
    <div ref={containerRef}>
      <Canvas style={{ background: '#191b1d', height: 320 }}>
        <CameraAndOrbit containerRef={containerRef} />
      </Canvas>
    </div>
  )
}

export { Renderer }
