#pragma once

#include "dheunit/test.h"

#include <functional>

namespace test {
namespace envelope {

using dhe::unit::Tester;
using TestFunc = std::function<void(Tester &)>;

struct Signals {
  auto level() const -> float { return level_; }
  auto input() const -> float { return input_; }
  void output(float f) { output_ = f; }
  void show_active(bool active) { active_ = active; }
  bool active_{};  // NOLINT
  float input_{};  // NOLINT
  float level_{};  // NOLINT
  float output_{}; // NOLINT
};

template <typename M>
auto test(std::function<void(Tester &, Signals &, M &)> mode_test) -> TestFunc {
  return [mode_test](Tester &t) {
    Signals signals{};
    M mode{signals};
    mode_test(t, signals, mode);
  };
}

} // namespace envelope
} // namespace test
