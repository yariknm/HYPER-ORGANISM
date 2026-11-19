#ifndef HYPER_ORGANISM_IO_SERIALIZER_HPP
#define HYPER_ORGANISM_IO_SERIALIZER_HPP
#include "hyper_organism/core/types.hpp"
#include <ostream>
namespace hyper_organism { namespace io {
void serialize_organism(std::ostream&, const Organism&);
Organism deserialize_organism(std::istream&);
} }
#endif
