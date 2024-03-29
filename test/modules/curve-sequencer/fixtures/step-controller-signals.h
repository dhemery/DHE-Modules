#pragma once

#include "modules/curve-sequencer/advance-mode.h"
#include "modules/curve-sequencer/step-controller.h"
#include "modules/curve-sequencer/step-event.h"

#include "components/phase-timer.h"
#include "components/sigmoid.h"

#include <array>

namespace test {
namespace curve_sequencer {
using dhe::PhaseTimer;
using dhe::Shape;
using dhe::curve_sequencer::AdvanceMode;
using dhe::curve_sequencer::GenerateMode;

template <int N> struct StepControllerSignals {
  std::array<AdvanceMode, N> advance_mode_;   // NOLINT
  std::array<float, N> curvature_;            // NOLINT
  std::array<float, N> duration_;             // NOLINT
  std::array<GenerateMode, N> generate_mode_; // NOLINT
  std::array<float, N> level_;                // NOLINT
  std::array<float, N> progress_;             // NOLINT
  std::array<Shape::Id, N> shape_;            // NOLINT
  float input_;                               // NOLINT
  float output_;                              // NOLINT
  int deactivated_step_{-1};                  // NOLINT

  auto advance_mode(int s) -> AdvanceMode { return advance_mode_[s]; }
  auto curvature(int s) -> float { return curvature_[s]; }
  auto duration(int s) -> float { return duration_[s]; }
  auto generate_mode(int s) -> GenerateMode { return generate_mode_[s]; }
  auto input() -> float { return input_; }
  auto level(int s) -> float { return level_[s]; }
  void output(float v) { output_ = v; }
  auto output() -> float { return output_; }
  auto shape(int s) -> Shape::Id const & { return shape_[s]; }
  void show_inactive(int s) { deactivated_step_ = s; }
  void show_progress(int s, float p) { progress_[s] = p; }
};
} // namespace curve_sequencer
} // namespace test
