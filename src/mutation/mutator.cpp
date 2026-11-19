#include "hyper_organism/mutation/mutator.hpp"
#include "hyper_organism/core/types.hpp"

#include <random>
#include <algorithm>
#include <cstring>

namespace hyper_organism {
namespace mutation {

class X86Mutator : public Mutator {
public:
    X86Mutator() : rate_(0.1), rng_(std::random_device{}()) {}
    
    Organism mutate(const Organism& parent, uint64_t seed) override {
        rng_.seed(seed);
        Organism child = parent;
        child.generation = parent.generation + 1;
        child.parent_a = 0;
        child.parent_b = 0;
        
        if (child.code.bytes.empty()) return child;
        
        if (rng_() % 100 < static_cast<int>(rate_ * 100)) {
            instruction_reorder(child);
        }
        if (rng_() % 100 < static_cast<int>(rate_ * 100)) {
            vectorize_instruction(child);
        }
        if (rng_() % 100 < static_cast<int>(rate_ * 100)) {
            insert_nop(child);
        }
        if (rng_() % 100 < static_cast<int>(rate_ * 100)) {
            change_immediate(child);
        }
        
        // Ensure RET at end
        if (!child.code.bytes.empty()) {
            child.code.bytes.back() = 0xC3;
        }
        
        return child;
    }
    
    void set_rate(double rate) override { rate_ = std::max(0.0, std::min(1.0, rate)); }
    double rate() const override { return rate_; }
    std::string type_name() const override { return "x86 graph mutator"; }

private:
    void instruction_reorder(Organism& org) {
        if (org.code.bytes.size() < 4) return;
        size_t max_pos = org.code.bytes.size() - 2;
        if (max_pos == 0) return;
        
        size_t pos1 = rng_() % max_pos;
        size_t pos2 = rng_() % max_pos;
        if (pos1 != pos2 && pos1 + 2 <= org.code.bytes.size() && pos2 + 2 <= org.code.bytes.size()) {
            std::swap(org.code.bytes[pos1], org.code.bytes[pos2]);
            std::swap(org.code.bytes[pos1 + 1], org.code.bytes[pos2 + 1]);
        }
    }
    
    void vectorize_instruction(Organism& org) {
        if (org.code.bytes.empty()) return;
        size_t pos = rng_() % org.code.bytes.size();
        switch (org.code.bytes[pos]) {
            case 0x01: org.code.bytes[pos] = 0x01; break;
            case 0x03: org.code.bytes[pos] = 0x03; break;
            default: break;
        }
    }
    
    void insert_nop(Organism& org) {
        if (org.code.bytes.size() >= MAX_CODE_SIZE) return;
        if (org.code.bytes.empty()) return;
        
        size_t pos = rng_() % org.code.bytes.size();
        org.code.bytes.insert(org.code.bytes.begin() + pos, 0x90);
    }
    
    void change_immediate(Organism& org) {
        if (org.code.bytes.size() < 3) return;
        size_t max_pos = org.code.bytes.size() - 2;
        if (max_pos == 0) return;
        size_t pos = rng_() % max_pos;
        org.code.bytes[pos + 1] = static_cast<uint8_t>(rng_() % 256);
        org.code.bytes[pos + 2] = static_cast<uint8_t>(rng_() % 256);
    }
    
    double rate_;
    std::mt19937 rng_;
};

std::unique_ptr<Mutator> create_mutator(ISA arch) {
    if (arch == ISA::X86_64) {
        return std::make_unique<X86Mutator>();
    }
    return nullptr;
}

} // namespace mutation
} // namespace hyper_organism