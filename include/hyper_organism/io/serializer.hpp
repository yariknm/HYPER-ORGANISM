#ifndef HYPER_ORGANISM_IO_SERIALIZER_HPP
#define HYPER_ORGANISM_IO_SERIALIZER_HPP

#include "core/types.hpp"
#include <ostream>

namespace hyper_organism {
namespace io {

void serialize_organism(std::ostream& os, const Organism& org);
Organism deserialize_organism(std::istream& is);

} // namespace io
} // namespace hyper_organism

#endif // HYPER_ORGANISM_IO_SERIALIZER_HPP