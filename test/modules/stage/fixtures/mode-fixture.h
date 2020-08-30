#pragma once

#include "components/phase-timer.h"
#include "modules/stage/event.h"
#include "types/enums.h"

#include <dheunit/test.h>
#include <functional>

namespace test {
namespace stage {

using dhe::stage::Event;
using dhe::unit::Tester;
using dhe::unit::TestFunc;

struct Controls {
  auto duration() const -> float { return duration_; }
  auto level() const -> float { return level_; }
  auto input() const -> float { return input_; }
  void output(float f) { output_ = f; }
  void show_active(bool active) { active_ = active; }
  bool active_{};    // NOLINT
  float duration_{}; // NOLINT
  float input_{};    // NOLINT
  float level_{};    // NOLINT
  float output_{};   // NOLINT
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

template <typename M, typename T>
auto test(std::function<void(Tester &, Controls &, T &, M &)> mode_test)
    -> TestFunc {
  return [mode_test](Tester &t) {
    Controls controls{};
    T timer{};
    M mode{controls, timer};
    mode_test(t, controls, timer, mode);
  };
}

} // namespace stage
} // namespace test

namespace dhe {
namespace stage {
static inline auto operator<<(std::ostream &os, Event event) -> std::ostream & {
  os << dhe::enum_index(event);
  return os;
}
} // namespace stage
} // namespace dhe
