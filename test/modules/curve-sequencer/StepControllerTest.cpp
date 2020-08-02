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
    controls.input = []() -> float { return 0.2F; };
    controls.curvature = [](int s) -> float { return 0.2F; };
    controls.duration = [](int s) -> float { return 0.2F; };
    controls.level = [](int s) -> float { return 0.2F; };
    controls.setOutput = [](float f) {};
    controls.taper = [](int s) -> VariableTaper const * { return dhe::taper::variableTapers[0]; };
  }

  // TODO: advance on time returns generated if timer does not expire
  // TODO: advance on time returns completed if timer expires
  // TODO: test generate

  TEST_SUITE("curve_sequencer::StepController") {
    FakeControls controls{};
    PhaseTimer timer{};

    StepController<FakeControls> stepController{controls, timer};

    TEST_CASE("enter activates step at 0 progress") {
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

    TEST_CASE("exit deactivates step") {
      controls.getOutput = []() -> float { return 5.5F; };

      auto constexpr step = 1;
      controls.showProgress = [](int step, int progress) {};
      stepController.enter(step);

      auto deactivatedStep = -1;
      controls.showInactive = [&](int step) { deactivatedStep = step; };

      stepController.exit();

      CHECK_EQ(deactivatedStep, step);
    }

    TEST_CASE("advance on high") {
      auto const step = 2;
      controls.condition = [](int step) -> AdvanceMode { return AdvanceMode::GateIsHigh; };
      stepController.enter(step);

      SUBCASE("completes if gate is high") {
        std::for_each(allModes.cbegin(), allModes.cend(), [](GenerateMode const mode) {
          controls.mode = [mode](int step) -> GenerateMode { return mode; };
          CHECK_EQ(stepController.execute(risenGate, 0.F), StepEvent::Completed);
          CHECK_EQ(stepController.execute(highGate, 0.F), StepEvent::Completed);
        });
      }

      SUBCASE("generates if gate is low") {
        prepareToGenerate(controls);
        std::for_each(allModes.cbegin(), allModes.cend(), [](GenerateMode const mode) {
          controls.mode = [mode](int s) -> GenerateMode { return mode; };
          CHECK_EQ(stepController.execute(lowGate, 0.F), StepEvent::Generated);
          CHECK_EQ(stepController.execute(fallenGate, 0.F), StepEvent::Generated);
        });
      }
    }

    TEST_CASE("advance on low") {
      auto const step = 2;
      controls.condition = [](int step) -> AdvanceMode { return AdvanceMode::GateIsLow; };
      stepController.enter(step);

      SUBCASE("completes if gate is high") {
        std::for_each(allModes.cbegin(), allModes.cend(), [](GenerateMode const mode) {
          controls.mode = [mode](int step) -> GenerateMode { return mode; };
          CHECK_EQ(stepController.execute(lowGate, 0.F), StepEvent::Completed);
          CHECK_EQ(stepController.execute(fallenGate, 0.F), StepEvent::Completed);
        });
      }

      SUBCASE("generates if gate is low") {
        prepareToGenerate(controls);
        std::for_each(allModes.cbegin(), allModes.cend(), [](GenerateMode const mode) {
          controls.mode = [mode](int s) -> GenerateMode { return mode; };
          CHECK_EQ(stepController.execute(highGate, 0.F), StepEvent::Generated);
          CHECK_EQ(stepController.execute(risenGate, 0.F), StepEvent::Generated);
        });
      }
    }

    TEST_CASE("advance on rise") {
      auto const step = 2;
      controls.condition = [](int step) -> AdvanceMode { return AdvanceMode::GateRises; };
      stepController.enter(step);

      SUBCASE("completes if gate rises") {
        std::for_each(allModes.cbegin(), allModes.cend(), [](GenerateMode const mode) {
          controls.mode = [mode](int step) -> GenerateMode { return mode; };
          CHECK_EQ(stepController.execute(risenGate, 0.F), StepEvent::Completed);
        });
      }

      SUBCASE("generates if gate does not rise") {
        prepareToGenerate(controls);
        std::for_each(allModes.cbegin(), allModes.cend(), [](GenerateMode const mode) {
          controls.mode = [mode](int s) -> GenerateMode { return mode; };
          CHECK_EQ(stepController.execute(highGate, 0.F), StepEvent::Generated);
          CHECK_EQ(stepController.execute(lowGate, 0.F), StepEvent::Generated);
          CHECK_EQ(stepController.execute(fallenGate, 0.F), StepEvent::Generated);
        });
      }
    }

    TEST_CASE("advance on fall") {
      auto const step = 2;
      controls.condition = [](int step) -> AdvanceMode { return AdvanceMode::GateFalls; };
      stepController.enter(step);

      SUBCASE("completes if gate falls") {
        std::for_each(allModes.cbegin(), allModes.cend(), [](GenerateMode const mode) {
          controls.mode = [mode](int step) -> GenerateMode { return mode; };
          CHECK_EQ(stepController.execute(fallenGate, 0.F), StepEvent::Completed);
        });
      }

      SUBCASE("generates if gate does not fall") {
        prepareToGenerate(controls);
        std::for_each(allModes.cbegin(), allModes.cend(), [](GenerateMode const mode) {
          controls.mode = [mode](int s) -> GenerateMode { return mode; };
          CHECK_EQ(stepController.execute(highGate, 0.F), StepEvent::Generated);
          CHECK_EQ(stepController.execute(lowGate, 0.F), StepEvent::Generated);
          CHECK_EQ(stepController.execute(risenGate, 0.F), StepEvent::Generated);
        });
      }
    }

    TEST_CASE("advance on edge") {
      auto const step = 2;
      controls.condition = [](int step) -> AdvanceMode { return AdvanceMode::GateChanges; };
      stepController.enter(step);

      SUBCASE("completes if gate changes") {
        std::for_each(allModes.cbegin(), allModes.cend(), [](GenerateMode const mode) {
          controls.mode = [mode](int step) -> GenerateMode { return mode; };
          CHECK_EQ(stepController.execute(risenGate, 0.F), StepEvent::Completed);
          CHECK_EQ(stepController.execute(fallenGate, 0.F), StepEvent::Completed);
        });
      }

      SUBCASE("generates if gate does not change") {
        prepareToGenerate(controls);
        std::for_each(allModes.cbegin(), allModes.cend(), [](GenerateMode const mode) {
          controls.mode = [mode](int s) -> GenerateMode { return mode; };
          CHECK_EQ(stepController.execute(highGate, 0.F), StepEvent::Generated);
          CHECK_EQ(stepController.execute(lowGate, 0.F), StepEvent::Generated);
        });
      }
    }
  }
} // namespace curve_sequencer_step_controller
} // namespace test
