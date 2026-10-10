import { Fragment, useEffect } from 'react'
import { CircleMarker, MapContainer, Polyline, TileLayer, useMap } from 'react-leaflet'
import { Minus, Plus } from 'lucide-react'
import L, { type LatLngExpression } from 'leaflet'
import 'leaflet/dist/leaflet.css'
import type { ExpeditionFeature, Position } from '../types/expedition'

interface MapViewProps {
  expeditions: ExpeditionFeature[]
  selectedId: string | null
  onSelect: (id: string) => void
}

function toLatLng([longitude, latitude]: Position): LatLngExpression {
  return [latitude, longitude]
}

function FocusSelected({ expedition }: { expedition: ExpeditionFeature | null }) {
  const map = useMap()

  useEffect(() => {
    if (!expedition) return
    if (expedition.geometry.type === 'Point') {
      map.flyTo(toLatLng(expedition.geometry.coordinates), Math.max(map.getZoom(), 4), {
        duration: 0.7,
      })
      return
    }
    const bounds = L.latLngBounds(expedition.geometry.coordinates.map(toLatLng))
    map.flyToBounds(bounds, { padding: [70, 70], maxZoom: 5, duration: 0.7 })
  }, [expedition, map])

  return null
}

function MapControls() {
  const map = useMap()
  return (
    <div className="map-controls" aria-label="Управление масштабом карты">
      <button type="button" title="Приблизить" aria-label="Приблизить карту" onClick={() => map.zoomIn()}>
        <Plus size={17} />
      </button>
      <button type="button" title="Отдалить" aria-label="Отдалить карту" onClick={() => map.zoomOut()}>
        <Minus size={17} />
      </button>
    </div>
  )
}

export function MapView({ expeditions, selectedId, onSelect }: MapViewProps) {
  const selected = expeditions.find((feature) => feature.properties.id === selectedId) ?? null

  return (
    <MapContainer
      className="map-view"
      center={[26, 12]}
      zoom={2}
      minZoom={2}
      maxZoom={9}
      scrollWheelZoom
      zoomControl={false}
      worldCopyJump
    >
      <TileLayer
        attribution='&copy; <a href="https://www.openstreetmap.org/copyright">OpenStreetMap</a> contributors &copy; <a href="https://carto.com/attributions">CARTO</a>'
         url="https://tile.openstreetmap.de/{z}/{x}/{y}.png"
      />
      {expeditions.map((feature) => {
        const { properties, geometry } = feature
        const isSelected = properties.id === selectedId
        const commonOptions = {
          color: properties.color,
          weight: isSelected ? 5 : 3,
          opacity: isSelected ? 0.95 : 0.67,
        }

        if (geometry.type === 'Point') {
          return (
            <CircleMarker
              key={properties.id}
              center={toLatLng(geometry.coordinates)}
              radius={isSelected ? 9 : 7}
              pathOptions={{ color: '#fff', weight: 2, fillColor: properties.color, fillOpacity: 1 }}
              eventHandlers={{ click: () => onSelect(properties.id) }}
            />
          )
        }

        return (
          <Fragment key={properties.id}>
            <Polyline
              positions={geometry.coordinates.map(toLatLng)}
              pathOptions={commonOptions}
              eventHandlers={{ click: () => onSelect(properties.id) }}
            />
            <CircleMarker
              center={toLatLng(geometry.coordinates[0])}
              radius={isSelected ? 6 : 4}
              pathOptions={{ color: '#fff', weight: 1.5, fillColor: properties.color, fillOpacity: 1 }}
              eventHandlers={{ click: () => onSelect(properties.id) }}
            />
            <CircleMarker
              center={toLatLng(geometry.coordinates[geometry.coordinates.length - 1])}
              radius={isSelected ? 6 : 4}
              pathOptions={{ color: '#fff', weight: 1.5, fillColor: properties.color, fillOpacity: 1 }}
              eventHandlers={{ click: () => onSelect(properties.id) }}
            />
          </Fragment>
        )
      })}
      <FocusSelected expedition={selected} />
      <MapControls />
    </MapContainer>
  )
}