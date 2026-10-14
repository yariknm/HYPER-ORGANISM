#include <gtest/gtest.h>
#include "hyper_organism/core/types.hpp"
#include "hyper_organism/pool/genetic_pool.hpp"

using namespace hyper_organism;

TEST(TypeTest, OrganismCreation) {
    Organism org;
    EXPECT_EQ(org.generation, 0);
    EXPECT_FALSE(org.is_valid());
}

TEST(TypeTest, MachineCodeValidation) {
    MachineCode code;
    code.size = 16;
    code.bytes[15] = 0xC3; // RET
    EXPECT_TRUE(code.valid());
}

TEST(TypeTest, FitnessScoreRanking) {
    FitnessScore score;
    score.cycles = 100.0;
    score.throughput = 1.0;
    score.correctness = 1.0;
    EXPECT_GT(score.to_rank(), 0.0);
}

TEST(GeneticPoolTest, Initialization) {
    GeneticPool pool(50, ISA::X86_64);
    pool.initialize();
    EXPECT_EQ(pool.population().individuals.size(), 50);
    EXPECT_EQ(pool.generation(), 0);
}

TEST(GeneticPoolTest, EvolutionCycle) {
    GeneticPool pool(20, ISA::X86_64);
    pool.initialize();
    
    std::vector<double> input(16, 1.0);
    pool.evaluate_all(input.data(), input.size());
    pool.evolve();
    
    EXPECT_EQ(pool.generation(), 1);
    EXPECT_GT(pool.population().individuals.size(), 0);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}