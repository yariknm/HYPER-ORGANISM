#ifndef HYPER_ORGANISM_CORE_TYPES_HPP
#define HYPER_ORGANISM_CORE_TYPES_HPP

#include <cstdint>
#include <cstddef>
#include <vector>
#include <array>
#include <string>
#include <memory>
#include <functional>

namespace hyper_organism {

constexpr size_t MAX_CODE_SIZE = 4096;
constexpr size_t MAX_INSTRUCTIONS = 512;
constexpr size_t DEFAULT_CODE_SIZE = 512;
constexpr size_t MAX_GENOME_SIZE = 16384;

enum class ISA : uint8_t {
    X86_64 = 0,
    ARM64 = 1,
    UNKNOWN = 255
};

enum class OperandType : uint8_t {
    REGISTER = 0,
    IMMEDIATE = 1,
    MEMORY = 2,
    LABEL = 3,
    NONE = 4
};

struct Operand {
    OperandType type;
    uint32_t value;
    uint8_t reg;
    int32_t offset;
};

struct Instruction {
    uint64_t offset;        // offset in code buffer
    uint8_t opcode;
    uint8_t length;
    std::array<Operand, 4> operands;
    std::vector<uint32_t> dependencies;  // indices of instructions this depends on
    std::vector<uint32_t> dependents;    // indices of instructions that depend on this
    bool is_vector;
    bool is_memory_access;
    uint8_t pipeline_latency;  // cycles
    uint8_t pipeline_throughput; // cycles
};

struct DependencyGraph {
    std::vector<Instruction> nodes;
    std::vector<std::vector<uint32_t>> edges;  // adjacency list
    std::vector<uint32_t> topological_order;
    
    void build();
    void compute_critical_path();
    size_t critical_path_length() const { return critical_path_; }
    
private:
    size_t critical_path_ = 0;
};

struct MachineCode {
    std::array<uint8_t, MAX_CODE_SIZE> bytes;
    size_t size;
    ISA target_isa;
    
    MachineCode() : size(0), target_isa(ISA::UNKNOWN) {}
    
    bool valid() const { return size > 0 && size <= MAX_CODE_SIZE; }
};

struct FitnessScore {
    double cycles;
    double throughput;
    double correctness;
    double energy_estimate;
    double composite;
    
    FitnessScore() : cycles(0), throughput(0), correctness(1.0), energy_estimate(0), composite(0) {}
    
    bool is_valid() const { return correctness > 0.95; }
    double to_rank() const {
        // Lower cycles is better; composite is weighted
        return correctness * (1.0 / (1.0 + cycles / 1000.0)) * throughput;
    }
};

struct Organism {
    MachineCode code;
    FitnessScore fitness;
    uint64_t generation;
    uint64_t parent_a;
    uint64_t parent_b;
    ISA target_arch;
    std::string hash;
    std::vector<uint8_t> metadata;
    
    Organism() : generation(0), parent_a(0), parent_b(0), target_arch(ISA::UNKNOWN) {}
    
    bool is_valid() const {
        return code.valid() && fitness.is_valid();
    }
};

struct Population {
    std::vector<Organism> individuals;
    size_t generation;
    double best_fitness;
    double avg_fitness;
    double diversity;
};

} // namespace hyper_organism

#endif // HYPER_ORGANISM_CORE_TYPES_HPP