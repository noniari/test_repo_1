import type { CSSProperties } from 'react'
import type { ExpeditionFeature } from '../types/expedition'

interface ExpeditionDetailsProps {
  expedition: ExpeditionFeature
}

export function ExpeditionDetails({ expedition }: ExpeditionDetailsProps) {
  const { properties } = expedition

  return (
    <article className="expedition-details" style={{ '--detail-color': properties.color } as CSSProperties}>
      <div className="details-kicker">КАРТОЧКА ЭКСПЕДИЦИИ</div>
      <h3>{properties.name}</h3>
      <div className="details-meta">
        {properties.startYear}–{properties.endYear} · {properties.leader}
      </div>
      <p>{properties.summary}</p>
      <div className="detail-tags">
        <span>{properties.vessel}</span>
        <span>{properties.sourceNote}</span>
      </div>
    </article>
  )
}