///
/// macondo.ts
///
/// Interpreta la "señal cruda" emitida por el generador (tgmr) y sintetiza,
/// para cada sensor Macondiano y cada lote, un único valor Macondo a partir
/// de las lecturas de sus microsensores.
///

// Una lectura cruda de un sensor en un lote dado: sus microsensores sin procesar.
export type SensorBatch = {
  batch: number;
  sensorId: string;
  microarray: number[];
};

// El resultado de sintetizar un SensorBatch: un único valor Macondo.
export type MacondoPoint = {
  batch: number;
  sensorId: string;
  value: number;
};

// Radio Macondiano de tolerancia: según la entrevista, 5%-10% del valor
// representativo. Se deja como parámetro ajustable; 0.07 es un punto medio.
export const TOLERANCE = 0.07;

export const median = (values: number[]): number => {
  const sorted = [...values].sort((a, b) => a - b);
  const mid = Math.floor(sorted.length / 2);
  return sorted.length % 2 !== 0
    ? sorted[mid]
    : (sorted[mid - 1] + sorted[mid]) / 2;
};

// Sintetiza el arreglo de microsensores en un único valor Macondo:
//  1) valor representativo = mediana (robusta ante valores atípicos)
//  2) radio de tolerancia  = TOLERANCE * |representativo|
//  3) se descartan las lecturas fuera de ese radio (outliers)
//  4) valor Macondo final  = promedio de las lecturas restantes
// Si todas las lecturas quedaran descartadas (caso degenerado), se usa el
// arreglo completo como respaldo para no perder el dato del lote.
export const macondoValue = (microarray: number[], tolerance = TOLERANCE): number => {
  const representative = median(microarray);
  const radius = Math.abs(representative) * tolerance;
  const inliers = microarray.filter(v => Math.abs(v - representative) <= radius);
  const pool = inliers.length > 0 ? inliers : microarray;
  return pool.reduce((sum, v) => sum + v, 0) / pool.length;
};

// Formato de línea emitido por el generador para un sensor en un lote:
//   "<batch> <sensorId> [ v1,v2,...,vn ]"
const SENSOR_LINE_RE = /^(\d+)\s+(\S+)\s+\[\s*([^\]]*)\]$/;

// Interpreta una línea de la señal cruda. Devuelve un SensorBatch si la
// línea trae lecturas de un sensor, o null para cualquier otra línea
// (encabezados de lote, mensajes de estado, etc.)
export const parseSensorLine = (line: string): SensorBatch | null => {
  const m = SENSOR_LINE_RE.exec(line);
  if (!m) return null;
  const [, batchStr, sensorId, valuesStr] = m;
  const microarray = valuesStr
    .split(',')
    .map(Number)
    .filter(v => !Number.isNaN(v));
  if (microarray.length === 0) return null;
  return { batch: Number(batchStr), sensorId, microarray };
};
