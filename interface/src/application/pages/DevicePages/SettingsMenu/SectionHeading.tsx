import React from 'react'

interface SectionHeadingProps {
  text: string
}

export const SectionHeading = (props: SectionHeadingProps) => {
  return (
    <h2 style={{ 
        textAlign: 'center', 
        fontWeight: 'initial', 
        opacity: '0.8' 
    }}>
      {props.text.toUpperCase()}
    </h2>
  )
}
