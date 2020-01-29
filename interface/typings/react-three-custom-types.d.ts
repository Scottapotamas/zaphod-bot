import { ReactThreeFiber } from 'react-three-fiber/src/three-types'
import * as THREE from 'three'
import { PerspectiveCamera } from 'three'
import { PolyLineCurve3 } from '../src/application/pages/DevicePages/DataViews/ThreeD/PolyLineCurve'

declare global {
  namespace JSX {
    interface IntrinsicElements {
      orbitControls: ReactThreeFiber.Object3DNode<any, any>
      meshLine: ReactThreeFiber.Object3DNode<any, any>
      meshLineMaterial: ReactThreeFiber.Object3DNode<any, any>
      polyLineCurve3: ReactThreeFiber.Object3DNode<any, any>
    }
  }
}
