#pragma once

#include "modules/sequencizer/generator.h"

#include "components/range.h"
#include "components/sigmoid.h"
#include "signals/shape-signals.h"

#include "dheunit/test.h"

#include <array>
#include <functional>

namespace test {
namespace sequencizer {

using dhe::Shape;
using dhe::unit::Tester;
using TestFunc = std::function<void(Tester &)>;

auto constexpr step_count = 8;
struct Anchor {
  void enter(int step) { entered_[step] = true; }
  auto voltage() const -> float { return voltage_; }

  std::array<bool, step_count> entered_{}; // NOLINT
  float voltage_{};                        // NOLINT
};

struct Signals {
  auto curvature(int step) const -> float { return curvature_[step]; }
  auto duration(int step) const -> float { return duration_[step]; }
  void output(float v) { output_ = v; }
  auto shape(int step) const -> Shape::Id { return shape_[step]; }
  void show_progress(int step, float progress) { progress_[step] = progress; }
  void show_inactive(int step) { inactive_step_ = step; }

  std::array<float, step_count> curvature_{};           // NOLINT
  std::array<float, step_count> duration_{};            // NOLINT
  std::array<float, step_count> progress_{};            // NOLINT
  std::array<float, step_count> duration_multiplier_{}; // NOLINT
  int inactive_step_{};                                 // NOLINT
  float output_{};                                      // NOLINT
  std::array<Shape::Id, step_count> shape_{};           // NOLINT
};

using Generator = dhe::sequencizer::Generator<Signals, Anchor>;

template <typename Run> static inline auto test(Run const &run) -> TestFunc {
  return [run](Tester &t) {
    Signals signals{};
    for (int i = 0; i < step_count; i++) {
      signals.duration_multiplier_[i] = 1.F;
    }
    Anchor start_anchor{};
    Anchor end_anchor{};
    Generator generator{signals, start_anchor, end_anchor};
    run(t, signals, start_anchor, end_anchor, generator);
  };
}

} // namespace sequencizer
} // namespace test
