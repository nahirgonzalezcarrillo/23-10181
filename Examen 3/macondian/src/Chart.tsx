///
/// Chart
///
/// Grafica la serie de valores Macondo (ya procesados) por sensor, a lo
/// largo de los lotes: curvas suavizadas, relleno degradado, grid de
/// referencia y leyenda tipo "pastilla" por sensor.
///

import { useMemo } from 'react';

import { MacondoPoint } from './macondo';

interface Props {
  data: MacondoPoint[];
}

const PALETTE = ['#ff6b4a', '#2fd7ea', '#ffd54a', '#5be08a', '#c17bff', '#ff9a5c', '#5c8bff', '#ff5ca8'];

// Curva suave (Catmull-Rom -> Bezier) a través de los puntos de una serie,
// en vez de segmentos rectos entre cada lote.
const smoothPath = (pts: { x: number; y: number }[]): string => {
  if (pts.length < 2) {
    return pts.length === 1 ? `M ${pts[0].x} ${pts[0].y}` : '';
  }
  let d = `M ${pts[0].x.toFixed(2)} ${pts[0].y.toFixed(2)}`;
  for (let i = 0; i < pts.length - 1; i++) {
    const p0 = pts[i === 0 ? 0 : i - 1];
    const p1 = pts[i];
    const p2 = pts[i + 1];
    const p3 = pts[i + 2 < pts.length ? i + 2 : i + 1];
    const c1x = p1.x + (p2.x - p0.x) / 6;
    const c1y = p1.y + (p2.y - p0.y) / 6;
    const c2x = p2.x - (p3.x - p1.x) / 6;
    const c2y = p2.y - (p3.y - p1.y) / 6;
    d += ` C ${c1x.toFixed(2)} ${c1y.toFixed(2)}, ${c2x.toFixed(2)} ${c2y.toFixed(2)}, ${p2.x.toFixed(2)} ${p2.y.toFixed(2)}`;
  }
  return d;
};

const Chart = ({ data }: Props) => {
  const { series, minBatch, maxBatch, minVal, maxVal } = useMemo(() => {
    const bySensor = new Map<string, MacondoPoint[]>();
    for (const p of data) {
      const arr = bySensor.get(p.sensorId) ?? [];
      arr.push(p);
      bySensor.set(p.sensorId, arr);
    }
    const series = Array.from(bySensor.entries()).map(([id, pts]) => ({
      id,
      pts: [...pts].sort((a, b) => a.batch - b.batch),
    }));
    const batches = data.map(p => p.batch);
    const values = data.map(p => p.value);
    return {
      series,
      minBatch: batches.length ? Math.min(...batches) : 0,
      maxBatch: batches.length ? Math.max(...batches) : 1,
      minVal: values.length ? Math.min(...values) : 0,
      maxVal: values.length ? Math.max(...values) : 1,
    };
  }, [data]);

  if (data.length === 0) {
    return <div className="chart-empty">Esperando datos procesados ...</div>;
  }

  const W = 640, H = 360, PAD_L = 46, PAD_R = 16, PAD_T = 16, PAD_B = 30;
  const spanX = Math.max(1, maxBatch - minBatch);
  const valPad = Math.max(1e-6, (maxVal - minVal) * 0.12); // aire arriba/abajo
  const loVal = minVal - valPad;
  const hiVal = maxVal + valPad;
  const spanY = Math.max(1e-6, hiVal - loVal);
  const x = (b: number) => PAD_L + ((b - minBatch) / spanX) * (W - PAD_L - PAD_R);
  const y = (v: number) => H - PAD_B - ((v - loVal) / spanY) * (H - PAD_T - PAD_B);
  const baseline = H - PAD_B;

  const GRID_ROWS = 4;
  const gridValues = Array.from({ length: GRID_ROWS + 1 }, (_, i) => loVal + (spanY * i) / GRID_ROWS);

  return (
    <div className="chart-wrap">
      <svg viewBox={`0 0 ${W} ${H}`} className="chart-svg" preserveAspectRatio="xMidYMid meet">
        <defs>
          {series.map((s, i) => {
            const color = PALETTE[i % PALETTE.length];
            return (
              <linearGradient key={s.id} id={`fill-${s.id}`} x1="0" y1="0" x2="0" y2="1">
                <stop offset="0%" stopColor={color} stopOpacity={0.35} />
                <stop offset="100%" stopColor={color} stopOpacity={0} />
              </linearGradient>
            );
          })}
          <filter id="chart-glow" x="-40%" y="-40%" width="180%" height="180%">
            <feGaussianBlur stdDeviation="2.2" result="blur" />
            <feMerge>
              <feMergeNode in="blur" />
              <feMergeNode in="SourceGraphic" />
            </feMerge>
          </filter>
        </defs>

        {/* grid de referencia */}
        {gridValues.map((v, i) => (
          <g key={i}>
            <line x1={PAD_L} y1={y(v)} x2={W - PAD_R} y2={y(v)} stroke="#2a2a2a" strokeDasharray="3 4" />
            <text x={PAD_L - 8} y={y(v) + 3} fill="#777" fontSize="9" textAnchor="end">{v.toFixed(2)}</text>
          </g>
        ))}
        <line x1={PAD_L} y1={baseline} x2={W - PAD_R} y2={baseline} stroke="#555" />

        <text x={PAD_L} y={H - 6} fill="#888" fontSize="10">lote {minBatch}</text>
        <text x={W - PAD_R} y={H - 6} fill="#888" fontSize="10" textAnchor="end">lote {maxBatch}</text>

        {series.map((s, i) => {
          const color = PALETTE[i % PALETTE.length];
          const pts = s.pts.map(p => ({ x: x(p.batch), y: y(p.value) }));
          const line = smoothPath(pts);
          const area = pts.length > 1
            ? `${line} L ${pts[pts.length - 1].x.toFixed(2)} ${baseline} L ${pts[0].x.toFixed(2)} ${baseline} Z`
            : '';
          return (
            <g key={s.id}>
              {area && <path d={area} fill={`url(#fill-${s.id})`} stroke="none" />}
              <path
                d={line}
                fill="none"
                stroke={color}
                strokeWidth={2.5}
                strokeLinecap="round"
                strokeLinejoin="round"
                filter="url(#chart-glow)"
              />
              {pts.map((p, idx) => (
                <circle key={`${s.id}-${idx}`} cx={p.x} cy={p.y} r={3} fill="#0d0d0d" stroke={color} strokeWidth={1.5}>
                  <title>{`${s.id} · lote ${s.pts[idx].batch} · ${s.pts[idx].value.toFixed(3)}`}</title>
                </circle>
              ))}
            </g>
          );
        })}
      </svg>
      <div className="chart-legend">
        {series.map((s, i) => (
          <span key={s.id} className="chart-legend-item" style={{ borderColor: PALETTE[i % PALETTE.length] }}>
            <i style={{ backgroundColor: PALETTE[i % PALETTE.length] }} /> {s.id}
          </span>
        ))}
      </div>
    </div>
  );
};

export default Chart;