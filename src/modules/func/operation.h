#pragma once

#include <array>

namespace dhe {
namespace func {
enum class Operation { Add, Multiply };

namespace operations {
static auto constexpr labels =
    std::array<char const *, 2>{"Add (offset)", "Multiply (scale)"};
} // namespace operations

} // namespace func
} // namespace dhe
