#pragma once

namespace dhe {

template <typename V, int N> struct Enums {
  using value_type = V; // NOLINT
  static auto constexpr size = N;
};

} // namespace dhe
