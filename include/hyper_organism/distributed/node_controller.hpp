#ifndef HYPER_ORGANISM_DISTRIBUTED_NODE_CONTROLLER_HPP
#define HYPER_ORGANISM_DISTRIBUTED_NODE_CONTROLLER_HPP
#include "hyper_organism/core/types.hpp"
#include "hyper_organism/pool/genetic_pool.hpp"
#include <memory>
#include <atomic>
namespace hyper_organism { namespace distributed {
class NodeController {
public:
    NodeController(size_t=100, ISA=ISA::X86_64, int=0);
    ~NodeController();
    void initialize();
    void run(size_t, const double*, size_t);
    void stop();
    GeneticPool& pool();
    const GeneticPool& pool() const;
    int node_id() const;
    void sync_with_peers();
    void receive_organism(const Organism&);
private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
};
} }
#endif
