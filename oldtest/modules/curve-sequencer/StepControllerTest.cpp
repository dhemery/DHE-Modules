#include "modules/curve-sequencer/StepController.h"

#include "components/Latch.h"
#include "components/PhaseTimer.h"
#include "components/Taper.h"
#include "doctest/doctest.h"
#include "fake/FakeControls.h"
#include "modules/curve-sequencer/AdvanceMode.h"
#include "modules/curve-sequencer/GenerateMode.h"
#include "modules/curve-sequencer/StepEvent.h"

#include <vector>

namespace test {
namespace curve_sequencer_step_controller {
  using dhe::Latch;
  using dhe::PhaseTimer;
  using dhe::curve_sequencer::AdvanceMode;
  using dhe::curve_sequencer::GenerateMode;
  using dhe::curve_sequencer::StepController;
  using dhe::curve_sequencer::StepEvent;
  using dhe::taper::VariableTaper;
  using test::curve_sequencer::FakeControls;

  static auto constexpr risenGate = Latch{true, true};
  static auto constexpr fallenGate = Latch{false, true};
  static auto constexpr highGate = Latch{true, false};
  static auto constexpr lowGate = Latch{false, false};

  std::vector<GenerateMode> const allModes{GenerateMode::Curve, GenerateMode::Hold,  GenerateMode::Sustain,
                                           GenerateMode::Input, GenerateMode::Chase, GenerateMode::Level};

  static inline void prepareToGenerate(FakeControls &controls) {
    controls.curvature = [](int s) -> float { return 0.2F; };
    controls.duration = [](int s) -> float { return 0.2F; };
    controls.input = []() -> float { return 0.2F; };
    controls.level = [](int s) -> float { return 0.2F; };
    controls.setOutput = [](float f) {};
    controls.showProgress = [](int step, float progress) {};
    controls.taper = [](int s) -> VariableTaper const * { return dhe::taper::variableTapers[0]; };
  }

  TEST_CASE("curve_sequencer::StepController") {
    FakeControls controls{};
    PhaseTimer timer{};

    StepController<FakeControls> stepController{controls, timer};

    SUBCASE("enter activates step at 0 progress") {
      controls.getOutput = []() -> float { return 5.5F; };

      auto constexpr step = 7;
      auto shownStep = int{};
      auto shownProgress = -1.F;
      controls.showProgress = [&](int step, float progress) {
        shownStep = step;
        shownProgress = progress;
      };

      stepController.enter(step);

      CHECK_EQ(shownStep, step);
      CHECK_EQ(shownProgress, 0.F);
    }

    SUBCASE("exit deactivates step") {
      controls.getOutput = []() -> float { return 5.5F; };

      auto constexpr step = 1;
      controls.showProgress = [](int step, int progress) {};
      stepController.enter(step);

      auto deactivatedStep = -1;
      controls.showInactive = [&](int step) { deactivatedStep = step; };

      stepController.exit();

      CHECK_EQ(deactivatedStep, step);
    }
  }
} // namespace curve_sequencer_step_controller
} // namespace test
