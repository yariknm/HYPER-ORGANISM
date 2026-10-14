#include "hyper_organism/core/types.hpp"

#include <cstring>
#include <sstream>
#include <iomanip>

namespace hyper_organism {

std::string hash_organism(const Organism& org) {
    std::stringstream ss;
    ss << std::hex << std::setfill('0');
    for (size_t i = 0; i < org.code.size && i < 64; ++i) {
        ss << std::setw(2) << static_cast<int>(org.code.bytes[i]);
    }
    ss << "_" << org.generation << "_" << static_cast<int>(org.target_arch);
    return ss.str();
}

bool validate_machine_code(const MachineCode& code) {
    if (!code.valid()) return false;
    if (code.size > MAX_CODE_SIZE) return false;
    bool has_ret = false;
    for (size_t i = 0; i < code.size; ++i) {
        if (code.bytes[i] == 0xC3 || code.bytes[i] == 0xC2) {
            has_ret = true;
            break;
        }
    }
    return has_ret;
}

} // namespace hyper_organism