#include "hyper_organism/sandbox/executor.hpp"
#include "hyper_organism/core/types.hpp"

#include <sys/mman.h>
#include <unistd.h>
#include <signal.h>
#include <setjmp.h>
#include <cstring>
#include <chrono>
#include <stdexcept>

#ifdef __x86_64__
#include <x86intrin.h>
#include <immintrin.h>
#endif

namespace hyper_organism {
namespace sandbox {

// Default executor factory - implementations are in ISA backends
std::unique_ptr<Executor> create_executor(ISA arch) {
    // Delegate to ISA-specific factory
    return nullptr;
}

} // namespace sandbox
} // namespace hyper_organism