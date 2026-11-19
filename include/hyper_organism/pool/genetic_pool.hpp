#ifndef HYPER_ORGANISM_POOL_GENETIC_POOL_HPP
#define HYPER_ORGANISM_POOL_GENETIC_POOL_HPP

#include "hyper_organism/core/types.hpp"
#include "hyper_organism/mutation/mutator.hpp"
#include "hyper_organism/crossover/crossover.hpp"
#include "hyper_organism/sandbox/executor.hpp"
#include <memory>
#include <random>

namespace hyper_organism {

class GeneticPool {
public:
    GeneticPool(size_t population_size, ISA arch);
    ~GeneticPool() = default;
    
    void initialize();
    void evaluate_all(const double* input_data, size_t input_size);
    void evolve();
    
    const Organism& best() const;
    const Population& population() const;
    Population& population();
    size_t generation() const;
    double best_fitness() const;
    double diversity() const;
    
    void set_mutation_rate(double rate);
    void set_crossover_rate(double rate);
    void print_statistics() const;

private:
    Organism& tournament_select();
    void update_statistics();
    
    size_t population_size_;
    ISA arch_;
    Population population_;
    std::unique_ptr<mutation::Mutator> mutator_;
    std::unique_ptr<crossover::Crossover> crossover_;
    std::unique_ptr<sandbox::Executor> executor_;
    size_t generation_;
    std::mt19937 rng_;
};

} // namespace hyper_organism

#endif // HYPER_ORGANISM_POOL_GENETIC_POOL_HPP