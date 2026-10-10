interface FilterPanelProps {
  region: string
  field: string
  period: string
  onRegionChange: (value: string) => void
  onFieldChange: (value: string) => void
  onPeriodChange: (value: string) => void
}

export function FilterPanel({
  region,
  field,
  period,
  onRegionChange,
  onFieldChange,
  onPeriodChange,
}: FilterPanelProps) {
  return (
    <section className="filter-panel" aria-label="Фильтры экспедиций">
      <div className="filter-title">ОТОБРАТЬ ПО ПАРАМЕТРАМ</div>
      <div className="filter-grid">
        <label className="filter-label">
          Регион
          <select value={region} onChange={(event) => onRegionChange(event.target.value)}>
            <option>Все регионы</option>
            <option>Арктика</option>
            <option>Антарктика</option>
            <option>Мировой океан</option>
            <option>Южная Америка</option>
          </select>
        </label>
        <label className="filter-label">
          Направление
          <select value={field} onChange={(event) => onFieldChange(event.target.value)}>
            <option>Все направления</option>
            <option>Полярные исследования</option>
            <option>Океанография</option>
            <option>Биология</option>
            <option>Картография</option>
          </select>
        </label>
        <label className="filter-label">
          Период
          <select value={period} onChange={(event) => onPeriodChange(event.target.value)}>
            <option>Любой период</option>
            <option>До 1900 года</option>
            <option>1900–1950</option>
            <option>После 1950 года</option>
          </select>
        </label>
      </div>
    </section>
  )
}