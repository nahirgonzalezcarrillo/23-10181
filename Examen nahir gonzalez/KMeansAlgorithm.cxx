#include "KMeansAlgorithm.h"
#include <cmath>
#include <limits>
#include <random>

double KMeansAlgorithm::calculateDistance(const Coord_3D& p1, const Coord_3D& p2) {
    return std::sqrt(std::pow(p1.x - p2.x, 2) + std::pow(p1.y - p2.y, 2) + std::pow(p1.z - p2.z, 2));
}

/*
 * CONDICIÓN DE PARADA DEL ALGORITMO K-MEANS
 * Parámetros:
 * - oldC: Posiciones de los centroides en la iteración anterior.
 * - newC: Posiciones de los centroides en la iteración actual.
 * - tolerance: Umbral máximo de movimiento permitido (Ej: 1e-4).
 * * Explicación de por qué es buena condición:
 * No debemos exigir que los centroides sean EXACTAMENTE iguales entre iteraciones, ya que en un 
 * espacio tridimensional de números flotantes (double) pueden existir variaciones infinitesimales
 * ocasionando un ciclo de iteraciones excesivo (o infinito). Comprobar si el movimiento 
 * máximo es menor a la tolerancia encapsula una convergencia matemática "estable" y ahorra recursos.
 */
bool KMeansAlgorithm::hasConverged(const std::vector<Coord_3D>& oldC, const std::vector<Coord_3D>& newC, double tolerance) {
    if (oldC.empty()) return false;
    for (size_t i = 0; i < oldC.size(); ++i) {
        if (calculateDistance(oldC[i], newC[i]) > tolerance) {
            return false;
        }
    }
    return true;
}

std::vector<Labeled> KMeansAlgorithm::fitPredict(const std::vector<Coord_3D>& data, int k, std::vector<ClusterSummary>& summaryOut) {
    if (data.empty() || k <= 0) return {};
    int num_points = data.size();

    // 1. Inicialización (Semilla fija en 42 para evaluación de consistencia)
    std::vector<Coord_3D> centroids;
    std::mt19937 rng(42); 
    std::uniform_int_distribution<int> dist(0, num_points - 1);
    for (int i = 0; i < k; ++i) {
        centroids.push_back(data[dist(rng)]);
    }

    std::vector<int> assignments(num_points, 0);
    std::vector<Coord_3D> old_centroids;
    int max_iters = 200;
    int iter = 0;

    // Ejecución hasta convergencia usando la función encapsulada
    while (!hasConverged(old_centroids, centroids, 1e-4) && iter < max_iters) {
        old_centroids = centroids;

        // Asignar puntos al cluster más cercano
        for (int i = 0; i < num_points; ++i) {
            double min_dist = std::numeric_limits<double>::max();
            int best_cluster = 0;
            for (int c = 0; c < k; ++c) {
                double d = calculateDistance(data[i], centroids[c]);
                if (d < min_dist) {
                    min_dist = d;
                    best_cluster = c;
                }
            }
            assignments[i] = best_cluster;
        }

        // Actualizar centroides
        std::vector<Coord_3D> new_centroids(k, {0.0, 0.0, 0.0});
        std::vector<int> counts(k, 0);

        for (int i = 0; i < num_points; ++i) {
            int c = assignments[i];
            new_centroids[c].x += data[i].x;
            new_centroids[c].y += data[i].y;
            new_centroids[c].z += data[i].z;
            counts[c]++;
        }

        for (int c = 0; c < k; ++c) {
            if (counts[c] > 0) {
                centroids[c].x = new_centroids[c].x / counts[c];
                centroids[c].y = new_centroids[c].y / counts[c];
                centroids[c].z = new_centroids[c].z / counts[c];
            } else {
                centroids[c] = data[dist(rng)]; // Mitigar clusters vacíos
            }
        }
        iter++;
    }

    // Preparar los resultados inmutables (A .. F)
    std::vector<Labeled> result;
    result.reserve(num_points);
    for (int i = 0; i < num_points; ++i) {
        Labeled l;
        l.coord = data[i];
        l.label = 'A' + assignments[i];
        result.push_back(l);
    }

    // Preparar y calcular "Medida de Dispersión (MD)" (Error Cuadrático Medio / MSE)
    summaryOut.clear();
    for (int c = 0; c < k; ++c) {
        ClusterSummary cs;
        cs.label = 'A' + c;
        cs.centroid = centroids[c];
        cs.count = 0;
        cs.dispersion = 0.0;
        summaryOut.push_back(cs);
    }

    for (int i = 0; i < num_points; ++i) {
        int c = assignments[i];
        summaryOut[c].count++;
        double d = calculateDistance(data[i], summaryOut[c].centroid);
        summaryOut[c].dispersion += (d * d); 
    }

    for (int c = 0; c < k; ++c) {
        if (summaryOut[c].count > 0) {
            summaryOut[c].dispersion /= summaryOut[c].count; // Promedio (MSE)
        }
    }

    return result;
}