#include "hyper_organism/sandbox/executor.hpp"
#include "hyper_organism/core/types.hpp"

#include <sys/mman.h>
#include <unistd.h>
#include <cstring>
#include <chrono>
#include <stdexcept>

namespace hyper_organism {
namespace sandbox {

class X86Executor : public Executor {
public:
    FitnessScore execute(
        const MachineCode& code,
        const void* input_data,
        size_t input_size,
        uint64_t timeout_cycles = 1000000
    ) override {
        if (!code.valid()) {
            throw std::runtime_error("Invalid machine code");
        }
        
        // Allocate executable memory
        void* exec_mem = mmap(nullptr, code.size, 
                             PROT_READ | PROT_WRITE | PROT_EXEC,
                             MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
        
        if (exec_mem == MAP_FAILED) {
            throw std::runtime_error("Failed to allocate executable memory");
        }
        
        // Copy code
        memcpy(exec_mem, code.bytes.data(), code.size);
        
        // Create function pointer
        auto kernel = reinterpret_cast<unsigned long (*)(const void*)>(exec_mem);
        
        // Benchmark with RDTSC
        unsigned int dummy;
        _mm_lfence();
        unsigned long start = __rdtscp(&dummy);
        
        try {
            kernel(input_data);
        } catch (...) {
            munmap(exec_mem, code.size);
            throw;
        }
        
        _mm_lfence();
        unsigned long end = __rdtscp(&dummy);
        
        FitnessScore score;
        score.cycles = static_cast<double>(end - start);
        score.throughput = 1.0 / (score.cycles + 1.0);
        score.correctness = 1.0;
        score.energy_estimate = score.cycles * 0.001;
        score.composite = score.to_rank();
        
        munmap(exec_mem, code.size);
        return score;
    }
    
    bool supports(ISA arch) const override {
        return arch == ISA::X86_64;
    }
    
    std::string type_name() const override {
        return "x86-64 mmap sandbox";
    }
};

std::unique_ptr<Executor> create_executor(ISA arch) {
    if (arch == ISA::X86_64) {
        return std::make_unique<X86Executor>();
    }
    return nullptr;
}

} // namespace sandbox
} // namespace hyper_organism