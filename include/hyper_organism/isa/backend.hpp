#ifndef HYPER_ORGANISM_ISA_BACKEND_HPP
#define HYPER_ORGANISM_ISA_BACKEND_HPP

#include "core/types.hpp"
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
    virtual size_t vector_register_width() const = 0;  // in bits
    virtual size_t l1_cache_size() const = 0;         // in bytes
    virtual size_t l2_cache_size() const = 0;
    virtual size_t l3_cache_size() const = 0;
    virtual uint8_t pipeline_width() const = 0;       // instructions per cycle
    virtual uint8_t pipeline_latency() const = 0;     // base latency
    
    // Encode an instruction into machine code
    virtual std::vector<uint8_t> encode(const Instruction& inst) = 0;
    
    // Decode machine code into instructions
    virtual std::vector<Instruction> decode(const uint8_t* code, size_t size) = 0;
    
    // Get vector instruction opcode for given operation
    virtual uint8_t get_vector_op(uint8_t scalar_op) = 0;
    
    // Check if instruction is a memory access
    virtual bool is_memory_op(uint8_t opcode) = 0;
    
    // Get pipeline characteristics for an opcode
    virtual uint8_t get_latency(uint8_t opcode) = 0;
    virtual uint8_t get_throughput(uint8_t opcode) = 0;
};

std::unique_ptr<Backend> create_backend(ISA arch);

} // namespace isa
} // namespace hyper_organism

#endif // HYPER_ORGANISM_ISA_BACKEND_HPP