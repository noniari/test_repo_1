import type {
  ExpeditionCollection,
  ExpeditionFeature,
  ExpeditionProperties,
  Position,
} from '../types/expedition'

function isRecord(value: unknown): value is Record<string, unknown> {
  return typeof value === 'object' && value !== null
}

function isPosition(value: unknown): value is Position {
  return (
    Array.isArray(value) &&
    value.length >= 2 &&
    typeof value[0] === 'number' &&
    typeof value[1] === 'number' &&
    Number.isFinite(value[0]) &&
    Number.isFinite(value[1]) &&
    Math.abs(value[0]) <= 180 &&
    Math.abs(value[1]) <= 90
  )
}

function parseProperties(value: unknown, featureIndex: number): ExpeditionProperties {
  if (!isRecord(value)) {
    throw new Error(`У объекта ${featureIndex + 1} отсутствуют свойства экспедиции`)
  }

  const textFields = ['id', 'name', 'leader', 'vessel', 'region', 'field', 'summary', 'color', 'sourceNote']
  for (const field of textFields) {
    if (typeof value[field] !== 'string' || value[field].trim() === '') {
      throw new Error(`У объекта ${featureIndex + 1} некорректное поле «${field}»`)
    }
  }
  if (
    typeof value.startYear !== 'number' ||
    typeof value.endYear !== 'number' ||
    !Number.isInteger(value.startYear) ||
    !Number.isInteger(value.endYear) ||
    value.startYear > value.endYear
  ) {
    throw new Error(`У объекта ${featureIndex + 1} некорректно указан период экспедиции`)
  }

  return {
    id: value.id as string,
    name: value.name as string,
    startYear: value.startYear,
    endYear: value.endYear,
    leader: value.leader as string,
    vessel: value.vessel as string,
    region: value.region as string,
    field: value.field as string,
    summary: value.summary as string,
    color: value.color as string,
    sourceNote: value.sourceNote as string,
  }
}

function parseFeature(value: unknown, index: number): ExpeditionFeature {
  if (!isRecord(value) || value.type !== 'Feature' || !isRecord(value.geometry)) {
    throw new Error(`Объект ${index + 1} не является GeoJSON Feature`)
  }

  const properties = parseProperties(value.properties, index)
  const geometry = value.geometry
  if (geometry.type === 'Point' && isPosition(geometry.coordinates)) {
    return {
      type: 'Feature',
      id: properties.id,
      properties,
      geometry: { type: 'Point', coordinates: geometry.coordinates },
    }
  }
  if (
    geometry.type === 'LineString' &&
    Array.isArray(geometry.coordinates) &&
    geometry.coordinates.length >= 2 &&
    geometry.coordinates.every(isPosition)
  ) {
    return {
      type: 'Feature',
      id: properties.id,
      properties,
      geometry: { type: 'LineString', coordinates: geometry.coordinates },
    }
  }
  throw new Error(`У объекта ${index + 1} неподдерживаемая или некорректная геометрия`)
}

export function parseExpeditionCollection(value: unknown): ExpeditionCollection {
  if (!isRecord(value) || value.type !== 'FeatureCollection' || !Array.isArray(value.features)) {
    throw new Error('Файл не является GeoJSON FeatureCollection')
  }

  const features = value.features.map(parseFeature)
  const ids = new Set<string>()
  for (const feature of features) {
    if (ids.has(feature.properties.id)) {
      throw new Error(`Повторяется идентификатор экспедиции: ${feature.properties.id}`)
    }
    ids.add(feature.properties.id)
  }
  return { type: 'FeatureCollection', features }
}