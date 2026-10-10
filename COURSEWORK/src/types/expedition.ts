export type Position = [longitude: number, latitude: number]

export interface ExpeditionGeometry {
  type: 'LineString'
  coordinates: Position[]
}

export interface ExpeditionProperties {
  id: string
  name: string
  startYear: number
  endYear: number
  region: string
  color: string
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