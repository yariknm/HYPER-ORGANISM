#include "hyper_organism/crossover/crossover.hpp"
#include "hyper_organism/core/types.hpp"

#include <random>
#include <cstring>

namespace hyper_organism {
namespace crossover {

class BlockCrossover : public Crossover {
public:
    BlockCrossover() : rate_(0.7), rng_(std::random_device{}()) {}
    
    CrossoverResult cross(const Organism& a, const Organism& b, uint64_t seed) override {
        rng_.seed(seed);
        CrossoverResult result;
        result.success = false;
        
        if (a.code.size == 0 || b.code.size == 0) {
            return result;
        }
        
        // Uniform crossover at instruction block level
        size_t min_size = std::min(a.code.size, b.code.size);
        size_t split_point = rng_() % min_size;
        
        result.child_a = a;
        result.child_b = b;
        
        // Swap code segments
        std::vector<uint8_t> temp_a(a.code.bytes.begin(), a.code.bytes.begin() + a.code.size);
        std::vector<uint8_t> temp_b(b.code.bytes.begin(), b.code.bytes.begin() + b.code.size);
        
        // Crossover: child_a gets first half from a, second half from b
        if (split_point < temp_b.size()) {
            std::copy(temp_b.begin() + split_point, temp_b.end(), 
                     temp_a.begin() + split_point);
            result.child_a.code.size = std::max(split_point, temp_b.size() - split_point);
        }
        
        if (split_point < temp_a.size()) {
            std::copy(temp_a.begin() + split_point, temp_a.end(),
                     temp_b.begin() + split_point);
            result.child_b.code.size = std::max(split_point, temp_a.size() - split_point);
        }
        
        // Ensure RET at end
        if (result.child_a.code.size > 0) {
            result.child_a.code.bytes[result.child_a.code.size - 1] = 0xC3;
        }
        if (result.child_b.code.size > 0) {
            result.child_b.code.bytes[result.child_b.code.size - 1] = 0xC3;
        }
        
        result.child_a.generation = std::max(a.generation, b.generation) + 1;
        result.child_b.generation = std::max(a.generation, b.generation) + 1;
        result.child_a.parent_a = 0;
        result.child_a.parent_b = 0;
        result.child_b.parent_a = 0;
        result.child_b.parent_b = 0;
        
        result.success = true;
        return result;
    }
    
    void set_rate(double rate) override { rate_ = std::max(0.0, std::min(1.0, rate)); }
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