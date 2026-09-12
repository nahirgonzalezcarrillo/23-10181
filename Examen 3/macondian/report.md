## Modelos de IA usados
- Claude Sonnet 5 (Anthropic), vía claude.ai — usado para las tareas [1], [2] y [3]
  (código de UX, procesamiento de datos y gráfica).

Tarea [4]
  Para cada sensor, en cada lote, el algoritmo recibe las lecturas de varios microsensores. Primero calcula la mediana como valor de referencia. Luego uso un porcentaje en vez de un número fijo porque así el margen de tolerancia cambia según el tamaño de los valores de cada sensor, en vez de ser el mismo para todos. Los valores que quedan fuera de ese margen se descartan por estar demasiado alejados de la mediana. Con los que quedan dentro del margen, se saca el promedio, y ese es el valor Macondo final del sensor para ese lote. Sin embargo, si ningún valor queda dentro del margen, en vez de perder el dato se mantienen los valores originales sin filtrar