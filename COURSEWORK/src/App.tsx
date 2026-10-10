import { useEffect, useState } from 'react'
import {
  Compass,
  LoaderCircle,
  MapPin,
  RotateCcw,
  Search,
  SlidersHorizontal,
  Waves,
} from 'lucide-react'
import { ExpeditionDetails } from './components/ExpeditionDetails'
import { ExpeditionList } from './components/ExpeditionList'
import { FilterPanel } from './components/FilterPanel'
import { MapView } from './components/MapView'
import { parseExpeditionCollection } from './data/parseExpeditions'
import type { ExpeditionFeature } from './types/expedition'
import './App.css'

function App() {
  const [expeditions, setExpeditions] = useState<ExpeditionFeature[]>([])
  const [selectedId, setSelectedId] = useState<string | null>(null)
  const [query, setQuery] = useState('')
  const [region, setRegion] = useState('Все регионы')
  const [field, setField] = useState('Все направления')
  const [period, setPeriod] = useState('Любой период')
  const [loading, setLoading] = useState(true)
  const [error, setError] = useState<string | null>(null)
  const [reloadKey, setReloadKey] = useState(0)

  useEffect(() => {
    const controller = new AbortController()

    async function loadExpeditions() {
      setLoading(true)
      setError(null)
      try {
        const response = await fetch('/data/expeditions.geojson', {
          signal: controller.signal,
        })
        if (!response.ok) {
          throw new Error(`Не удалось загрузить GeoJSON (${response.status})`)
        }
        const collection: unknown = await response.json()
        setExpeditions(parseExpeditionCollection(collection).features)
      } catch (loadError) {
        if (loadError instanceof DOMException && loadError.name === 'AbortError') {
          return
        }
        setError(loadError instanceof Error ? loadError.message : 'Неизвестная ошибка загрузки')
      } finally {
        if (!controller.signal.aborted) {
          setLoading(false)
        }
      }
    }

    void loadExpeditions()
    return () => controller.abort()
  }, [reloadKey])

  const visibleExpeditions = expeditions.filter((feature) => {
    const properties = feature.properties
    const normalizedQuery = query.trim().toLocaleLowerCase('ru')
    const matchesQuery = [
      properties.name,
      properties.leader,
      properties.vessel,
      properties.region,
      properties.field,
    ].some((value) => value.toLocaleLowerCase('ru').includes(normalizedQuery))
    const matchesRegion = region === 'Все регионы' || properties.region === region
    const matchesField = field === 'Все направления' || properties.field === field
    const matchesPeriod =
      period === 'Любой период' ||
      (period === 'До 1900 года' && properties.endYear < 1900) ||
      (period === '1900–1950' && properties.startYear <= 1950 && properties.endYear >= 1900) ||
      (period === 'После 1950 года' && properties.endYear > 1950)
    return matchesQuery && matchesRegion && matchesField && matchesPeriod
  })

  const selectedExpedition =
    visibleExpeditions.find((feature) => feature.properties.id === selectedId) ??
    visibleExpeditions[0] ??
    null

  const filtersAreActive =
    query.length > 0 ||
    region !== 'Все регионы' ||
    field !== 'Все направления' ||
    period !== 'Любой период'

  function resetFilters() {
    setQuery('')
    setRegion('Все регионы')
    setField('Все направления')
    setPeriod('Любой период')
  }

  return (
    <main className="app-shell">
      <header className="topbar">
        <a className="brand" href="#map" aria-label="Атлас экспедиций — к карте">
          <span className="brand-mark"><Compass size={21} strokeWidth={1.7} /></span>
          <span className="brand-copy">
            <span className="brand-name">Полевой атлас</span>
            <span className="brand-caption">Архив научных экспедиций</span>
          </span>
        </a>

        <div className="topbar-meta">
          <span className="archive-status"><span /> ОТКРЫТЫЙ АРХИВ</span>
          <span className="topbar-divider" />
          <span className="topbar-count">{expeditions.length.toString().padStart(2, '0')} экспедиций</span>
        </div>
      </header>

      <section className="workspace" id="map" aria-label="Интерактивная карта экспедиций">
        <aside className="sidebar">
          <div className="sidebar-heading">
            <div className="section-kicker"><Waves size={13} /> КАРТОТЕКА / 01</div>
            <h1>Маршруты<br />исследования мира</h1>
            <p>Научные экспедиции и полевые наблюдения на одной карте.</p>
          </div>

          <div className="search-wrap">
            <Search size={17} aria-hidden="true" />
            <input
              aria-label="Поиск экспедиций"
              placeholder="Название, исследователь, судно"
              value={query}
              onChange={(event) => setQuery(event.target.value)}
            />
            {query && (
              <button className="clear-search" type="button" onClick={() => setQuery('')} aria-label="Очистить поиск">
                ×
              </button>
            )}
          </div>

          <FilterPanel
            region={region}
            field={field}
            period={period}
            onRegionChange={setRegion}
            onFieldChange={setField}
            onPeriodChange={setPeriod}
          />

          <div className="results-heading">
            <div>
              <span className="section-kicker"><SlidersHorizontal size={13} /> РЕЗУЛЬТАТЫ</span>
              <h2>Экспедиции <span>{visibleExpeditions.length.toString().padStart(2, '0')}</span></h2>
            </div>
            {filtersAreActive && (
              <button className="reset-button" type="button" onClick={resetFilters} title="Сбросить фильтры">
                <RotateCcw size={14} /> Сбросить
              </button>
            )}
          </div>

          {loading ? (
            <div className="state-message"><LoaderCircle className="spin" size={19} /> Загружаем архив…</div>
          ) : error ? (
            <div className="state-message state-error" role="alert">
              <p>{error}</p>
              <button type="button" onClick={() => setReloadKey((value) => value + 1)}>Повторить</button>
            </div>
          ) : (
            <ExpeditionList
              expeditions={visibleExpeditions}
              selectedId={selectedExpedition?.properties.id ?? null}
              onSelect={setSelectedId}
            />
          )}

          {selectedExpedition && !loading && !error && (
            <ExpeditionDetails expedition={selectedExpedition} />
          )}

          <footer className="sidebar-footer">
            <MapPin size={13} /> Координаты маршрутов обобщены по историческим источникам
          </footer>
        </aside>

        <section className="map-panel" aria-label="Карта маршрутов">
          {loading ? (
            <div className="map-placeholder"><LoaderCircle className="spin" size={24} />
              <span>Подготавливаем карту</span>
            </div>
          ) : error ? (
            <div className="map-placeholder map-error">Карта ожидает данные архива</div>
          ) : (
            <MapView
              expeditions={visibleExpeditions}
              selectedId={selectedExpedition?.properties.id ?? null}
              onSelect={setSelectedId}
            />
          )}
          <div className="map-stamp"><span>ПЛАНЕТА</span><strong>ЗЕМЛЯ</strong></div>
          <div className="map-legend" aria-label="Условные обозначения">
            <span><i className="legend-line legend-polar" /> Полярные</span>
            <span><i className="legend-line legend-ocean" /> Океанография</span>
            <span><i className="legend-line legend-other" /> Другие направления</span>
            <span><i className="legend-dot" /> Место наблюдения</span>
          </div>
          <div className="map-credit">© OpenStreetMap · FOSSGIS</div>
        </section>
      </section>
    </main>
  )
}

export default App
