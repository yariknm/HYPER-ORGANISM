#ifndef HYPER_ORGANISM_MUTATION_MUTATOR_HPP
#define HYPER_ORGANISM_MUTATION_MUTATOR_HPP

#include "hyper_organism/core/types.hpp"
#include <memory>

namespace hyper_organism {
namespace mutation {

class Mutator {
public:
    virtual ~Mutator() = default;
    
    // Apply mutations to an organism, producing a new one
    virtual Organism mutate(const Organism& parent, uint64_t seed) = 0;
    
    // Set mutation rate (0.0 to 1.0)
    virtual void set_rate(double rate) = 0;
    virtual double rate() const = 0;
    
    // Get mutation type name
    virtual std::string type_name() const = 0;
};

std::unique_ptr<Mutator> create_mutator(ISA arch);

} // namespace mutation
} // namespace hyper_organism

#endif // HYPER_ORGANISM_MUTATION_MUTATOR_HPP