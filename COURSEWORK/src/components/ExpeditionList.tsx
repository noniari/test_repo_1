import type { ExpeditionFeature } from '../types/expedition'

interface ExpeditionListProps {
  expeditions: ExpeditionFeature[]
  selectedId: string | null
  onSelect: (id: string) => void
}

export function ExpeditionList({ expeditions, selectedId, onSelect }: ExpeditionListProps) {
  if (expeditions.length === 0) {
    return <div className="empty-results">По заданным условиям экспедиций не найдено.</div>
  }

  return (
    <div className="expedition-list" aria-label="Список экспедиций">
      {expeditions.map(({ properties }) => (
        <button
          className={`expedition-row${properties.id === selectedId ? ' is-selected' : ''}`}
          key={properties.id}
          type="button"
          aria-pressed={properties.id === selectedId}
          onClick={() => onSelect(properties.id)}
        >
          <span className="expedition-symbol" style={{ color: properties.color }} />
          <span className="expedition-row-copy">
            <strong>{properties.name}</strong>
            <span>{properties.region} · {properties.field}</span>
          </span>
          <span className="expedition-years">{properties.startYear}–{properties.endYear}</span>
        </button>
      ))}
    </div>
  )
}