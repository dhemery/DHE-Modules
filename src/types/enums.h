#pragma once
#include <type_traits>

namespace dhe {
template <typename E>
static constexpr auto enum_index(E e) ->
    typename std::underlying_type<E>::type {
  return static_cast<typename std::underlying_type<E>::type>(e);
}
} // namespace dhe
