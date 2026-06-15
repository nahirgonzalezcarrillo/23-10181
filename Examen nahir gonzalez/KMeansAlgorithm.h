#ifndef KMEANSALGORITHM_H
#define KMEANSALGORITHM_H
#include "types.h"

class KMeansAlgorithm : public IClusterAlgorithm {
private:
    double calculateDistance(const Coord_3D& p1, const Coord_3D& p2);
    bool hasConverged(const std::vector<Coord_3D>& oldC, const std::vector<Coord_3D>& newC, double tolerance);
public:
    std::vector<Labeled> fitPredict(const std::vector<Coord_3D>& data, int k, std::vector<ClusterSummary>& summaryOut) override;
};
#endif