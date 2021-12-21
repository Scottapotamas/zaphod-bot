import fs from 'fs'
import path from 'path'
import { importJson, Renderable } from './import'
import { MovementJSON } from './import'
import { Toolpath } from './passes'

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
  }
}

export function calculateObjectTree(renderables: Renderable[]) {
  let tree = {}

  // Merges every object across every frame
  for (const renderable of renderables) {
    tree = Object.assign(tree, renderable.getObjectNameTree())
  }

  return tree
}

export function writeToolpathToFile(filepath: string, toolpath: Toolpath) {
  return fs.promises.writeFile(filepath, JSON.stringify(toolpath))
}
