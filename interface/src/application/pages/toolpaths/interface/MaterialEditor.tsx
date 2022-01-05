import { Card, Icon, IconName } from '@blueprintjs/core'
import React, { useCallback, useState } from 'react'
import { findNodeWithID, NodeID } from './RenderableTree'
import { incrementViewportFrameVersion, setSetting, useStore } from './state'
import { Button, MenuItem } from '@blueprintjs/core'
import { ItemRenderer, Select } from '@blueprintjs/select'
import { MATERIALS } from '../optimiser/materials/utilities'
import { getDefaultJSONForType, MaterialJSON } from '../optimiser/material'
import { IconNames } from '@blueprintjs/icons'
import {
  ColorMaterialEditor,
  ColorMaterialJSON,
} from './../optimiser/materials/Color'
import {
  ColorRampMaterialEditor,
  ColorRampMaterialJSON,
} from './../optimiser/materials/ColorRamp'
import {
  InvisibleMaterialEditor,
  InvisibleMaterialJSON,
} from './../optimiser/materials/Invisible'
import {
  VelocityMaterialEditor,
  VelocityMaterialJSON,
} from './../optimiser/materials/Velocity'

interface MaterialOption {
  materialType: MATERIALS
  text: string
  icon: IconName
}

const materialOptions: MaterialOption[] = [
  {
    materialType: MATERIALS.DEFAULT,
    text: 'No override',
    icon: IconNames.CLEAN,
  },
  {
    materialType: MATERIALS.COLOR,
    text: 'Single Color',
    icon: IconNames.TINT,
  },
  {
    materialType: MATERIALS.COLOR_RAMP,
    text: 'Color Ramp',
    icon: IconNames.EXCHANGE,
  },
  {
    materialType: MATERIALS.VELOCITY,
    text: 'Velocity as Color',
    icon: IconNames.DASHBOARD,
  },
  {
    materialType: MATERIALS.INVISIBLE,
    text: 'Invisible on Delta',
    icon: IconNames.EYE_OFF,
  },
]

// Select<T> is a generic component to work with your data types.
// In TypeScript, you must first obtain a non-generic reference:
const MaterialSelector = Select.ofType<MaterialOption>()

const renderMaterialOption: ItemRenderer<MaterialOption> = (
  materialOption,
  { handleClick, modifiers },
) => {
  return (
    <MenuItem
      active={modifiers.active}
      disabled={modifiers.disabled}
      labelElement={<Icon icon={materialOption.icon} />}
      key={materialOption.materialType}
      onClick={handleClick}
      text={materialOption.text}
    />
  )
}

const materialEditorStyle = {
  display: 'grid',
  gridTemplateColumns: 'max-content 1fr',
  gridGap: 20,
}

export function MaterialEditor() {
  const selectedItemID = useStore(state => state.treeStore.selectedItemID)

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
        const override = getDefaultJSONForType(option.materialType)

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

  // Hide this section if no items are selected
  if (selectedItemID === null) return null

  return (
    <Card style={materialEditorStyle}>
      <MaterialSelector
        items={materialOptions}
        itemRenderer={renderMaterialOption}
        onItemSelect={pickMaterialOverride}
        filterable={false}
      >
        {/* children become the popover target; render value here */}
        <Button
          text={
            materialOptions.find(
              opt => opt.materialType === materialTypeOverride?.type,
            )?.text ?? 'Select Material Override'
          }
          rightIcon="double-caret-vertical"
        />
      </MaterialSelector>
      {selectMaterialEditor(selectedItemID, materialTypeOverride)}
    </Card>
  )
}

function selectMaterialEditor(objectID: NodeID, json: MaterialJSON | null) {
  let MaterialOverrideEditor: React.ReactNode = null

  switch (json?.type) {
    case MATERIALS.COLOR:
      MaterialOverrideEditor = (
        <ColorMaterialEditor
          objectID={objectID}
          json={json as ColorMaterialJSON}
        />
      )
      break
    case MATERIALS.COLOR_RAMP:
      MaterialOverrideEditor = (
        <ColorRampMaterialEditor
          objectID={objectID}
          json={json as ColorRampMaterialJSON}
        />
      )
      break
    case MATERIALS.INVISIBLE:
      MaterialOverrideEditor = (
        <InvisibleMaterialEditor
          objectID={objectID}
          json={json as InvisibleMaterialJSON}
        />
      )
      break
    case MATERIALS.VELOCITY:
      MaterialOverrideEditor = (
        <VelocityMaterialEditor
          objectID={objectID}
          json={json as VelocityMaterialJSON}
        />
      )
      break

    default:
      break
  }

  return MaterialOverrideEditor
}
