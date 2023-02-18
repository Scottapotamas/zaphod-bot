import React from 'react'
import { Colors, Tag } from '@blueprintjs/core'

import { Accessor } from '@electricui/components-core'
import { Queryable } from '@electricui/timeseries'

import {
  ChartContainer,
  LineChart,
  RealTimeDomain,
  DataSourcePrinter,
} from '@electricui/components-desktop-charts'

import { OutlineCard } from 'src/application/components/OutlineCard'

interface HeadingProps {
  text: string
  headingColor: string
}

const Heading = (props: HeadingProps) => {
  // TODO how to handle a null/empty string formatting?
  // Layout should use the top bar regardless for visual consistency?

  return (
    <Tag minimal fill>
      <h2
        style={{
          textAlign: 'center',
          opacity: '0.8',
          marginBlock: '0.25em',
          color: `${props.headingColor}`,
        }}
      >
        {props.text.toUpperCase()}
      </h2>
    </Tag>
  )
}

interface OverlayValueProps {
  valueColor: string
  datasource: Queryable
  accessor: Accessor
}

const OverlayValue = (props: OverlayValueProps) => {
  return (
    <div
      style={{
        position: 'relative',
        top: '50%',
        transform: 'translateY(-50%)',
        textAlign: 'center',
        color: `${props.valueColor}`,
        fontWeight: 'bold',
        fontSize: '2em',
        textShadow: '0px 0px 5px black',
      }}
    >
      <DataSourcePrinter
        dataSource={props.datasource}
        accessor={props.accessor}
        precision={0}
      />
    </div>
  )
}

interface SparklineProps {
  lineColor: string
  datasource: Queryable
  accessor: Accessor
  min?: number
  max?: number
}

const Sparkline = (props: SparklineProps) => {
  const minValue = props.min ? props.min : 0
  const maxValue = props.max ? props.max : 100

  return (
    <div
      style={{
        width: '100%',
        height: '100%',
        position: 'absolute',
        top: 0,
        left: 0,
        zIndex: -1,
      }}
    >
      <ChartContainer height="100%" width="100%">
        <LineChart
          dataSource={props.datasource}
          accessor={props.accessor}
          color={props.lineColor}
          lineWidth={3}
        />
        <RealTimeDomain
          window={30_000}
          yMinSoft={minValue}
          yMaxSoft={maxValue}
        />
      </ChartContainer>
    </div>
  )
}

interface SparklineProps {
  title?: string
  color?: string
  datasource: Queryable
  accessor: Accessor // TODO allow optional use?
  drawValue?: boolean // this requires the datasource/accessor when enabled - how to strictly-type that?
  min?: number
  max?: number
}

export const SparkTag = (props: SparklineProps) => {
  const color: string = props.color ? props.color : Colors.GRAY3
  const heading: string = props.title ? props.title : ' '

  return (
    <>
      <OutlineCard>
        <Heading text={heading} headingColor={color} />

        <div style={{ position: 'relative', height: '150px' }}>
          <Sparkline
            lineColor={color}
            datasource={props.datasource}
            accessor={props.accessor}
            min={props.min}
            max={props.max}
          />

          {/* Optionally display the value over the sparkline */}
          {props.drawValue ? (
            <OverlayValue
              valueColor={color}
              datasource={props.datasource}
              accessor={props.accessor}
            />
          ) : (
            <></>
          )}
        </div>
      </OutlineCard>
    </>
  )
}
