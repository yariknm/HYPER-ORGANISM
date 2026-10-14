#ifndef HYPER_ORGANISM_DISTRIBUTED_NODE_CONTROLLER_HPP
#define HYPER_ORGANISM_DISTRIBUTED_NODE_CONTROLLER_HPP

#include "core/types.hpp"
#include "pool/genetic_pool.hpp"
#include <memory>
#include <atomic>

namespace hyper_organism {
namespace distributed {

class NodeController {
public:
    NodeController(size_t population_size, ISA arch, int node_id = 0);
    ~NodeController();
    
    void initialize();
    void run(size_t generations, const double* input_data, size_t input_size);
    void stop();
    
    const GeneticPool& pool() const;
    GeneticPool& pool();
    int node_id() const;
    
    void sync_with_peers();
    void receive_organism(const Organism& org);

private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
};

} // namespace distributed
} // namespace hyper_organism

#endif // HYPER_ORGANISM_DISTRIBUTED_NODE_CONTROLLER_HPP