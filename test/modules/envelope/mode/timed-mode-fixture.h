#pragma once

#include "components/phase-timer.h"
#include "components/sigmoid.h"
#include "modules/envelope/mode/event.h"

#include "dheunit/test.h"

#include <functional>

namespace test {
namespace envelope {

using dhe::PhaseTimer;
using dhe::envelope::mode::Event;
using dhe::sigmoid::Taper;
using dhe::unit::Tester;
using TestFunc = std::function<void(Tester &)>;

struct Signals {
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
auto test(std::function<void(Tester &, Signals &, PhaseTimer &, M &)> mode_test)
    -> TestFunc {
  return [mode_test](Tester &t) {
    Signals signals{};
    PhaseTimer timer{};
    M mode{signals, timer};
    mode_test(t, signals, timer, mode);
  };
}

} // namespace envelope
} // namespace test

namespace dhe {
namespace envelope {
namespace mode {
static inline auto operator<<(std::ostream &os, Event event) -> std::ostream & {
  os << static_cast<int>(event);
  return os;
}
} // namespace mode
} // namespace envelope
} // namespace dhe
