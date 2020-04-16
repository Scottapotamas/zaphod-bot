import 'source-map-support/register'

import { BrowserWindow, Menu, app } from 'electron'
import {
  ExternallyResolvedPromise,
  createdNewUIWindow,
  fetchSystemDarkModeFromWinManager,
  getElectricWindow,
  getSettingFromWinManager,
  installDevTools,
  setSettingFromWinManager,
  setupElectricUIHandlers,
  setupSettingsListenersWindowManager,
  setupSettingsPathing,
} from '@electricui/utility-electron'

import { format as formatUrl } from 'url'
import { join as pathJoin } from 'path'

const isDevelopment = process.env.NODE_ENV === 'development'

// Disallow process reuse
app.allowRendererProcessReuse = false

// Setup persistent settings helpers
setupSettingsPathing()
setupSettingsListenersWindowManager()

// global reference to mainWindows (necessary to prevent window from being garbage collected)
let mainWindows: Array<BrowserWindow> = []

const transportReady = setupElectricUIHandlers(mainWindows)

function createMainWindow() {
  const window = new BrowserWindow({
    webPreferences: {
      nodeIntegration: true,
      devTools: isDevelopment, // Only allow devTools in development mode
    },
    minHeight: 690,
    minWidth: 650,
    height: 690,
    width: 1200,
    title: 'Electric UI',
    backgroundColor: '#191b1d', // This needs to be set to something so the background on resize can be changed to match the dark / light mode theme
    show: false, // The window is shown once the transport manager is ready
  })

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

  if (isDevelopment) {
    // Only install extensions in developer mode
    installDevTools()
    window.webContents.openDevTools()
  }

  // Notify the handler that we have a new window
  createdNewUIWindow(window)

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
  const firstWindow = createMainWindow()
  mainWindows.push(firstWindow) // add a new window

  const firstWindowReady = new ExternallyResolvedPromise()
  firstWindow.once('ready-to-show', firstWindowReady.resolve)

  // Wait until the transport and the window is ready before showing the first window
  Promise.all([firstWindowReady.getPromise(), transportReady]).then(() => {
    firstWindow.show()
  })

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
    submenu: [{ role: 'quit', label: 'Quit Electric UI' }],
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
      // Window magnification options
      { role: 'zoomin' },
      { role: 'zoomout' },
      { role: 'resetzoom' },
      { type: 'separator' },
      // Fullscreen Toggle
      { role: 'togglefullscreen' },
      ...(isMac
        ? [
            { type: 'separator' },
            { role: 'front' },
            { type: 'separator' },
            { role: 'window' },
          ]
        : [{ type: 'separator' }, { role: 'close' }]),
    ],
  },
  ...(process.env.NODE_ENV === 'development'
    ? [
        {
          label: 'DevTools',
          submenu: [
            { role: 'reload' },
            { role: 'forcereload' },
            { type: 'separator' },

            { role: 'toggledevtools' },
            {
              label: 'Show Transport Window',
              click: () => {
                const electricWindow = getElectricWindow()

                if (electricWindow) {
                  electricWindow.show()
                  electricWindow.webContents.openDevTools({
                    mode: 'undocked',
                  })
                }
              },
            },
          ],
        },
      ]
    : []),
  {
    role: 'help',
    submenu: [
      { role: 'reload' },
      { type: 'separator' },
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
