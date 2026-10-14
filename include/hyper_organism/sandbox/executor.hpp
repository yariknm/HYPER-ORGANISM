#ifndef HYPER_ORGANISM_SANDBOX_EXECUTOR_HPP
#define HYPER_ORGANISM_SANDBOX_EXECUTOR_HPP

#include "core/types.hpp"
#include <cstdint>
#include <cstddef>
#include <memory>

namespace hyper_organism {
namespace sandbox {

class Executor {
public:
    virtual ~Executor() = default;
    
    // Execute machine code in an isolated environment
    // Returns fitness score; throws on timeout or crash
    virtual FitnessScore execute(
        const MachineCode& code,
        const void* input_data,
        size_t input_size,
        uint64_t timeout_cycles = 1000000
    ) = 0;
    
    // Check if this executor supports the given ISA
    virtual bool supports(ISA arch) const = 0;
    
    // Get sandbox type name
    virtual std::string type_name() const = 0;
};

std::unique_ptr<Executor> create_executor(ISA arch);

} // namespace sandbox
} // namespace hyper_organism

#endif // HYPER_ORGANISM_SANDBOX_EXECUTOR_HPP