import { ReactThreeFiber } from 'react-three-fiber/src/three-types'
import * as THREE from 'three'

declare global {
  namespace JSX {
    interface IntrinsicElements {
      orbitControls: ReactThreeFiber.Object3DNode<any, any>
    }
  }
}
