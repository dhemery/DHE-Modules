#pragma once

#include <type_traits>

namespace dhe {
template <typename...> using void_t = void; // NOLINT

template <bool B, typename T = void>
using enable_if_t = typename std::enable_if<B, T>::type; // NOLINT

} // namespace dhe