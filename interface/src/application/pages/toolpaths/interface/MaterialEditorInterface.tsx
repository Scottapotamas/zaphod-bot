import React, { useCallback } from 'react'
import { findNodeWithID, NodeTypes } from './RenderableTree'
import { incrementViewportFrameVersion, setSetting, useStore } from './state'
import { MaterialJSON } from '../optimiser/material'
import { EmptyViewer } from './EmptyViewer'

import {
  MaterialSelector,
  MaterialOption,
  MaterialEditor,
  calculateInitialMaterialJSON,
  Spacer,
} from './MaterialSelector'

const materialEditorStyle = {
  display: 'grid',
  gridTemplateRows: 'max-content 1fr',
  marginBottom: 10,

  background: '#293742',
  padding: 10,
  borderRadius: 5,
}

export function MaterialEditorInterface() {
  const selectedItemID = useStore(state => state.treeStore.selectedItemID)

  const selectedItemType = useStore(state =>
    state.treeStore.selectedItemID
      ? findNodeWithID(state.treeStore.tree, state.treeStore.selectedItemID)
          ?.nodeData?.type ?? null
      : null,
  )

  const materialTypeOverride = useStore(state => {
    if (!state.treeStore.selectedItemID) {
      return null
    }

    const potentialOverride =
      state.visualisationSettings.objectMaterialOverrides[
        state.treeStore.selectedItemID
      ]

    if (!potentialOverride) return null

    return potentialOverride
  })

  const pickMaterialOverride = useCallback(
    (option: MaterialOption) => {
      if (!selectedItemID) return

      setSetting(state => {
        const override = calculateInitialMaterialJSON(
          option.materialType,
          selectedItemID as string | null,
        )

        if (!override) {
          // Remove the key
          delete state.visualisationSettings.objectMaterialOverrides[
            selectedItemID
          ]
        } else {
          // Set the override to the default
          state.visualisationSettings.objectMaterialOverrides[selectedItemID] =
            override
        }

        // Trigger an update
        incrementViewportFrameVersion(state)
      })
    },
    [selectedItemID],
  )

  // Pass through a convenience function to update the material's json
  const updateJson = useCallback(
    (writer: (json: MaterialJSON) => void) => {
      if (!selectedItemID) return

      setSetting(state => {
        writer(
          state.visualisationSettings.objectMaterialOverrides[selectedItemID],
        )

        incrementViewportFrameVersion(state)
      })
    },
    [selectedItemID],
  )

  // Hide this section if no items are selected
  if (selectedItemID === null) return null

  if (selectedItemType === NodeTypes.EMPTY) {
    return <EmptyViewer id={selectedItemID} />
  }

  return (
    <div style={materialEditorStyle}>
      <MaterialSelector
        selectedType={materialTypeOverride?.type ?? null}
        onChange={pickMaterialOverride}
      />
      {materialTypeOverride ? <Spacer /> : null}
      <MaterialEditor json={materialTypeOverride} updateJson={updateJson} />
    </div>
  )
}
