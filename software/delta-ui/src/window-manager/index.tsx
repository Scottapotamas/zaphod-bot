import { app, BrowserWindow, Menu } from 'electron'
import { join as pathJoin } from 'path'
import { format as formatUrl } from 'url'

import {
  installDevTools,
  setupElectricUIHandlers,
  setupSettingsListenersWindowManager,
  setupSettingsPathing,
  setSettingFromWinManager,
  getSettingFromWinManager,
  fetchSystemDarkModeFromWinManager,
} from '@electricui/utility-electron'

const isDevelopment = process.env.NODE_ENV !== 'production'

// Setup persistent settings helpers
setupSettingsPathing()
setupSettingsListenersWindowManager()

// global reference to mainWindows (necessary to prevent window from being garbage collected)
let mainWindows: Array<BrowserWindow> = []

function createMainWindow() {
  const window = new BrowserWindow({
    webPreferences: {
      nodeIntegration: true,
    },
    minHeight: 680,
    minWidth: 1200,
    height: 680,
    width: 1200,
    title: 'Electric UI',
  })

  if (isDevelopment) {
    window.webContents.openDevTools()
  }

  if (isDevelopment) {
    window.loadURL(
      `http://localhost:${process.env.ELECTRICUI_BUILD_TOOLS_PORT}`,
    )
  } else {
    window.loadURL(
      formatUrl({
        pathname: pathJoin(__dirname, '../renderer/index.html'),
        protocol: 'file',
        slashes: true,
      }),
    )
  }

  window.on('closed', () => {
    mainWindows = mainWindows.filter(win => win !== window) // remove the window from the list
  })

  // since right now we only support one window, if it gets closed, kill the app.
  window.on('close', event => {
    // Exit the app, don't 'quit', quit asks each window to close.
    app.exit()
  })

  window.webContents.on('devtools-opened', () => {
    window.focus()
    setImmediate(() => {
      window.focus()
    })
  })

  installDevTools()

  return window
}

// Since we always have a window open, this always fails, so lets just terminate without asking permission.
app.on('before-quit', event => {
  event.preventDefault()
  app.exit()
})

// quit application when all windows are closed
// this never actually gets called since our transport window is always open, just hidden.
app.on('window-all-closed', () => {
  // on macOS it is common for applications to stay open until the user explicitly quits
  if (process.platform !== 'darwin') {
    app.quit()
  }
})

app.on('activate', () => {
  // on macOS it is common to re-create a window even after all windows have been closed
  if (mainWindows.length === 0) {
    mainWindows.push(createMainWindow()) // add a new window
  }
})

// create main BrowserWindow when electron is ready
app.on('ready', () => {
  mainWindows.push(createMainWindow()) // add a new window

  // A secure policy to prevent running scripts external to this browser.

  /*
  session.defaultSession.webRequest.onHeadersReceived((details, callback) => {
    callback({
      responseHeaders: {
        ...details.responseHeaders,
        'Content-Security-Policy': [
          "default-src 'self'",
        ],
      },
    })
  })
  */
})

setupElectricUIHandlers(mainWindows)

/**
 * Setup our main menu bar
 */

const isMac = process.platform === 'darwin'

const template = [
  // { role: 'appMenu' }
  ...(isMac
    ? [
        {
          label: app.getName(),
          submenu: [
            { role: 'about', label: 'About Electric UI' },
            { type: 'separator' },
            { role: 'services' },
            { type: 'separator' },
            { role: 'hide' },
            { role: 'hideothers' },
            { role: 'unhide' },
            { type: 'separator' },
            { role: 'quit' },
          ],
        },
      ]
    : []),
  // { role: 'fileMenu' }
  {
    label: 'File',
    submenu: [{ role: 'quit', label: 'Quit Electric UI' }], // isMac ? { role: 'close' } : { role: 'quit' }
  },
  // { role: 'editMenu' }
  {
    label: 'Edit',
    submenu: [
      { role: 'undo' },
      { role: 'redo' },
      { type: 'separator' },
      { role: 'cut' },
      { role: 'copy' },
      { role: 'paste' },
      ...(isMac
        ? [
            { role: 'pasteAndMatchStyle' },
            { role: 'delete' },
            { role: 'selectAll' },
            { type: 'separator' },
            {
              label: 'Speech',
              submenu: [{ role: 'startspeaking' }, { role: 'stopspeaking' }],
            },
          ]
        : [{ role: 'delete' }, { type: 'separator' }, { role: 'selectAll' }]),
    ],
  },
  // { role: 'viewMenu' }
  {
    label: 'View',
    submenu: [
      // Dark mode!
      {
        label: 'Toggle Dark Mode',
        click: () => {
          const userDark = getSettingFromWinManager('darkMode', null)

          if (userDark === null) {
            // if the user mode is not set, set the dark mode to the opposite of the current system dark mode
            const sys = fetchSystemDarkModeFromWinManager()

            setSettingFromWinManager('darkMode', !sys)
            return
          }

          setSettingFromWinManager('darkMode', !userDark)
        },
      },
      {
        label: 'Use system dark mode',
        click: () => {
          setSettingFromWinManager('darkMode', null)
        },
      },

      { type: 'separator' },
      { role: 'reload' },
      { role: 'forcereload' },
      { role: 'toggledevtools' },
      { type: 'separator' },
      { role: 'resetzoom' },
      { role: 'zoomin' },
      { role: 'zoomout' },
      { type: 'separator' },
      { role: 'togglefullscreen' },
    ],
  },
  // { role: 'windowMenu' }
  {
    label: 'Window',
    submenu: [
      { role: 'minimize' },
      { role: 'zoom' },
      ...(isMac
        ? [
            { type: 'separator' },
            { role: 'front' },
            { type: 'separator' },
            { role: 'window' },
          ]
        : [{ role: 'close' }]),
    ],
  },
  {
    role: 'help',
    submenu: [
      {
        label: 'Learn More about Electric UI',
        click() {
          require('electron').shell.openExternal('https://electricui.com')
        },
      },
    ],
  },
]

const menu = Menu.buildFromTemplate(
  template as Electron.MenuItemConstructorOptions[],
)
Menu.setApplicationMenu(menu)
