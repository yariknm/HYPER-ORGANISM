#ifndef HYPER_ORGANISM_CROSSOVER_CROSSOVER_HPP
#define HYPER_ORGANISM_CROSSOVER_CROSSOVER_HPP

#include "core/types.hpp"
#include <vector>

namespace hyper_organism {
namespace crossover {

struct CrossoverResult {
    Organism child_a;
    Organism child_b;
    bool success;
};

class Crossover {
public:
    virtual ~Crossover() = default;
    
    // Perform crossover between two parent organisms
    virtual CrossoverResult cross(const Organism& a, const Organism& b, uint64_t seed) = 0;
    
    // Set crossover rate
    virtual void set_rate(double rate) = 0;
    virtual double rate() const = 0;
    
    virtual std::string type_name() const = 0;
};

std::unique_ptr<Crossover> create_crossover(ISA arch);

} // namespace crossover
} // namespace hyper_organism

#endif // HYPER_ORGANISM_CROSSOVER_CROSSOVER_HPP