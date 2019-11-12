import React, { Component, useState } from 'react'
import {
  Button,
  Dialog,
  Classes,
  Intent,
  IPanelProps,
  PanelStack,
  ButtonGroup,
} from '@blueprintjs/core'
import { useDarkMode } from '@electricui/components-desktop'

import './calibration-modal.css'
import {
  Slider,
  Button as ElectricButton,
  Switch,
} from '@electricui/components-desktop-blueprint'

const modalStartPage = <span></span>

interface PassedState {
  persistentThing: number
}

const InfoPage = (props: IPanelProps & PassedState) => {
  return (
    <>
      <div className={Classes.DIALOG_BODY}>
        <p>
          <strong>Before calibrating, make sure:</strong>
        </p>
        <ol>
          <li>A RGB LED is connected to the delta, and output is visible,</li>
          <li>You have a consistent colour measuring device/method,</li>
          <li>
            Ambient lighting will not skew calibrations <i>(dim the lights)</i>
          </li>
        </ol>
      </div>
      <div className={Classes.DIALOG_FOOTER}>
        <div className={Classes.DIALOG_FOOTER_ACTIONS}>
          <Button
            onClick={() => {
              props.openPanel({
                component: TunePage, // <- class or stateless function type
                props: {
                  enabled: true,
                  persistentThing: props.persistentThing,
                }, // <- Props to pass to it
                title: 'Colour Offsets', // <- appears in header and back button
              })
            }}
          >
            Start
          </Button>
        </div>
      </div>
    </>
  )
}

const TunePage = (props: IPanelProps & PassedState) => {
  return (
    <>
      <div className={Classes.DIALOG_BODY}>
        <p>
          Drag the channel sliders until you are satisfied that the LED is
          balanced.
        </p>
        <br />
        <div style={{ width: '200px', margin: '0 auto' }}>
          <Switch
            large
            innerLabel="OFF"
            innerLabelChecked="ON"
            unchecked={{
              hsv: {
                hue: 0,
                saturation: 0,
                lightness: 0,
                enable: 0,
              },
            }}
            checked={{
              hsv: {
                hue: 0,
                saturation: 0,
                lightness: 0.7,
                enable: 1,
              },
            }}
          >
            LED Control
          </Switch>
        </div>

        <div style={{ padding: '2em' }}>
          <Slider
            showTrackFill={false}
            writer={values => ({
              ledset: {
                offset_red: values.red_handle,
              },
            })}
            min={-1}
            max={1}
            stepSize={0.02}
            labelStepSize={0.25}
            labelRenderer={val => `${Math.round(val * 100)}%`}
          >
            <Slider.Handle
              name="red_handle"
              accessor={state => state.ledset.offset_red}
            />
          </Slider>
          <br />
          <Slider
            showTrackFill={false}
            writer={values => ({
              ledset: {
                offset_green: values.green_handle,
              },
            })}
            min={-1}
            max={1}
            stepSize={0.02}
            labelStepSize={0.25}
            labelRenderer={val => `${Math.round(val * 100)}%`}
          >
            <Slider.Handle
              name="green_handle"
              accessor={state => state.ledset.offset_green}
            />
          </Slider>
          <br />
          <Slider
            showTrackFill={false}
            writer={values => ({
              ledset: {
                offset_blue: values.blue_handle,
              },
            })}
            min={-1}
            max={1}
            stepSize={0.02}
            labelStepSize={0.25}
            labelRenderer={val => `${Math.round(val * 100)}%`}
          >
            <Slider.Handle
              name="blue_handle"
              accessor={state => state.ledset.offset_blue}
            />
          </Slider>
        </div>
      </div>

      <div className={Classes.DIALOG_FOOTER}>
        <div className={Classes.DIALOG_FOOTER_ACTIONS}>
          <Button
            onClick={() => {
              props.openPanel({
                component: ValidatePage, // <- class or stateless function type
                props: {
                  enabled: true,
                  persistentThing: props.persistentThing,
                }, // <- Props to pass to it
                title: 'Colour Check', // <- appears in header and back button
              })
            }}
          >
            Check
          </Button>
        </div>
      </div>
    </>
  )
}

const ValidatePage = (props: IPanelProps & PassedState) => {
  return (
    <>
      <div className={Classes.DIALOG_BODY}>
        <p>Validate white performance at different brightnesses</p>
        <br />
        <ButtonGroup large fill>
          <ElectricButton
            writer={{
              hsv: {
                hue: 0,
                saturation: 0,
                lightness: 0.1,
              },
            }}
          >
            Dim White
          </ElectricButton>
          <ElectricButton
            writer={{
              hsv: {
                hue: 0,
                saturation: 0,
                lightness: 0.5,
              },
            }}
          >
            Half White
          </ElectricButton>
          <ElectricButton
            writer={{
              hsv: {
                hue: 0,
                saturation: 0,
                lightness: 1.0,
              },
            }}
          >
            Blinding White
          </ElectricButton>
        </ButtonGroup>
        <br />
        <br />
        <ButtonGroup large fill>
          <ElectricButton
            intent="danger"
            writer={{
              hsv: {
                hue: 0,
                saturation: 1.0,
                lightness: 0.5,
              },
            }}
          >
            Red
          </ElectricButton>
          <ElectricButton
            intent="success"
            writer={{
              hsv: {
                hue: 0.33,
                saturation: 1,
                lightness: 0.5,
              },
            }}
          >
            Green
          </ElectricButton>
          <ElectricButton
            intent="primary"
            writer={{
              hsv: {
                hue: 0.66,
                saturation: 0,
                lightness: 0.5,
              },
            }}
          >
            Blue
          </ElectricButton>
        </ButtonGroup>
      </div>

      <div className={Classes.DIALOG_FOOTER}>
        <div className={Classes.DIALOG_FOOTER_ACTIONS}>
          <Button onClick={() => setModalOpen(false)}>Save</Button>
        </div>
      </div>
    </>
  )
}

const LEDCalibrationModal = () => {
  const [isOpen, setModalOpen] = useState(false)
  const isDarkMode = useDarkMode()

  const [persistentState, setExamplePersistentState] = useState(3)

  return (
    <>
      <Button onClick={() => setModalOpen(true)}>Calibrate Whitepoint</Button>
      <Dialog
        onClose={() => setModalOpen(false)}
        isOpen={isOpen}
        className={isDarkMode ? 'bp3-dark' : ''}
        canOutsideClickClose={false}
      >
        <PanelStack
          initialPanel={{
            component: InfoPage,
            title: 'Info',
            props: {
              persistentThing: persistentState,
            },
          }}
          className="panel-fix"
        />
      </Dialog>
    </>
  )
}

export default LEDCalibrationModal
