#pragma once

#include <utility>

#include "util/range.h"

namespace DHE {
template <typename T> class Selector {
public:
  Selector(std::vector<T> choices, std::function<void(T)> on_selection)
      : choices{std::move(choices)}, notify{std::move(on_selection)} {}

  /**
   * Select the choice indicated by the input and send notification of the
   * selection.
   */
  void operator()(float input) { notify(selection(input)); }

private:
  auto selection(float input) -> T { return choices[static_cast<int>(input)]; }
  const std::vector<T> choices;
  const std::function<void(T)> notify;
};
} // namespace DHE
