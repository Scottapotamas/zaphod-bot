import fs from 'fs'
import path from 'path'
import { importJson, Renderable } from './import'
import { MovementJSON } from './import'
import type { Toolpath } from './toolpath'
import { Camera } from './camera'
import { Settings } from './settings'
import { Movement } from './movements'
import { isEmpty } from './empty'

async function* walkJSON(dir: string): AsyncGenerator<string> {
  for await (const d of await fs.promises.opendir(dir)) {
    const entry = path.join(dir, d.name)
    if (d.isDirectory()) yield* walkJSON(entry)
    else if (d.isFile() && path.extname(d.name) === '.json') yield entry
  }
}

// Walk a folder path, extracting all renderables
export async function importFolder(folderPath: string) {
  const renderablesByFrame: {
    [frame: number]: Renderable[]
  } = {}
  const movementJSONByFrame: {
    [frame: number]: MovementJSON[]
  } = {}

  const allRenderables: Renderable[] = []

  const frameData: {
    [frame: number]: {
      [key: string]: any
    }
  } = {}

  let minFrame = Infinity
  let maxFrame = -Infinity

  // Walk the folder to find json files
  for await (const p of walkJSON(folderPath)) {
    const contents = await fs.promises.readFile(p)
    const parsed: MovementJSON = JSON.parse(contents.toString())

    if (!parsed.type || !parsed.frame) {
      // unknown file
      console.warn('Unknown file format', p)
      continue
    }

    if (!renderablesByFrame[parsed.frame]) {
      renderablesByFrame[parsed.frame] = []
    }

    if (!movementJSONByFrame[parsed.frame]) {
      movementJSONByFrame[parsed.frame] = []
    }

    // Grab this frame's bag of renderables

    // Import the file
    const renderable = importJson(parsed)

    // Import any frame data
    if (isEmpty(renderable)) {
      frameData[parsed.frame] = renderable.data
    }

    // Add it to the global list
    allRenderables.push(renderable)

    // Add it to the frame's structure
    renderablesByFrame[parsed.frame].push(renderable)
    movementJSONByFrame[parsed.frame].push(parsed)

    // Update frame counters
    minFrame = Math.min(minFrame, parsed.frame)
    maxFrame = Math.max(maxFrame, parsed.frame)
  }

  return {
    renderablesByFrame,
    movementJSONByFrame,
    allRenderables,
    minFrame,
    maxFrame,
    frameData,
  }
}

export function renderablesToMovements(
  renderables: Renderable[],
  settings: Settings,
) {
  const movements: Movement[] = []

  for (const renderable of renderables) {
    for (const movement of renderable.toMovements(settings)) {
      movements.push(movement)
    }
  }

  return movements
}

export interface ObjectNameTree {
  [key: string]: ObjectNameTree | string[]
}

export function writeToolpathToFile(filepath: string, toolpath: Toolpath) {
  return fs.promises.writeFile(filepath, JSON.stringify(toolpath))
}
