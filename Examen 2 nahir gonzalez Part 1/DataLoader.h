#ifndef DATALOADER_H
#define DATALOADER_H
#include "types.h"

class DataLoader : public IDataLoader {
public:
    std::vector<Coord_3D> load(const std::string& filename) override;
};
#endif