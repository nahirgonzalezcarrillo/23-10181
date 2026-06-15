#ifndef DATAOUTPUT_H
#define DATAOUTPUT_H
#include "types.h"

class DataOutput : public IDataOutput {
public:
    void saveClassified(const std::vector<Labeled>& data, const std::string& filename) override;
    void saveSummary(const std::vector<ClusterSummary>& summary, const std::string& filename) override;
};
#endif