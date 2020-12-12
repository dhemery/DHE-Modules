#pragma once

#include "components/phase-timer.h"
#include "components/sigmoid.h"
#include "modules/stage/event.h"

#include <dheunit/test.h>
#include <functional>

namespace test {
namespace stage {

using dhe::PhaseTimer;
using dhe::sigmoid::Taper;
using dhe::stage::Event;
using dhe::unit::Tester;
using TestFunc = std::function<void(Tester &)>;

struct Controls {
  auto curvature() const -> float { return curvature_; }
  auto duration() const -> float { return duration_; }
  auto level() const -> float { return level_; }
  auto input() const -> float { return input_; }
  void output(float f) { output_ = f; }
  void show_active(bool active) { active_ = active; }
  auto taper() const -> Taper { return taper_; }
  bool active_{};                       // NOLINT
  float duration_{};                    // NOLINT
  float curvature_{};                   // NOLINT
  float input_{};                       // NOLINT
  float level_{};                       // NOLINT
  float output_{};                      // NOLINT
  Taper taper_ = dhe::sigmoid::j_taper; // NOLINT
};

template <typename M>
auto test(
    std::function<void(Tester &, Controls &, PhaseTimer &, M &)> mode_test)
    -> TestFunc {
  return [mode_test](Tester &t) {
    Controls controls{};
    PhaseTimer timer{};
    M mode{controls, timer};
    mode_test(t, controls, timer, mode);
  };
}

} // namespace stage
} // namespace test

namespace dhe {
namespace stage {
static inline auto operator<<(std::ostream &os, Event event) -> std::ostream & {
  os << static_cast<int>(event);
  return os;
}
} // namespace stage
} // namespace dhe
