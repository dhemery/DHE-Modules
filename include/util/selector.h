#pragma once

#include <array>
#include <functional>
#include <utility>

namespace DHE {
template <typename T, int N> class Selector {
public:
  Selector(std::array<T, N> const choices, std::function<void(T)> on_selection)
      : choices{std::move(choices)}, notify{std::move(on_selection)} {}

  /**
   * Send notice that the choice at the given position has been selected.
   */
  void operator()(int position) { notify(choices[position]); }

private:
  const std::array<T, N> choices;
  const std::function<void(T)> notify;
};
} // namespace DHE
