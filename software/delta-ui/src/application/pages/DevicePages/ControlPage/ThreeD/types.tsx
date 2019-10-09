export type Vertices = [number, number, number]
export type ViewerVertices = Array<Vertices>
export interface CollectionForUI {
  name: string
  duration: number
  first_move: number
  last_move: number
  viewer_vertices: ViewerVertices
  viewer_uv: string
}
