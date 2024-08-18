import fs from 'fs'
import path from 'path'
import { importJson, Renderable } from './import'
import type { MovementJSON, FrameMovementJSON } from './import'
import type { Toolpath } from './toolpath'
import type { Settings } from './settings'
import type { Movement } from './movements'
import { isEmpty } from './empty'
import { VisualisationSettings } from '../interface/state'
import { preprocess } from './preprocess'
import { CancellationToken } from '@electricui/async-utilities'

async function* walkJSON(dir: string): AsyncGenerator<string> {
  for await (const d of await fs.promises.opendir(dir)) {
    const entry = path.join(dir, d.name)
    if (d.isDirectory()) yield* walkJSON(entry)
    else if (d.isFile() && path.extname(d.name) === '.json') yield entry
  }
}

export interface RootSettingsFile {
  type: `root-settings`
  version: 1
  settings: Settings
  visualisationSettings: VisualisationSettings
}

// Walk a folder path, extracting all renderables
export async function importFolder(folderPath: string) {
  const renderablesByFrame: Map<number, Renderable[]> = new Map()

  const movementJSONByFrame: {
    [frame: number]: FrameMovementJSON
  } = {}

  const allRenderables: Renderable[] = []

  const frameData: {
    [frame: number]: {
      [key: string]: any
    }
  } = {}

  let minFrame = Infinity
  let maxFrame = -Infinity

  const potentialRootSettingsFile = path.join(folderPath, `settings.json`)

  let settingsToMerge: Partial<Settings> = {}
  let visualisationSettingsToMerge: Partial<VisualisationSettings> = {}

  // Walk the folder to find json files
  for await (const p of walkJSON(folderPath)) {
    const contents = await fs.promises.readFile(p)
    let parsed: MovementJSON | RootSettingsFile = JSON.parse(contents.toString())

    // Handle the special case of the root settings file
    if (p === potentialRootSettingsFile && parsed.type === `root-settings`) {
      settingsToMerge = parsed.settings ?? {}
      visualisationSettingsToMerge = parsed.visualisationSettings ?? {}
      continue
    }

    // Narrow the type
    parsed = parsed as MovementJSON

    if (!parsed.type || !parsed.frame) {
      // unknown file
      console.warn('Unknown file format', p)
      continue
    }

    if (!renderablesByFrame.has(parsed.frame)) {
      renderablesByFrame.set(parsed.frame, [])
    }

    if (!movementJSONByFrame[parsed.frame]) {
      movementJSONByFrame[parsed.frame] = {
        filepath: p,
        movementJSON: [],
      }
    }

    // Grab this frame's bag of renderables

    // Import the file
    const renderable = importJson(p, parsed)

    // Import any frame data
    if (isEmpty(renderable)) {
      frameData[parsed.frame] = renderable.data
    }

    // Add it to the global list
    allRenderables.push(renderable)

    // Add it to the frame's structure
    renderablesByFrame.get(parsed.frame)!.push(renderable)
    movementJSONByFrame[parsed.frame].movementJSON.push(parsed)

    // Update frame counters
    minFrame = Math.min(minFrame, parsed.frame)
    maxFrame = Math.max(maxFrame, parsed.frame)
  }

  const sceneTotalFrames = maxFrame - minFrame

  return {
    renderablesByFrame,
    movementJSONByFrame,
    allRenderables,
    minFrame,
    maxFrame,
    frameData,
    settingsToMerge,
    visualisationSettingsToMerge,
    sceneTotalFrames,
  }
}

export async function renderablesToMovements(
  renderables: Renderable[],
  settings: Settings,
  cancellationToken: CancellationToken,
) {
  const movements: Movement[] = []

  for (const renderable of renderables) {
    for (const movement of await renderable.toMovements(settings, true)) {
      movements.push(movement)

      cancellationToken.haltIfCancelled()
    }
  }

  return preprocess(movements, settings)
}

export interface ObjectNameTree {
  [key: string]: ObjectNameTree | string[]
}

export function writeToolpathToFile(filepath: string, toolpath: Toolpath) {
  return fs.promises.writeFile(filepath, JSON.stringify(toolpath))
}
