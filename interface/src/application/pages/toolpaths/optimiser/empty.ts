import { NodeInfo, NodeTypes } from '../interface/RenderableTree'
import { TreeNodeInfo } from '@blueprintjs/core'
import { Renderable } from './import'
import { Settings } from './settings'
import { IconNames } from '@blueprintjs/icons'

export interface EmptyToMovementsSettings {}

export class Empty {
  readonly type = 'empty'

  constructor(
    public name: string,
    public data: {
      [key: string]: any
    },
  ) {}

  public getObjectTree: () => TreeNodeInfo<NodeInfo> = () => {
    const node: TreeNodeInfo<NodeInfo> = {
      id: this.name,
      label: this.name,
      icon: IconNames.PIVOT,
      nodeData: {
        type: NodeTypes.EMPTY,
      },
    }

    return node
  }

  public toMovements = (settings: Settings) => {
    return []
  }
}

export function isEmpty(renderable: Renderable): renderable is Empty {
  return renderable.type === 'empty'
}

export interface EmptyJSON {
  type: 'empty'
  frame: number
  name: string
  data: {
    [key: string]: any
  }
}

export function importEmpty(json: EmptyJSON) {
  const empty = new Empty(json.name, json.data)

  return empty
}
