import { Card } from '@blueprintjs/core'
import React, { useCallback, useState } from 'react'
import { useTreeStore } from './RenderableTree'
import { useStore } from './state'

export function MaterialEditor() {
  const selectedItemID = useTreeStore(state => state.selectedItemID)

  // Hide this section if no items are selected
  if (selectedItemID === null) return null

  const movements = useStore(state => {
    const movements = (
      state.orderedMovementsByFrame[state.viewportFrame] ?? []
    ).filter(movement => movement.objectID === selectedItemID)
    return movements
  })

  return <Card>{selectedItemID}</Card>
}
