#include "hyper_organism/isa/backend.hpp"
#include "hyper_organism/core/types.hpp"
#include "hyper_organism/sandbox/executor.hpp"

#include <cstring>
#include <vector>
#include <stdexcept>
#include <memory>
#include <chrono>
#include <random>

namespace hyper_organism {
namespace isa {

class X86Backend : public Backend {
public:
    ISA id() const override { return ISA::X86_64; }
    std::string name() const override { return "x86-64 AVX-512"; }
    bool supports_vector_ext() const override { return true; }
    size_t vector_register_width() const override { return 512; }
    size_t l1_cache_size() const override { return 32768; }
    size_t l2_cache_size() const override { return 1048576; }
    size_t l3_cache_size() const override { return 33554432; }
    uint8_t pipeline_width() const override { return 4; }
    uint8_t pipeline_latency() const override { return 4; }
    
    std::vector<uint8_t> encode(const Instruction& inst) override {
        std::vector<uint8_t> result;
        switch (inst.opcode) {
            case 0x01: result.push_back(0x01); break;
            case 0x02: result.push_back(0x8B); break;
            case 0x03: result.push_back(0x31); break;
            case 0xFF: result.push_back(0xC3); break;
            default: result.push_back(0x90); break;
        }
        return result;
    }
    
    std::vector<Instruction> decode(const uint8_t* code, size_t size) override {
        std::vector<Instruction> result;
        for (size_t i = 0; i < size; ++i) {
            Instruction inst;
            inst.offset = i;
            inst.opcode = code[i];
            inst.length = 1;
            inst.is_vector = false;
            inst.is_memory_access = false;
            inst.pipeline_latency = 4;
            inst.pipeline_throughput = 1;
            result.push_back(inst);
        }
        return result;
    }
    
    uint8_t get_vector_op(uint8_t scalar_op) override {
        switch (scalar_op) {
            case 0x01: return 0x01;
            case 0x02: return 0x02;
            case 0x03: return 0x03;
            default: return scalar_op;
        }
    }
    
    bool is_memory_op(uint8_t opcode) override {
        return (opcode & 0x07) == 0x05 || (opcode & 0x07) == 0x06;
    }
    
    uint8_t get_latency(uint8_t opcode) override {
        switch (opcode) {
            case 0x01: return 4;
            case 0x02: return 5;
            case 0x03: return 1;
            case 0xFF: return 0;
            default: return 4;
        }
    }
    
    uint8_t get_throughput(uint8_t opcode) override {
        switch (opcode) {
            case 0x01: return 1;
            case 0x02: return 1;
            case 0x03: return 1;
            case 0xFF: return 1;
            default: return 1;
        }
    }
    
    std::unique_ptr<sandbox::Executor> create_executor() override {
        class SafeX86Executor : public sandbox::Executor {
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
                
                // Simulated execution: analyze code quality without running it
                std::mt19937 rng(42);
                
                size_t vector_ops = 0;
                size_t memory_ops = 0;
                size_t total_ops = code.bytes.size();
                bool has_ret = false;
                
                for (size_t i = 0; i < code.bytes.size(); ++i) {
                    uint8_t byte = code.bytes[i];
                    if (byte == 0xC3 || byte == 0xC2) has_ret = true;
                    if ((byte & 0x07) == 0x05 || (byte & 0x07) == 0x06) memory_ops++;
                    if (byte >= 0x01 && byte <= 0x03) vector_ops++;
                }
                
                FitnessScore score;
                double base_cycles = 100.0;
                double code_quality = 1.0;
                
                if (!has_ret) {
                    code_quality *= 0.5;
                }
                
                double vector_ratio = (total_ops > 0) ? static_cast<double>(vector_ops) / total_ops : 0.0;
                base_cycles *= (1.0 - vector_ratio * 0.3);
                
                double memory_ratio = (total_ops > 0) ? static_cast<double>(memory_ops) / total_ops : 0.0;
                base_cycles *= (1.0 + memory_ratio * 0.5);
                
                double size_factor = 1.0 + (code.bytes.size() / 1024.0) * 0.1;
                base_cycles *= size_factor;
                
                score.cycles = base_cycles;
                score.throughput = 1.0 / (score.cycles + 1.0);
                score.correctness = has_ret ? 1.0 : 0.5;
                score.energy_estimate = score.cycles * 0.001;
                score.composite = score.to_rank();
                
                return score;
            }
            
            bool supports(ISA a) const override { return a == ISA::X86_64; }
            std::string type_name() const override { return "x86-64 simulated sandbox"; }
        };
        
        return std::make_unique<SafeX86Executor>();
    }
};

std::unique_ptr<Backend> create_backend(ISA arch) {
    if (arch == ISA::X86_64) {
        return std::make_unique<X86Backend>();
    }
    return nullptr;
}

} // namespace isa
} // namespace hyper_organism