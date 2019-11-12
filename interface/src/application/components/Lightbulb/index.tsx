import React from 'react'
import brightOff from './bright-off.png'
import brightOn from './bright-on.png'
import darkOff from './dark-off.png'
import darkOn from './dark-on.png'

type LightbulbProps = {
  on: boolean
  darkMode: boolean
  style: React.CSSProperties
}

const Lightbulb = (props: LightbulbProps) => {
  let image = darkOn
  if (props.darkMode) {
    if (props.on) {
      image = darkOn
    } else {
      image = darkOff
    }
  } else {
    if (props.on) {
      image = brightOn
    } else {
      image = brightOff
    }
  }

  return <img src={image} style={props.style} />
}

export default Lightbulb
