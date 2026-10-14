#include "hyper_organism/distributed/node_controller.hpp"
#include "hyper_organism/pool/genetic_pool.hpp"

#include <vector>
#include <iostream>
#include <atomic>

namespace hyper_organism {
namespace distributed {

struct NodeController::Impl {
    std::unique_ptr<GeneticPool> pool;
    int node_id;
    ISA arch;
    std::atomic<bool> running;
    
    Impl(size_t population_size, ISA arch, int node_id)
        : pool(std::make_unique<GeneticPool>(population_size, arch)),
          node_id(node_id), arch(arch), running(false) {}
};

NodeController::NodeController(size_t population_size, ISA arch, int node_id)
    : impl_(std::make_unique<Impl>(population_size, arch, node_id)) {}

NodeController::~NodeController() = default;

void NodeController::initialize() {
    impl_->pool->initialize();
    std::cout << "Node " << impl_->node_id << " initialized" << std::endl;
}

void NodeController::run(size_t generations, const double* input_data, size_t input_size) {
    impl_->running = true;
    for (size_t i = 0; i < generations && impl_->running; ++i) {
        impl_->pool->evaluate_all(input_data, input_size);
        impl_->pool->evolve();
        
        if (i % 10 == 0) {
            std::cout << "Node " << impl_->node_id << " gen " << i << ": ";
            impl_->pool->print_statistics();
        }
        
        if (i % 100 == 0 && i > 0) {
            sync_with_peers();
        }
    }
    impl_->running = false;
}

void NodeController::stop() { impl_->running = false; }

const GeneticPool& NodeController::pool() const { return *impl_->pool; }
GeneticPool& NodeController::pool() { return *impl_->pool; }
int NodeController::node_id() const { return impl_->node_id; }

void NodeController::sync_with_peers() {
    std::cout << "Node " << impl_->node_id << " syncing with peers..." << std::endl;
}

void NodeController::receive_organism(const Organism& org) {
    impl_->pool->population().individuals.push_back(org);
}

} // namespace distributed
} // namespace hyper_organism