#pragma once

#include "dheunit/test.h"

#include <functional>

namespace test {
namespace stage {

using dhe::unit::Tester;
using TestFunc = std::function<void(Tester &)>;

struct Controls {
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
auto test(std::function<void(Tester &, Controls &, M &)> mode_test)
    -> TestFunc {
  return [mode_test](Tester &t) {
    Controls controls{};
    M mode{controls};
    mode_test(t, controls, mode);
  };
}

} // namespace stage
} // namespace test
