#ifndef HYPER_ORGANISM_ISA_BACKEND_HPP
#define HYPER_ORGANISM_ISA_BACKEND_HPP

#include "hyper_organism/core/types.hpp"
#include "hyper_organism/sandbox/executor.hpp"
#include <vector>
#include <memory>

namespace hyper_organism {
namespace isa {

class Backend {
public:
    virtual ~Backend() = default;
    
    virtual ISA id() const = 0;
    virtual std::string name() const = 0;
    virtual bool supports_vector_ext() const = 0;
    virtual size_t vector_register_width() const = 0;
    virtual size_t l1_cache_size() const = 0;
    virtual size_t l2_cache_size() const = 0;
    virtual size_t l3_cache_size() const = 0;
    virtual uint8_t pipeline_width() const = 0;
    virtual uint8_t pipeline_latency() const = 0;
    
    virtual std::vector<uint8_t> encode(const Instruction& inst) = 0;
    virtual std::vector<Instruction> decode(const uint8_t* code, size_t size) = 0;
    virtual uint8_t get_vector_op(uint8_t scalar_op) = 0;
    virtual bool is_memory_op(uint8_t opcode) = 0;
    virtual uint8_t get_latency(uint8_t opcode) = 0;
    virtual uint8_t get_throughput(uint8_t opcode) = 0;
    
    // Create ISA-specific executor
    virtual std::unique_ptr<sandbox::Executor> create_executor() = 0;
};

std::unique_ptr<Backend> create_backend(ISA arch);

} // namespace isa
} // namespace hyper_organism

#endif // HYPER_ORGANISM_ISA_BACKEND_HPP