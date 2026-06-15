#include "DataOutput.h"
#include <fstream>
#include <iostream>

void DataOutput::saveClassified(const std::vector<Labeled>& data, const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) return;
    for (const auto& item : data) {
        file << item.coord.x << "," << item.coord.y << "," << item.coord.z << "," << item.label << "\n";
    }
}

void DataOutput::saveSummary(const std::vector<ClusterSummary>& summary, const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) return;
    for (const auto& item : summary) {
        // Molde: A: N, (x, y, z), MD
        file << item.label << ": " << item.count << ", ("
             << item.centroid.x << ", " << item.centroid.y << ", " << item.centroid.z << "), "
             << item.dispersion << "\n";
    }
}