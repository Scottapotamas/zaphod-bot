import { Message, Sink, Transport } from '@electricui/core'
import * as GPhoto from '@typedproject/gphoto2-driver'

import fs from 'fs'

import { promisify } from 'util'
const writeFileAsync = promisify(fs.writeFile)

const dcraw = require('dcraw')

const dTransport = require('debug')('electricui-device-camera:transport')

export interface CameraTransportOptions {
  port: string
  transportKey?: string
  GPhoto: typeof GPhoto
}

class CameraWriteSink extends Sink {
  callback: (message: Message) => Promise<any>

  constructor(callback: (message: Message) => Promise<any>) {
    super()
    this.callback = callback
  }

  receive(message: Message) {
    return this.callback(message)
  }
}

export default class CameraTransport extends Transport {
  GPhoto: typeof GPhoto
  camera: GPhoto.Camera | null = null
  port: string

  constructor(options: CameraTransportOptions) {
    super(options)

    this.port = options.port
    this.GPhoto = options.GPhoto

    this.writeToDevice = this.writeToDevice.bind(this)

    this.writePipeline = new CameraWriteSink(this.writeToDevice)

    this.receiveData = this.receiveData.bind(this)
    this.error = this.error.bind(this)
    this.close = this.close.bind(this)
  }

  error(err: Error) {
    this.onError(err)
  }

  close(err: Error) {
    this.onClose(err)
  }

  /**
   * This never happens, the camera never tells us anything unless we explicitly ask for it.
   *
   * Except for when we do liveview, we receive a stream of the liveview in some format
   *
   */
  receiveData(message: Message) {
    // const message = new Message('event', chunk)
    // this.readPipeline.push(message)
  }

  async connect() {
    dTransport('Connecting to the camera at port', this.port)
    const cameraList = new this.GPhoto.CameraList().load()

    let camera: GPhoto.Camera | null = null

    for (const [index, device] of cameraList.toArray().entries()) {
      if (device.port === this.port) {
        const potentialCamera = cameraList.getCamera(index)

        if (typeof potentialCamera !== 'undefined') {
          camera = potentialCamera
          break
        }
      }
    }

    if (camera === null) {
      return Promise.reject(
        "Couldn't 'connect' to the camera, it doesn't seem to exist anymore.",
      )
    }

    this.camera = camera
  }

  async disconnect() {
    if (this.camera === null) {
      return
    }

    this.camera.close()
    this.camera = null
  }

  cInspect = async (message: Message) => {
    if (this.camera === null) {
      throw new Error('Camera is disconnected')
    }

    const widget = this.camera.widgets.get(message.payload)

    console.log('choices', widget.choices)
    console.log('info', widget.info)
    console.log('path', widget.path)
    console.log('pointer', widget.pointer)
    console.log('range', widget.range)
    console.log('readonly', widget.readonly)
    console.log('type', widget.type)
    console.log('value', widget.value)
  }

  cDumpInfo = async (message: Message) => {
    if (this.camera === null) {
      throw new Error('Camera is disconnected')
    }

    console.log('==> abilities.model', this.camera.getAbilities().model)
    console.log(
      '==> abilities.port',
      GPhoto.GPPortType[this.camera.getAbilities().port],
    )
    console.log(
      '==> abilities.status',
      GPhoto.GPCameraDriverStatus[this.camera.getAbilities().status],
    )
    console.log('==> abilities.id', this.camera.getAbilities().id)
    console.log('==> abilities.library', this.camera.getAbilities().library)
    console.log('==> abilities.operation', this.camera.getAbilities().operation)
    console.log(
      '==> abilities.fileOperations',
      GPhoto.GPCameraFileOperation[this.camera.getAbilities().fileOperations],
    )
    console.log(
      '==> abilities.folderOperations',
      GPhoto.GPCameraFolderOperation[
        this.camera.getAbilities().folderOperations
      ],
    )
    console.log('==> abilities.usbVendor', this.camera.getAbilities().usbVendor)
    console.log(
      '==> abilities.usbProduct',
      this.camera.getAbilities().usbProduct,
    )
    console.log('==> abilities.usbClass', this.camera.getAbilities().usbClass)
    console.log(
      '==> abilities.usbSubclass',
      this.camera.getAbilities().usbSubclass,
    )
    console.log(
      '==> abilities.deviceType',
      GPhoto.GPDeviceType[this.camera.getAbilities().deviceType],
    )
    console.log('==> abilities.speed', this.camera.getAbilities().speed)
  }

  /**
   * messageID: __capture
   * payload: {
   *   savePath: absolute savepath for the cr2
   * }
   */
  cCapture = async (message: Message) => {
    if (this.camera === null) {
      throw new Error('Camera is disconnected')
    }

    const savePath = message.payload.savePath

    if (savePath === '') {
      return
    }

    console.log('Starting camera capture')
    console.time('capture')
    const cameraFile = await this.camera.captureImageAsync()
    console.timeEnd('capture')
    console.log('Finished camera capture')

    if (!cameraFile) {
      console.error('Preview file was undefined')
      return
    }

    console.log('Fetching camera file')
    console.time('fetch')
    const { data } = await cameraFile.getDataAndSizeAsync('binary')
    console.timeEnd('fetch')
    console.log('Fetched camera file')

    // write the file
    await writeFileAsync(savePath, data)

    console.time('dcraw')
    const thumbnail = dcraw(data, { extractThumbnail: true })
    console.timeEnd('dcraw')

    const thumbMsg = new Message(
      '__thumbnail',
      Buffer.from(thumbnail).toString('base64'),
    )

    cameraFile.close()

    await this.readPipeline.push(thumbMsg)

    return
  }

  cStartBulb = async (message: Message) => {
    if (this.camera === null) {
      throw new Error('Camera is disconnected')
    }
    const startBulbWidgetRelease = this.camera.widgets.get(
      '/actions/eosremoterelease',
    )

    startBulbWidgetRelease.setValue('Press Full', true)
  }
  cEndBulb = async (message: Message) => {
    if (this.camera === null) {
      throw new Error('Camera is disconnected')
    }

    const endBulbWidgetRelease = this.camera.widgets.get(
      '/actions/eosremoterelease',
    )

    endBulbWidgetRelease.setValue('Release Full', true)
  }

  cDownloadPhoto = async (message: Message) => {
    if (this.camera === null) {
      throw new Error('Camera is disconnected')
    }

    if (this.camera.isClosed()) {
      console.warn('Camera is closed')
      return
    }

    const savePath = message.payload.savePath

    if (savePath === '') {
      return
    }

    console.log('new camera file path')
    const cameraFilePath = new GPhoto.CameraFilePath()

    // This is readonly?
    cameraFilePath.filename = 'fuck'

    console.log('newFileAsync', this.camera.pointer)
    const cameraFile = await cameraFilePath.newFileAsync(this.camera.pointer)

    if (!cameraFile) {
      console.warn('cameraFile was undefined', cameraFile)
      return
    }

    console.log('got the camera file?')
    console.log('cameraFile.pointer', cameraFile.pointer.toString('hex'))
    console.log('cameraFile.buffer', cameraFile.buffer.toString('hex'))

    console.log('cameraFilePath.filename', cameraFilePath.filename)
    console.log('cameraFilePath.path', cameraFilePath.path)

    const { data } = await cameraFile.getDataAndSizeAsync('binary')

    console.log('Fetched camera file!')

    // write the file
    await writeFileAsync(savePath, data)

    console.time('dcraw')
    const thumbnail = dcraw(data, { extractThumbnail: true })
    console.timeEnd('dcraw')

    const thumbMsg = new Message(
      '__thumbnail',
      Buffer.from(thumbnail).toString('base64'),
    )

    cameraFile.close()

    await this.readPipeline.push(thumbMsg)
  }

  async writeToDevice(message: Message) {
    dTransport('Parsing message for writing', message)

    if (this.camera === null) {
      throw new Error('Camera is disconnected')
    }

    // This shit errors, so catch everything
    try {
      // Do our magic stuff like handshake handling
      switch (message.messageID) {
        case '__handshake':
          this.camera.widgets.refresh()
          for (const [path, data] of this.camera.widgets.entries()) {
            // send the messages of the data up
            const msg = new Message(path, data.value)
            this.readPipeline.push(msg)
          }

          return

        /**
         * call this with a payload of '/actions/whatever'
         */
        case '__inspect':
          return this.cInspect(message)

        case '__dumpinfo':
          return this.cDumpInfo(message)

        case '__capture':
          return this.cCapture(message)

        case `__startbulb`:
          return this.cStartBulb(message)

        case `__endbulb`:
          return this.cEndBulb(message)

        /**
         * What in the fuck is this API
         */
        case `__download`:
          return this.cDownloadPhoto(message)

        default:
        // noop
      }

      // From here on, assume we're looking for a camera widget
      const widget = this.camera.widgets.get(message.messageID)

      if (message.metadata.query) {
        this.camera.widgets.refresh() // I guess we refresh everything?

        // Send the message up with the new value
        const msg = new Message(message.messageID, widget.value)
        this.readPipeline.push(msg)

        // Return the value with the promise
        return widget.value
      }

      // we're probably writing, so write

      widget.setValue(message.payload, true) // apply the changes
    } catch (e) {
      console.warn('Could not do the thing', message, e)
    }

    return
  }
}
