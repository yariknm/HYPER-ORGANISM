#include "hyper_organism/io/serializer.hpp"
#include "hyper_organism/core/types.hpp"

#include <iomanip>
#include <cstring>

namespace hyper_organism {
namespace io {

void serialize_organism(std::ostream& os, const Organism& org) {
    os << "{\n";
    os << "  \"generation\": " << org.generation << ",\n";
    os << "  \"arch\": \"" << static_cast<int>(org.target_arch) << "\",\n";
    os << "  \"code_size\": " << org.code.size << ",\n";
    os << "  \"fitness\": {\n";
    os << "    \"cycles\": " << org.fitness.cycles << ",\n";
    os << "    \"throughput\": " << org.fitness.throughput << ",\n";
    os << "    \"correctness\": " << org.fitness.correctness << ",\n";
    os << "    \"composite\": " << org.fitness.composite << "\n";
    os << "  },\n";
    os << "  \"hash\": \"" << hash_organism(org) << "\"\n";
    os << "}";
}

Organism deserialize_organism(std::istream& is) {
    Organism org;
    // Simple deserialization for demonstration
    return org;
}

} // namespace io
} // namespace hyper_organism