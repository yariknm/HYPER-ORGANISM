#include "hyper_organism/pool/genetic_pool.hpp"
#include "hyper_organism/core/types.hpp"
#include "hyper_organism/isa/backend.hpp"

#include <vector>
#include <algorithm>
#include <random>
#include <chrono>
#include <iostream>
#include <cstring>

namespace hyper_organism {

GeneticPool::GeneticPool(size_t population_size, ISA arch)
    : population_size_(population_size), arch_(arch),
      mutator_(mutation::create_mutator(arch)),
      crossover_(crossover::create_crossover(arch)),
      generation_(0), rng_(std::random_device{}()) {
    population_.individuals.reserve(population_size);
    
    auto backend = isa::create_backend(arch);
    if (backend) {
        executor_ = backend->create_executor();
    }
}

void GeneticPool::initialize() {
    for (size_t i = 0; i < population_size_; ++i) {
        Organism org;
        size_t code_size = 16 + (rng_() % 64);
        org.code.bytes.resize(code_size);
        for (size_t j = 0; j < code_size; ++j) {
            org.code.bytes[j] = static_cast<uint8_t>(rng_() % 256);
        }
        org.code.bytes.back() = 0xC3;
        org.code.target_isa = arch_;
        org.generation = 0;
        population_.individuals.push_back(std::move(org));
    }
    generation_ = 0;
}

void GeneticPool::evaluate_all(const double* input_data, size_t input_size) {
    if (!executor_) {
        for (auto& org : population_.individuals) {
            org.fitness.correctness = 0.0;
            org.fitness.composite = 0.0;
        }
        return;
    }
    
    for (auto& org : population_.individuals) {
        try {
            org.fitness = executor_->execute(org.code, input_data, input_size);
        } catch (...) {
            org.fitness.correctness = 0.0;
            org.fitness.composite = 0.0;
        }
    }
    update_statistics();
}

void GeneticPool::evolve() {
    std::vector<Organism> new_population;
    new_population.reserve(population_size_);
    
    size_t elite_count = std::min(population_size_ / 10, population_.individuals.size());
    std::sort(population_.individuals.begin(), population_.individuals.end(),
              [](const Organism& a, const Organism& b) {
                  return a.fitness.composite > b.fitness.composite;
              });
    
    for (size_t i = 0; i < elite_count; ++i) {
        new_population.push_back(population_.individuals[i]);
    }
    
    while (new_population.size() < population_size_ && !population_.individuals.empty()) {
        auto& parent_a = tournament_select();
        auto& parent_b = tournament_select();
        
        auto result = crossover_->cross(parent_a, parent_b, rng_());
        
        if (result.success) {
            Organism child = mutator_->mutate(result.child_a, rng_());
            child.parent_a = 0;
            child.parent_b = 0;
            new_population.push_back(std::move(child));
            
            if (new_population.size() < population_size_) {
                Organism child2 = mutator_->mutate(result.child_b, rng_());
                child2.parent_a = 0;
                child2.parent_b = 0;
                new_population.push_back(std::move(child2));
            }
        }
    }
    
    population_.individuals = std::move(new_population);
    generation_++;
    update_statistics();
}

const Organism& GeneticPool::best() const {
    if (population_.individuals.empty()) {
        static Organism dummy;
        return dummy;
    }
    return *std::max_element(population_.individuals.begin(), population_.individuals.end(),
                             [](const Organism& a, const Organism& b) {
                                 return a.fitness.composite < b.fitness.composite;
                             });
}

const Population& GeneticPool::population() const { return population_; }
Population& GeneticPool::population() { return population_; }
size_t GeneticPool::generation() const { return generation_; }
double GeneticPool::best_fitness() const { return population_.best_fitness; }
double GeneticPool::diversity() const { return population_.diversity; }

void GeneticPool::set_mutation_rate(double rate) { mutator_->set_rate(rate); }
void GeneticPool::set_crossover_rate(double rate) { crossover_->set_rate(rate); }

void GeneticPool::print_statistics() const {
    std::cout << "Generation " << generation_ 
              << " | Best: " << population_.best_fitness
              << " | Avg: " << population_.avg_fitness
              << " | Diversity: " << population_.diversity << std::endl;
}

Organism& GeneticPool::tournament_select() {
    if (population_.individuals.empty()) {
        static Organism dummy;
        return dummy;
    }
    
    size_t tournament_size = std::min(size_t(5), population_.individuals.size());
    size_t best_idx = rng_() % population_.individuals.size();
    for (size_t i = 1; i < tournament_size; ++i) {
        size_t idx = rng_() % population_.individuals.size();
        if (population_.individuals[idx].fitness.composite > 
            population_.individuals[best_idx].fitness.composite) {
            best_idx = idx;
        }
    }
    return population_.individuals[best_idx];
}

void GeneticPool::update_statistics() {
    if (population_.individuals.empty()) {
        population_.best_fitness = 0.0;
        population_.avg_fitness = 0.0;
        population_.diversity = 0.0;
        return;
    }
    
    double sum = 0.0;
    double best = 0.0;
    for (const auto& org : population_.individuals) {
        sum += org.fitness.composite;
        best = std::max(best, org.fitness.composite);
    }
    population_.avg_fitness = sum / population_.individuals.size();
    population_.best_fitness = best;
    
    double total_dist = 0.0;
    size_t count = 0;
    for (size_t i = 0; i < population_.individuals.size(); i += 10) {
        for (size_t j = i + 1; j < population_.individuals.size(); j += 10) {
            size_t dist = 0;
            size_t min_size = std::min(population_.individuals[i].code.bytes.size(),
                                      population_.individuals[j].code.bytes.size());
            for (size_t k = 0; k < min_size; ++k) {
                if (population_.individuals[i].code.bytes[k] != 
                    population_.individuals[j].code.bytes[k]) {
                    dist++;
                }
            }
            total_dist += static_cast<double>(dist) / min_size;
            count++;
        }
    }
    population_.diversity = count > 0 ? total_dist / count : 0.0;
}

} // namespace hyper_organism