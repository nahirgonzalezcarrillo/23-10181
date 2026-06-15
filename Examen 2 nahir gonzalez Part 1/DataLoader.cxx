#include "DataLoader.h"
#include <fstream>
#include <sstream>
#include <stdexcept>

std::vector<Coord_3D> DataLoader::load(const std::string& filename) {
    std::vector<Coord_3D> data;
    std::ifstream file(filename);
    if (!file.is_open()) throw std::runtime_error("No se pudo abrir el archivo de datos.");

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string token;
        Coord_3D p;
        if (std::getline(ss, token, ',')) p.x = std::stod(token);
        if (std::getline(ss, token, ',')) p.y = std::stod(token);
        if (std::getline(ss, token, ',')) p.z = std::stod(token);
        data.push_back(p);
    }
    return data;
}