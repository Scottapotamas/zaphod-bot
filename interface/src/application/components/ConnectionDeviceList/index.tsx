import './connection-page.css'

import {
  Button,
  Card,
  Classes,
  Elevation,
  Icon,
  NonIdealState,
  Overlay,
  ProgressBar,
  Tag,
} from '@blueprintjs/core'
import {
  CONNECTION_STATE,
  ConnectionMetadata,
  DeviceMetadata,
} from '@electricui/core'
import { Cell, Grid } from 'styled-css-grid'
import {
  Connect,
  ConnectionMetadata as ConnectionMetadataComponent,
  ConnectionState,
  ConsecutivePollFailureMessage,
  DeviceConnectionHashes,
  DeviceIDList,
  DeviceMetadata as DeviceMetadataComponent,
  Disconnect,
  Poll,
} from '@electricui/components-core'
import posed, { PoseGroup } from 'react-pose'

import { IconNames } from '@blueprintjs/icons'
import React from 'react'

const NoFoundDiv = posed.div({
  enter: { y: 0, opacity: 1 },
  exit: { y: 0, opacity: 0 },
})

const DeviceCard = posed.div({
  enter: { y: 0, opacity: 1 },
  exit: { y: 0, opacity: 0 },
  init: {
    marginBottom: '2em',
  },
})

const DeviceInnerCard = posed.div({
  CONNECTING: {
    borderRight: '2px solid rgba(72, 175, 240, 1)',
  },
  CONNECTED: {
    borderRight: '2px solid rgba(61, 204, 145, 1)',
  },
  DISCONNECTING: {
    borderRight: '2px solid rgba(255, 115, 115, 0.5)',
  },
  DISCONNECTED: {
    borderRight: '2px solid rgba(61, 204, 145, 0)',
  },
  DISCOVERING: {
    borderRight: '2px solid rgba(194, 116, 194, 0.5)',
  },
})

type ConnectionsProps = {
  maxWidth: number
  preConnect: (deviceID: string) => void
  postHandshake: (deviceID: string) => void
  onFailure: (deviceID: string) => void
  style: React.CSSProperties
}

class Connections extends React.Component<ConnectionsProps> {
  static defaultProps = {
    maxWidth: 400,
  }

  renderNoDevices = () => {
    return (
      <NoFoundDiv key="nodevices">
        <NonIdealState
          title="No devices found"
          description={
            <ConsecutivePollFailureMessage>
              {noIncreases =>
                noIncreases >= 3 ? (
                  <div>Hey maybe try something else?</div>
                ) : null
              }
            </ConsecutivePollFailureMessage>
          }
        />
      </NoFoundDiv>
    )
  }

  renderDeviceInternal = (deviceID: string, metadata: DeviceMetadata) => {
    let header = (
      <h3 className={`${Classes.HEADING} ${Classes.SKELETON}`}>
        Placeholder name
      </h3>
    )

    if (metadata.name) {
      header = <h3 className={Classes.HEADING}>{metadata.name}</h3>
    }
    if (metadata.type) {
      header = <h3 className={Classes.HEADING}>{metadata.type}</h3>
    }

    return (
      <React.Fragment>
        {header}
        <p>Device ID: {deviceID}</p>
      </React.Fragment>
    )
  }

  cardClick = (
    deviceID: string,
    numConnectionHashes: number,
    connectionRequested: boolean,
    connectToDevice: () => void,
  ) => {
    if (numConnectionHashes === 0) {
      // we're a ghost card, don't do anything
      return
    }

    if (!connectionRequested) {
      // we haven't requested a connection yet, so do so.
      this.props.preConnect(deviceID)
      connectToDevice()
      return
    }

    // we've already requested a connection
    this.props.postHandshake(deviceID)
  }

  renderDeviceID = (deviceID: string, metadata: DeviceMetadata) => {
    const { maxWidth } = this.props

    console.log('rendering device card', deviceID, metadata)

    if (metadata.type && metadata.type === 'Camera') {
      return null
    }

    return (
      <DeviceCard
        key={deviceID}
        style={{ maxWidth: maxWidth, margin: '0 auto' }}
      >
        <DeviceConnectionHashes deviceID={deviceID}>
          {(connectionHashes: Array<string>) => (
            <Connect
              deviceID={deviceID}
              preConnect={() => {
                this.props.preConnect(deviceID)
              }}
              postHandshake={() => {
                this.props.postHandshake(deviceID)
              }}
              onFailure={() => {
                this.props.onFailure(deviceID)
              }}
            >
              {(
                connectOnClick: () => void,
                connectionRequested: boolean,
                connectionState: CONNECTION_STATE,
                deviceManagerReady: boolean,
              ) => (
                <Disconnect deviceID={deviceID}>
                  {(disconnectOnClick: () => void) => {
                    let deviceInnerCardPose

                    if (
                      !connectionRequested &&
                      (connectionState === 'CONNECTED' ||
                        connectionState === 'CONNECTING')
                    ) {
                      deviceInnerCardPose = 'DISCOVERING'
                    } else {
                      deviceInnerCardPose = connectionState
                    }

                    return (
                      <React.Fragment>
                        <DeviceInnerCard
                          className={
                            connectionHashes.length === 0
                              ? 'bp3-card bp3-elevation-0 disabled-card'
                              : 'bp3-card bp3-interactive bp3-elevation-1'
                          }
                          pose={deviceInnerCardPose}
                          style={{
                            padding: 0,
                          }}
                        >
                          {/* The disconnect button */}
                          {connectionRequested &&
                          connectionState === 'CONNECTED' ? (
                            <Button
                              intent="danger"
                              onClick={disconnectOnClick}
                              style={{
                                position: 'absolute',
                                right: 0,
                                marginRight: '-2em',
                              }}
                            >
                              <Icon icon={IconNames.CROSS} />
                            </Button>
                          ) : null}

                          {/* The connection card */}
                          <div
                            onClick={() =>
                              this.cardClick(
                                deviceID,
                                connectionHashes.length,
                                connectionRequested,
                                connectOnClick,
                              )
                            }
                            style={{
                              padding: 20,
                            }}
                          >
                            <Grid columns={2} alignItems="end">
                              <Cell>
                                {this.renderDeviceInternal(deviceID, metadata)}
                              </Cell>
                              {connectionHashes.length === 0 ? null : (
                                <Cell
                                  style={{
                                    textAlign: 'right',
                                  }}
                                >
                                  <div
                                    style={{
                                      display: 'flex',
                                      justifyContent: 'space-between',
                                      flexDirection: 'column',
                                      height: '100%',
                                    }}
                                  >
                                    <div>
                                      {connectionHashes.map(connectionHash => (
                                        <ConnectionState
                                          connectionHash={connectionHash}
                                          key={connectionHash}
                                        >
                                          {(
                                            connectionState: CONNECTION_STATE,
                                          ) => (
                                            <ConnectionMetadataComponent
                                              connectionHash={connectionHash}
                                            >
                                              {(
                                                metadata: ConnectionMetadata,
                                              ) => (
                                                <Tag
                                                  round
                                                  intent={
                                                    connectionRequested &&
                                                    connectionState ===
                                                      'CONNECTED'
                                                      ? 'success'
                                                      : 'none'
                                                  }
                                                  style={{ marginLeft: 4 }}
                                                >
                                                  {metadata.name}
                                                </Tag>
                                              )}
                                            </ConnectionMetadataComponent>
                                          )}
                                        </ConnectionState>
                                      ))}
                                    </div>
                                  </div>
                                </Cell>
                              )}
                            </Grid>
                          </div>
                        </DeviceInnerCard>
                      </React.Fragment>
                    )
                  }}
                </Disconnect>
              )}
            </Connect>
          )}
        </DeviceConnectionHashes>
      </DeviceCard>
    )
  }

  render() {
    const { maxWidth, style } = this.props

    return (
      <div
        style={{
          margin: '0 auto',
          maxWidth: maxWidth + 50,
          position: 'relative',
          ...style,
        }}
      >
        <DeviceIDList>
          {(deviceIDs: Array<string>) => (
            <React.Fragment>
              <PoseGroup>
                {deviceIDs.length === 0
                  ? this.renderNoDevices() // render no devices found, otherwise render each of them
                  : deviceIDs.map(deviceID => (
                      <DeviceMetadataComponent
                        deviceID={deviceID}
                        key={deviceID}
                      >
                        {(metadata: DeviceMetadata) =>
                          this.renderDeviceID(deviceID, metadata)
                        }
                      </DeviceMetadataComponent>
                    ))}
              </PoseGroup>

              <Poll>
                {(onClick, polling, deviceManagerReady) => (
                  <Button
                    onClick={onClick}
                    disabled={polling || !deviceManagerReady}
                    fill
                    style={{
                      width: maxWidth,
                      margin: '2em auto 0 auto',
                    }}
                    loading={polling}
                  >
                    Refresh
                  </Button>
                )}
              </Poll>
            </React.Fragment>
          )}
        </DeviceIDList>
      </div>
    )
  }
}

export { Connections }
