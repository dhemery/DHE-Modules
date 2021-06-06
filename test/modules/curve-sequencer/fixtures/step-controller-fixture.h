#include "modules/curve-sequencer/advance-mode.h"
#include "modules/curve-sequencer/step-controller.h"
#include "modules/curve-sequencer/step-event.h"

#include "components/phase-timer.h"

#include <dheunit/test.h>

#include <array>
#include <functional>
#include <vector>

namespace test {
namespace curve_sequencer {
using dhe::unit::Tester;
using TestFunc = std::function<void(Tester &)>;
using dhe::PhaseTimer;
using dhe::curve_sequencer::AdvanceMode;
using dhe::curve_sequencer::GenerateMode;
using dhe::sigmoid::Taper;

static auto constexpr step_count = 8;

struct Controls {
  Controls() {
    for (int i = 0; i < step_count; i++) {
      taper_[i] = &dhe::sigmoid::s_taper;
    }
  }
  std::array<AdvanceMode, step_count> advance_mode_;   // NOLINT
  std::array<float, step_count> curvature_;            // NOLINT
  std::array<float, step_count> duration_;             // NOLINT
  std::array<GenerateMode, step_count> generate_mode_; // NOLINT
  std::array<float, step_count> level_;                // NOLINT
  std::array<float, step_count> progress_;             // NOLINT
  std::array<Taper const *, step_count> taper_;        // NOLINT
  float input_;                                        // NOLINT
  float output_;                                       // NOLINT
  int deactivated_step_{-1};                           // NOLINT

  auto condition(int s) -> AdvanceMode { return advance_mode_[s]; }
  auto curvature(int s) -> float { return curvature_[s]; }
  auto duration(int s) -> float { return duration_[s]; }
  auto input() -> float { return input_; }
  auto level(int s) -> float { return level_[s]; }
  auto mode(int s) -> GenerateMode { return generate_mode_[s]; }
  void output(float v) { output_ = v; }
  auto output() -> float { return output_; }
  void show_inactive(int s) { deactivated_step_ = s; }
  void show_progress(int s, float p) { progress_[s] = p; }
  auto taper(int s) -> Taper const & { return *taper_[s]; }
};

using StepController = dhe::curve_sequencer::StepController<Controls>;
} // namespace curve_sequencer
} // namespace test
