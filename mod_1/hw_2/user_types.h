#pragma once

#include <array>
#include <cstdint>
#include <vector>

namespace user_types {
using ip_t = std::array<uint8_t, 4>;
using ip_pool_t = std::vector<ip_t>;
} // namespace user_types