#ifndef TYPES_H
#define TYPES_H

#include <vector>
#include <string>

struct Coord_3D {
    double x;
    double y;
    double z;
};

struct Labeled {
    Coord_3D coord;
    char label;
};

struct ClusterSummary {
    char label;
    int count;
    Coord_3D centroid;
    double dispersion;
};

// --- INTERFACES ---
class IDataLoader {
public:
    virtual std::vector<Coord_3D> load(const std::string& filename) = 0;
    virtual ~IDataLoader() = default;
};

class IClusterAlgorithm {
public:
    virtual std::vector<Labeled> fitPredict(const std::vector<Coord_3D>& data, int k, std::vector<ClusterSummary>& summaryOut) = 0;
    virtual ~IClusterAlgorithm() = default;
};

class IDataOutput {
public:
    virtual void saveClassified(const std::vector<Labeled>& data, const std::string& filename) = 0;
    virtual void saveSummary(const std::vector<ClusterSummary>& summary, const std::string& filename) = 0;
    virtual ~IDataOutput() = default;
};

#endif