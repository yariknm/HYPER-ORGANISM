#include "hyper_organism/core/types.hpp"
#include "hyper_organism/pool/genetic_pool.hpp"
#include "hyper_organism/ecology/ecosystem.hpp"
#include "hyper_organism/garbage/collector.hpp"
#include "hyper_organism/distributed/node_controller.hpp"
#include "hyper_organism/distributed/rdma_bridge.hpp"
#include "hyper_organism/io/serializer.hpp"
#include "hyper_organism/immune/system.hpp"
#include "hyper_organism/environment/dynamic.hpp"
#include "hyper_organism/mutation/genetic_control.hpp"
#include "hyper_organism/visualization/visualizer.hpp"

#include <iostream>
#include <vector>
#include <cstring>
#include <chrono>
#include <fstream>

using namespace hyper_organism;

void print_banner() {
    std::cout << R"(
╔══════════════════════════════════════════════════════════╗
║           HYPER-ORGANISM v1.0.0                          ║
║   Distributed Autonomous Cluster Agent Synthesizer       ║
║   Digital Biology in Silicon                             ║
╚══════════════════════════════════════════════════════════╝
)" << std::endl;
}

void run_evolution_cycle() {
    std::cout << "=== Starting Evolution Cycle ===" << std::endl;
    
    const size_t POPULATION_SIZE = 100;
    const size_t GENERATIONS = 50;
    const ISA ARCH = ISA::X86_64;
    
    GeneticPool pool(POPULATION_SIZE, ARCH);
    pool.initialize();
    
    ecology::Ecosystem ecosystem;
    garbage::GarbageCollector gc;
    immune::EncryptionKey master_key = immune::generate_key();
    environment::DynamicEnvironment env;
    mutation::GeneticMutationController mutation_controller;
    visualization::ASCIIVisualizer viz;
    visualization::ChampionExporter exporter;
    
    std::vector<double> input_data(256);
    for (size_t i = 0; i < input_data.size(); ++i) {
        input_data[i] = static_cast<double>(i) / 256.0;
    }
    
    std::cout << "Population: " << POPULATION_SIZE << " organisms" << std::endl;
    std::cout << "Generations: " << GENERATIONS << std::endl;
    std::cout << "Target: x86-64 AVX-512" << std::endl;
    std::cout << std::endl;
    
    for (size_t gen = 0; gen < GENERATIONS; ++gen) {
        env.update(gen);
        
        pool.evaluate_all(input_data.data(), input_data.size());
        
        ecosystem.cull_dead();
        double avg_fitness = pool.population().avg_fitness;
        
        for (auto& org : pool.population().individuals) {
            ecology::EcologicalOrganism eco_org;
            eco_org.core = org;
            eco_org.species = ecology::classify_by_fitness(org, avg_fitness);
            eco_org.energy = org.fitness.composite * 100.0;
            ecosystem.add_organism(eco_org);
        }
        
        // Immune system: check for parasites and self-repair
        for (auto& org : pool.population().individuals) {
            uint32_t checksum = immune::compute_checksum(org.code.bytes);
            if (!immune::self_repair(org, checksum)) {
                immune::self_destruct(org);
            }
        }
        
        // Genetic mutation control: apply genomic mutation rates
        for (auto& org : pool.population().individuals) {
            mutation_controller.apply_genomic_mutation(org, 0.1);
        }
        
        pool.evolve();
        
        // Collect dead organisms as humus
        std::vector<Organism> dead;
        for (const auto& eco_org : ecosystem.organisms()) {
            if (eco_org.energy <= 0) {
                dead.push_back(eco_org.core);
            }
        }
        gc.collect(dead);
        
        // Catastrophe handling
        if (env.is_catastrophe_due() && gen > 0 && gen % 100 == 0) {
            env.trigger_catastrophe();
            std::cout << "  *** CATASTROPHE at gen " << gen << " ***" << std::endl;
        }
        
        if (gen % 10 == 0 || gen == GENERATIONS - 1) {
            std::cout << "Gen " << gen 
                      << " | Best: " << pool.best_fitness()
                      << " | Avg: " << pool.population().avg_fitness
                      << " | Diversity: " << pool.population().diversity
                      << " | Ecosystem: " << ecosystem.population() << " organisms"
                      << " | Humus: " << gc.humus_size() << " fragments"
                      << " | Pressure: " << env.current_pressure()
                      << std::endl;
        }
    }
    
    std::cout << std::endl;
    std::cout << "=== Evolution Complete ===" << std::endl;
    std::cout << "Best fitness: " << pool.best_fitness() << std::endl;
    std::cout << "Final generation: " << pool.generation() << std::endl;
    std::cout << "Ecosystem biodiversity: " << ecosystem.biodiversity_index() << std::endl;
    std::cout << "Digital humus collected: " << gc.humus_size() << " fragments" << std::endl;
    
    // Visualize final population
    std::cout << "\n" << viz.render_population(pool.population().individuals) << std::endl;
    std::cout << viz.render_memory_map(pool.population().individuals) << std::endl;
    std::cout << viz.render_fitness_landscape(pool.population().individuals) << std::endl;
    
    // Export champion
    std::cout << "Exporting champion..." << std::endl;
    exporter.export_to_asm(pool.best(), "champion.asm");
    exporter.export_to_bin(pool.best(), "champion.bin");
    exporter.export_to_c_header(pool.best(), "champion.h");
    std::cout << "Champion exported to champion.asm, champion.bin, champion.h" << std::endl;
    
    // Serialize best organism
    std::cout << std::endl << "Best organism:" << std::endl;
    io::serialize_organism(std::cout, pool.best());
    std::cout << std::endl;
}

void run_cluster_simulation() {
    std::cout << std::endl;
    std::cout << "=== Cluster Simulation ===" << std::endl;
    
    distributed::RDMABridge bridge(0, "127.0.0.1");
    bridge.connect({"127.0.0.1:5001", "127.0.0.1:5002", "127.0.0.1:5003"});
    
    std::cout << "Node 0 connected to cluster" << std::endl;
    
    Organism migrant;
    migrant.code.bytes.resize(32);
    for (size_t i = 0; i < migrant.code.bytes.size(); ++i) {
        migrant.code.bytes[i] = static_cast<uint8_t>(i * 7);
    }
    migrant.code.bytes.back() = 0xC3;
    migrant.generation = 5;
    migrant.target_arch = ISA::X86_64;
    
    std::cout << "Migrating organism (gen " << migrant.generation << ") to node 1..." << std::endl;
    bridge.broadcast_organism(migrant);
    
    auto received = bridge.receive_messages(100);
    std::cout << "Received " << received.size() << " messages from cluster" << std::endl;
    
    bridge.disconnect();
}

int main() {
    print_banner();
    
    std::cout << "HYPER-ORGANISM: Digital Evolution in Silicon" << std::endl;
    std::cout << "==============================================" << std::endl;
    
    auto start = std::chrono::high_resolution_clock::now();
    
    run_evolution_cycle();
    run_cluster_simulation();
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    
    std::cout << std::endl;
    std::cout << "Total execution time: " << duration.count() << " ms" << std::endl;
    std::cout << "HYPER-ORGANISM evolution cycle complete." << std::endl;
    
    return 0;
}