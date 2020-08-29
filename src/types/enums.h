#pragma once
#include <type_traits>

namespace dhe {
template <typename E>
static constexpr auto
enum_index(E value, typename std::underlying_type<E>::type offset = 0) ->
    typename std::underlying_type<E>::type {
  return static_cast<typename std::underlying_type<E>::type>(value) + offset;
}
} // namespace dhe
