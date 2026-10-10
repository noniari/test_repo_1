import { useEffect } from 'react'
import { MapContainer, Polyline, TileLayer, useMap } from 'react-leaflet'
import { Minus, Plus } from 'lucide-react'
import L, { type LatLngExpression } from 'leaflet'
import 'leaflet/dist/leaflet.css'
import type { ExpeditionFeature, Position } from '../types/expedition'

interface ExpeditionMapProps {
  expeditions: ExpeditionFeature[]
  selectedId: string | null
  onSelect: (id: string) => void
}

function toLatLng([longitude, latitude]: Position): LatLngExpression {
  return [latitude, longitude]
}

function FocusRoute({ expedition }: { expedition: ExpeditionFeature | null }) {
  const map = useMap()

  useEffect(() => {
    if (!expedition) return
    map.flyToBounds(L.latLngBounds(expedition.geometry.coordinates.map(toLatLng)), {
      padding: [64, 64],
      maxZoom: 5,
      duration: 0.65,
    })
  }, [expedition, map])

  return null
}

function MapControls() {
  const map = useMap()
  return (
    <div className="map-controls" aria-label="Масштаб карты">
      <button type="button" title="Приблизить" aria-label="Приблизить карту" onClick={() => map.zoomIn()}>
        <Plus size={17} />
      </button>
      <button type="button" title="Отдалить" aria-label="Отдалить карту" onClick={() => map.zoomOut()}>
        <Minus size={17} />
      </button>
    </div>
  )
}

export function ExpeditionMap({ expeditions, selectedId, onSelect }: ExpeditionMapProps) {
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
        attribution='&copy; <a href="https://www.openstreetmap.org/copyright">OpenStreetMap</a> contributors &copy; <a href="https://www.fossgis.de/">FOSSGIS</a>'
        url="https://tile.openstreetmap.de/{z}/{x}/{y}.png"
      />
      {expeditions.map(({ properties, geometry }) => {
        const isSelected = selectedId === properties.id
        const routeStyle = {
          color: properties.color,
          weight: isSelected ? 5 : 3,
          opacity: isSelected ? 0.95 : 0.72,
        }

        return (
          <Polyline
            key={properties.id}
            positions={geometry.coordinates.map(toLatLng)}
            pathOptions={routeStyle}
            eventHandlers={{ click: () => onSelect(properties.id) }}
          />
        )
      })}
      <FocusRoute expedition={selected} />
      <MapControls />
    </MapContainer>
  )
}