import * as React from 'react'
import { useRef, useMemo, useState, forwardRef, useCallback, useEffect } from 'react'

import { Canvas } from '@react-three/fiber'

import {
  Line,
} from '@react-three/drei'

import { useFrame, useThree } from '@react-three/fiber'
import { Mesh, Group, Color, Vector3 } from 'three'

export const GroundPlane = () => {
  
    return (
      <>
        <group position={[0, -325, 0]}>
          <gridHelper args={[2000, 25, "blue", "#2A2E32"]} />
        </group >
      </>
    )
  }
  
