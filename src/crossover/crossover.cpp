#include "hyper_organism/crossover/crossover.hpp"
#include "hyper_organism/core/types.hpp"

#include <random>
#include <cstring>
#include <algorithm>

namespace hyper_organism {
namespace crossover {

class BlockCrossover : public Crossover {
public:
    BlockCrossover() : rate_(0.7), rng_(std::random_device{}()) {}
    
    CrossoverResult cross(const Organism& a, const Organism& b, uint64_t seed) override {
        rng_.seed(seed);
        CrossoverResult result;
        result.success = false;
        
        if (a.code.bytes.empty() || b.code.bytes.empty()) {
            return result;
        }
        
        size_t min_size = std::min(a.code.bytes.size(), b.code.bytes.size());
        if (min_size < 2) return result;
        
        size_t split_point = rng_() % (min_size - 1) + 1;
        
        result.child_a = a;
        result.child_b = b;
        
        // Child A: first part from A, second part from B
        std::vector<uint8_t> child_a_bytes;
        child_a_bytes.reserve(MAX_CODE_SIZE);
        child_a_bytes.insert(child_a_bytes.end(), a.code.bytes.begin(), 
                            a.code.bytes.begin() + split_point);
        child_a_bytes.insert(child_a_bytes.end(), b.code.bytes.begin() + split_point,
                            b.code.bytes.end());
        result.child_a.code.bytes = std::move(child_a_bytes);
        
        // Child B: first part from B, second part from A
        std::vector<uint8_t> child_b_bytes;
        child_b_bytes.reserve(MAX_CODE_SIZE);
        child_b_bytes.insert(child_b_bytes.end(), b.code.bytes.begin(),
                            b.code.bytes.begin() + split_point);
        child_b_bytes.insert(child_b_bytes.end(), a.code.bytes.begin() + split_point,
                            a.code.bytes.end());
        result.child_b.code.bytes = std::move(child_b_bytes);
        
        // Ensure RET at end
        if (!result.child_a.code.bytes.empty()) {
            result.child_a.code.bytes.back() = 0xC3;
        }
        if (!result.child_b.code.bytes.empty()) {
            result.child_b.code.bytes.back() = 0xC3;
        }
        
        result.child_a.generation = std::max(a.generation, b.generation) + 1;
        result.child_b.generation = std::max(a.generation, b.generation) + 1;
        
        result.success = true;
        return result;
    }
    
    void set_rate(double rate) override { rate_ = rate; }
    double rate() const override { return rate_; }
    std::string type_name() const override { return "block crossover"; }

private:
    double rate_;
    std::mt19937 rng_;
};

std::unique_ptr<Crossover> create_crossover(ISA arch) {
    if (arch == ISA::X86_64) {
        return std::make_unique<BlockCrossover>();
    }
    return nullptr;
}

} // namespace crossover
} // namespace hyper_organism