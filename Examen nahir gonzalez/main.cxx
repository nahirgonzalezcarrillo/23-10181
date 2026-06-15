#include <iostream>
#include <string>
#include <vector>
#include "types.h"
#include "DataLoader.h"
#include "KMeansAlgorithm.h"
#include "DataOutput.h"

int main(int argc, char* argv[]) {
    // Validar invocación de "cluster <k> <datos>"
    if (argc != 3) {
        std::cerr << "Uso: cluster <k> <datos>\n";
        std::cerr << "Ejemplo: cluster 3 datos.csv\n";
        return 1;
    }

    int k = std::stoi(argv[1]);
    std::string filename = argv[2];

    if (k <= 0 || k > 6) {
        std::cerr << "Error: No se recomiendan mas de 6 clusters.\n";
        return 1;
    }

    // Usando Polimorfismo e interfaces 
    IDataLoader* loader = new DataLoader();
    IClusterAlgorithm* kmeans = new KMeansAlgorithm();
    IDataOutput* output = new DataOutput();

    try {
        // 1. Carga (Vector const para evitar su modificacion)
        const std::vector<Coord_3D> data = loader->load(filename);

        // 2. Ejecucion
        std::vector<ClusterSummary> summary;
        std::vector<Labeled> classifiedData = kmeans->fitPredict(data, k, summary);

        // 3. Escritura
        output->saveClassified(classifiedData, "clasificados.csv");
        output->saveSummary(summary, "summary.txt");

        std::cout << "Clustering completado exitosamente.\n";

    } catch (const std::exception& e) {
        std::cerr << "Excepcion: " << e.what() << "\n";
    }

    delete loader;
    delete kmeans;
    delete output;

    return 0;
}