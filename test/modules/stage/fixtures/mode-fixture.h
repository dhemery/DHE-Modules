#pragma once

#include <dheunit/test.h>
#include <functional>

namespace test {
namespace stage {

using dhe::unit::Tester;
using dhe::unit::TestFunc;

struct Controls {
  void show_active(bool active) { active_ = active; }
  auto input() const -> float { return input_; }
  void output(float f) { output_ = f; }
  bool active_;          // NOLINT
  float input_{-444.F};  // NOLINT
  float output_{-555.F}; // NOLINT
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
