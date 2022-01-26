import { NodeID } from './RenderableTree'
import React, { useCallback } from 'react'
import { Column, Table2, Cell } from '@blueprintjs/table'
import { useStore } from './state'

export interface EmptyViewerProps {
  id: NodeID
}

export function EmptyViewer(props: EmptyViewerProps) {
  const data = useStore(state => state.arbitrary[state.viewportFrame])

  const renderer = useCallback(
    (rowIndex: number, columnIndex: number) => {
      if (columnIndex === 0) {
        return <Cell>{Object.keys(data)[rowIndex]}</Cell>
      } else {
        return <Cell>{Object.values(data)[rowIndex]}</Cell>
      }
    },
    [data],
  )

  return (
    <div style={{ height: 100 }}>
      <Table2 numRows={Object.keys(data).length} enableRowHeader={false}>
        <Column name="Key" cellRenderer={renderer} />
        <Column name="Value" cellRenderer={renderer} />
      </Table2>
    </div>
  )
}
