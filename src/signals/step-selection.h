#pragma once

namespace dhe {

template <int N> struct SelectionStart {
  static auto constexpr min = 1;
  static auto constexpr max = N;
  static auto constexpr unit = "";
};

template <int N> struct SelectionLength {
  static auto constexpr min = 1;
  static auto constexpr max = N;
  static auto constexpr unit = " steps";
};

} // namespace dhe
