import 'mocha'

import * as chai from 'chai'
import * as fs from 'fs'
import * as path from 'path'

import { Application } from 'spectron'

const assert = chai.assert

const appPath = path.join(__dirname, '..')

let app: Application

const electronPathCalculated = (require('electron') as unknown) as string

async function cycleToVisibleWindow() {
  console.log('Cycling to visible window')

  const handles = ((await app.client.windowHandles()) as any).value

  for (const handle of handles) {
    const window = app.client.window(handle)

    const url = await window.getUrl()

    console.log('Inspecting window', url)

    if (url.endsWith('renderer/index.html')) {
      return window
    }
  }
}

describe('Basic Integration Test', function () {
  this.timeout(30000)

  beforeEach(() => {
    app = new Application({
      path: electronPathCalculated,
      args: [appPath],
      chromeDriverArgs: [
        '--disable-dev-shm-usage',
        '--no-sandbox',
        // '--headless',
        // '--disable-gpu',
      ],
    })

    return app.start()
  })

  afterEach(() => {
    if (app && app.isRunning()) {
      return app.stop()
    }
  })

  it('Can take a screenshot', function () {
    return app.client.waitUntilWindowLoaded().then(async () => {
      const visibleWindow = await cycleToVisibleWindow()

      console.log('Visible window:', visibleWindow)

      const title = await app.browserWindow.getTitle()

      console.log('Title of that index:', title)

      // Set the window size
      app.browserWindow.setSize(1920, 1080, false)

      // Take a screenshot for validation
      const screenshotPath = path.join(__dirname, 'screenshots')
      await app.browserWindow.capturePage().then(imageBuffer => {
        return fs.promises.writeFile(
          path.join(screenshotPath, 'ui.png'),
          imageBuffer,
        )
      })
    })
  })

  it('Found a device', function () {
    return app.client.waitUntilWindowLoaded().then(async () => {
      await cycleToVisibleWindow()

      const connectionList = app.client.$('.eui-connections-list')

      const connectionPageTextObj = await connectionList.getText()
      const connectionPageText = connectionPageTextObj.toString()

      // Print the connection page text to console
      console.log(connectionPageText)

      // Make sure the connection page contains a device
      assert.isTrue(connectionPageText.includes('Device information!'))
    })
  })
})
