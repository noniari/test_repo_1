export type Position = [longitude: number, latitude: number]

export type ExpeditionGeometry =
  | { type: 'Point'; coordinates: Position }
  | { type: 'LineString'; coordinates: Position[] }

export interface ExpeditionProperties {
  id: string
  name: string
  startYear: number
  endYear: number
  leader: string
  vessel: string
  region: string
  field: string
  summary: string
  color: string
  sourceNote: string
}

export interface ExpeditionFeature {
  type: 'Feature'
  id: string
  properties: ExpeditionProperties
  geometry: ExpeditionGeometry
}

export interface ExpeditionCollection {
  type: 'FeatureCollection'
  features: ExpeditionFeature[]
}