#pragma once

namespace dhe {

/**
 * Value is the index of the start step: [0, N-1]. Display value is the step
 * number, which is index+1.
 */
template <int N> struct SelectionStart {
  static auto constexpr min = 0;
  static auto constexpr max = N - 1;
  static auto constexpr display_offset = 1;
  static auto constexpr unit = "";
};

/**
 * The number of selected steps.
 */
template <int N> struct SelectionLength {
  static auto constexpr min = 1;
  static auto constexpr max = N;
  static auto constexpr display_offset = 0;
  static auto constexpr unit = " steps";
};

} // namespace dhe
