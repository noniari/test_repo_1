import { useEffect, useState, type CSSProperties } from 'react'
import { Compass, MapPin, MoveUpRight } from 'lucide-react'
import { ExpeditionMap } from './components/ExpeditionMap'
import type { ExpeditionCollection, ExpeditionFeature } from './types/expedition'
import './App.css'

function App() {
  const [expeditions, setExpeditions] = useState<ExpeditionFeature[]>([])
  const [selectedId, setSelectedId] = useState<string | null>(null)
  const [loadError, setLoadError] = useState(false)

  useEffect(() => {
    const controller = new AbortController()

    fetch('/data/expeditions.geojson', { signal: controller.signal })
      .then((response) => {
        if (!response.ok) throw new Error('GeoJSON недоступен')
        return response.json() as Promise<ExpeditionCollection>
      })
      .then((collection) => setExpeditions(collection.features))
      .catch((error: unknown) => {
        if (error instanceof DOMException && error.name === 'AbortError') return
        setLoadError(true)
      })

    return () => controller.abort()
  }, [])

  return (
    <main className="app-shell">
      <header className="topbar">
        <a className="brand" href="#map" aria-label="Атлас экспедиций — к карте">
          <span className="brand-mark"><Compass size={21} strokeWidth={1.7} /></span>
          <span className="brand-copy">
            <span className="brand-name">Полевой атлас</span>
            <span className="brand-caption">Научные экспедиции</span>
          </span>
        </a>
        <div className="release-badge"><span /> v0.1 BETA · В РАЗРАБОТКЕ</div>
      </header>

      <section className="workspace" id="map">
        <aside className="sidebar">
          <div className="sidebar-heading">
            <div className="section-kicker"><span className="kicker-line" /> КУРСОВОЙ ПРОЕКТ / 01</div>
            <h1>Маршруты<br />научных открытий</h1>
            <p>Начинаем собирать карту экспедиций. Сейчас здесь — первые демонстрационные маршруты.</p>
          </div>

          <div className="progress-note">
            <span className="progress-indicator" />
            <div>
              <strong>Первая версия проекта</strong>
              <span>Основа карты и данные GeoJSON</span>
            </div>
          </div>

          <div className="routes-heading">
            <span className="section-kicker">ПЕРВЫЕ МАРШРУТЫ</span>
            <span className="route-count">{expeditions.length.toString().padStart(2, '0')}</span>
          </div>

          {loadError ? (
            <p className="load-message" role="alert">Не удалось загрузить демонстрационные данные.</p>
          ) : (
            <div className="route-list">
              {expeditions.map(({ properties }) => (
                <button
                  className={`route-item${selectedId === properties.id ? ' is-selected' : ''}`}
                  type="button"
                  key={properties.id}
                  aria-pressed={selectedId === properties.id}
                  onClick={() => setSelectedId(properties.id)}
                >
                  <span className="route-marker" style={{ '--route-color': properties.color } as CSSProperties} />
                  <span className="route-copy">
                    <strong>{properties.name}</strong>
                    <span>{properties.startYear}–{properties.endYear} · {properties.region}</span>
                  </span>
                  <MoveUpRight className="route-arrow" size={14} />
                </button>
              ))}
              {expeditions.length === 0 && <p className="load-message">Загружаем первые маршруты…</p>}
            </div>
          )}

          <div className="next-note">
            <MapPin size={15} />
            <p>Дальше добавим проверенные источники, карточки экспедиций и поиск.</p>
          </div>
          <footer className="sidebar-footer">ПРОЕКТ В РАЗРАБОТКЕ <span>·</span> 2026</footer>
        </aside>

        <section className="map-panel" aria-label="Карта маршрутов">
          <ExpeditionMap expeditions={expeditions} selectedId={selectedId} onSelect={setSelectedId} />
          <div className="map-stamp"><span>ПОЛЕВОЙ АТЛАС</span><strong>Первые маршруты</strong></div>
          <div className="map-legend" aria-label="Условные обозначения">
            <span><i className="legend-line" /> Демонстрационные маршруты</span>
          </div>
          <div className="map-credit">© OpenStreetMap · FOSSGIS</div>
        </section>
      </section>
    </main>
  )
}

export default App
