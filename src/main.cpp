#include "hyper_organism/core/types.hpp"
#include "hyper_organism/pool/genetic_pool.hpp"
#include "hyper_organism/distributed/node_controller.hpp"
#include "hyper_organism/io/serializer.hpp"

#include <iostream>
#include <vector>
#include <cstring>

void print_banner() {
    std::cout << R"(
╔══════════════════════════════════════════════════════════╗
║           HYPER-ORGANISM v1.0.0                          ║
║   Distributed Autonomous Cluster Agent Synthesizer       ║
╚══════════════════════════════════════════════════════════╝
)" << std::endl;
}

int main(int argc, char** argv) {
    print_banner();
    
    std::cout << "Initializing HYPER-ORGANISM cluster..." << std::endl;
    
    // Configuration
    const size_t POPULATION_SIZE = 100;
    const size_t GENERATIONS = 50;
    const ISA ARCH = ISA::X86_64;
    
    // Create node controller
    hyper_organism::distributed::NodeController controller(POPULATION_SIZE, ARCH, 0);
    
    // Initialize with sample input data
    std::vector<double> input_data(256);
    for (size_t i = 0; i < input_data.size(); ++i) {
        input_data[i] = static_cast<double>(i) / 256.0;
    }
    
    // Initialize the genetic pool
    controller.initialize();
    
    std::cout << "Starting evolution for " << GENERATIONS << " generations..." << std::endl;
    std::cout << "Population size: " << POPULATION_SIZE << std::endl;
    std::cout << "Target architecture: x86-64 AVX-512" << std::endl;
    
    // Run evolution
    controller.run(GENERATIONS, input_data.data(), input_data.size());
    
    // Report results
    std::cout << "\n=== Evolution Complete ===" << std::endl;
    std::cout << "Best fitness: " << controller.pool().best_fitness() << std::endl;
    std::cout << "Generation: " << controller.pool().generation() << std::endl;
    std::cout << "Diversity: " << controller.pool().diversity() << std::endl;
    
    // Serialize best organism
    hyper_organism::io::serialize_organism(std::cout, controller.pool().best());
    
    std::cout << "\nHYPER-ORGANISM evolution cycle complete." << std::endl;
    return 0;
}