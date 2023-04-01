import { Icon, IconName } from '@blueprintjs/core'
import React from 'react'
import { getSetting } from './state'
import { Button, MenuItem } from '@blueprintjs/core'
import { ItemRenderer, Select } from '@blueprintjs/select'
import { MATERIALS } from '../optimiser/materials/utilities'
import { getDefaultJSONForType, MaterialJSON } from '../optimiser/material'
import { IconNames } from '@blueprintjs/icons'
import { ColorMaterialEditor, ColorMaterialJSON } from './../optimiser/materials/Color'
import { ColorRampMaterialEditor, ColorRampMaterialJSON } from './../optimiser/materials/ColorRamp'
import { InvisibleMaterialEditor, InvisibleMaterialJSON } from './../optimiser/materials/Invisible'
import { VelocityMaterialEditor, VelocityMaterialJSON } from './../optimiser/materials/Velocity'
import { RandomColorMaterialEditor, RandomColorMaterialJSON } from '../optimiser/materials/Random'
import { FlickerMaterialEditor, FlickerMaterialJSON } from '../optimiser/materials/Flicker'
import { ZDepthMaterialEditor, ZDepthMaterialJSON } from '../optimiser/materials/ZDepth'
import { ZGradientMaterialEditor, ZGradientMaterialJSON } from '../optimiser/materials/ZGradient'
import { BlendMaterialEditor, BlendMaterialJSON } from '../optimiser/materials/Blend'
import { DurationMaterialEditor, DurationMaterialJSON } from '../optimiser/materials/Duration'
import { FlippedMaterialEditor, FlippedMaterialJSON } from '../optimiser/materials/Flipped'

export interface MaterialOption {
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
    materialType: MATERIALS.DURATION,
    text: 'Duration as Color',
    icon: IconNames.TIME,
  },
  {
    materialType: MATERIALS.FLIPPED,
    text: 'Flipped movements',
    icon: IconNames.SWAP_HORIZONTAL,
  },
  {
    materialType: MATERIALS.INVISIBLE,
    text: 'Invisible on Delta',
    icon: IconNames.EYE_OFF,
  },
  {
    materialType: MATERIALS.RANDOM,
    text: 'Random Color',
    icon: IconNames.RANDOM,
  },
  {
    materialType: MATERIALS.FLICKER,
    text: 'Flicker',
    icon: IconNames.FLAME,
  },
  {
    materialType: MATERIALS.Z_GRADIENT,
    text: 'Z-Gradient',
    icon: IconNames.MOUNTAIN,
  },
  {
    materialType: MATERIALS.Z_DEPTH,
    text: 'Z-Depth',
    icon: IconNames.CAMERA,
  },
  {
    materialType: MATERIALS.BLEND,
    text: 'Blend',
    icon: IconNames.RESOLVE,
  },
]

const materialOptionsNoDefault = materialOptions.filter(option => option.materialType !== MATERIALS.DEFAULT)

const MaterialSelectorSelect = Select.ofType<MaterialOption>()

const renderMaterialOption: ItemRenderer<MaterialOption> = (materialOption, { handleClick, modifiers }) => {
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
  gridTemplateRows: 'max-content 1fr',
  marginBottom: 10,

  background: '#293742',
  padding: 10,
  borderRadius: 5,
}

export interface MaterialSelectorProps {
  selectedType: MATERIALS | null
  onChange: (option: MaterialOption) => void
  disallowDefault?: boolean
}

export function MaterialSelector(props: MaterialSelectorProps) {
  const selectedOpt = materialOptions.find(opt => opt.materialType === props.selectedType) ?? null

  const options = props.disallowDefault ? materialOptionsNoDefault : materialOptions

  return (
    <MaterialSelectorSelect
      items={options}
      itemRenderer={renderMaterialOption}
      onItemSelect={props.onChange}
      filterable={false}
      fill
    >
      {/* children become the popover target; render value here */}
      <Button
        text={selectedOpt?.text ?? 'Select Material Override'}
        icon={selectedOpt?.icon ?? IconNames.CLEAN}
        rightIcon="double-caret-vertical"
      />
    </MaterialSelectorSelect>
  )
}

type JsonMutator<T> = (writer: (json: T) => void) => void

export interface MaterialEditorProps {
  json: MaterialJSON | null
  updateJson: JsonMutator<MaterialJSON>
}

export function MaterialEditor(props: MaterialEditorProps) {
  let MaterialOverrideEditor: React.ReactNode = null

  switch (props.json?.type) {
    case MATERIALS.COLOR:
      MaterialOverrideEditor = (
        <ColorMaterialEditor
          json={props.json as ColorMaterialJSON}
          mutateJson={props.updateJson as JsonMutator<ColorMaterialJSON>}
        />
      )
      break
    case MATERIALS.COLOR_RAMP:
      MaterialOverrideEditor = (
        <ColorRampMaterialEditor
          json={props.json as ColorRampMaterialJSON}
          mutateJson={props.updateJson as JsonMutator<ColorRampMaterialJSON>}
        />
      )
      break
    case MATERIALS.VELOCITY:
      MaterialOverrideEditor = (
        <VelocityMaterialEditor
          json={props.json as VelocityMaterialJSON}
          mutateJson={props.updateJson as JsonMutator<VelocityMaterialJSON>}
        />
      )
      break
      case MATERIALS.DURATION:
        MaterialOverrideEditor = (
          <DurationMaterialEditor
            json={props.json as DurationMaterialJSON}
            mutateJson={props.updateJson as JsonMutator<DurationMaterialJSON>}
          />
        )
        break
        case MATERIALS.FLIPPED:
          MaterialOverrideEditor = (
            <FlippedMaterialEditor
              json={props.json as FlippedMaterialJSON}
              mutateJson={props.updateJson as JsonMutator<FlippedMaterialJSON>}
            />
          )
          break
    // case MATERIALS.MIX:
    //   MaterialOverrideEditor = (
    //     <MixMaterialEditor objectID={objectID} json={json as MixMaterialJSON} />
    //   )
    //   break
    case MATERIALS.INVISIBLE:
      MaterialOverrideEditor = (
        <InvisibleMaterialEditor
          json={props.json as InvisibleMaterialJSON}
          mutateJson={props.updateJson as JsonMutator<InvisibleMaterialJSON>}
        />
      )
      break
    // case MATERIALS.DELAY:
    //   MaterialOverrideEditor = (
    //     <DelayMaterialEditor objectID={objectID} json={json as DelayMaterialJSON} />
    //   )
    // break
    case MATERIALS.RANDOM:
      MaterialOverrideEditor = (
        <RandomColorMaterialEditor
          json={props.json as RandomColorMaterialJSON}
          mutateJson={props.updateJson as JsonMutator<RandomColorMaterialJSON>}
        />
      )
      break
    case MATERIALS.FLICKER:
      MaterialOverrideEditor = (
        <FlickerMaterialEditor
          json={props.json as FlickerMaterialJSON}
          mutateJson={props.updateJson as JsonMutator<FlickerMaterialJSON>}
        />
      )
      break
    case MATERIALS.Z_GRADIENT:
      MaterialOverrideEditor = (
        <ZGradientMaterialEditor
          json={props.json as ZGradientMaterialJSON}
          mutateJson={props.updateJson as JsonMutator<ZGradientMaterialJSON>}
        />
      )
      break
    case MATERIALS.Z_DEPTH:
      MaterialOverrideEditor = (
        <ZDepthMaterialEditor
          json={props.json as ZDepthMaterialJSON}
          mutateJson={props.updateJson as JsonMutator<ZDepthMaterialJSON>}
        />
      )
      break
    case MATERIALS.BLEND:
      MaterialOverrideEditor = (
        <BlendMaterialEditor
          json={props.json as BlendMaterialJSON}
          mutateJson={props.updateJson as JsonMutator<BlendMaterialJSON>}
        />
      )
      break

    default:
      break
  }

  return MaterialOverrideEditor === null ? null : <div style={{ marginTop: 10 }}>{MaterialOverrideEditor}</div>
}

export function calculateInitialMaterialJSON(materialType: MATERIALS, objectID: string | null) {
  if (objectID) {
    const renderableOriginalJSON = getSetting(state => {
      const renderable = state.renderablesByFrame[state.viewportFrame].find(renderable =>
        renderable.getOriginalMaterialJSON(objectID),
      )

      if (renderable) return renderable.getOriginalMaterialJSON(objectID)

      return null
    })

    if (renderableOriginalJSON && renderableOriginalJSON.type === materialType) {
      return renderableOriginalJSON
    }
  }

  return getDefaultJSONForType(materialType)
}

const spacerStyle = {
  border: '2px solid #222d35',
  width: 'calc(100% + 20px)',
  marginTop: 10,
  marginBottom: 10,
  marginLeft: -10,
  marginRight: -10,
}

export function Spacer() {
  return <div style={spacerStyle} />
}
