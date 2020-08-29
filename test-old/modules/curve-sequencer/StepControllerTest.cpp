#include "modules/old-curve-sequencer/step-controller.h"

#include "components/latch.h"
#include "components/phase-timer.h"
#include "components/sigmoid.h"
#include "fake/Controls.h"
#include "modules/old-curve-sequencer/advance-mode.h"
#include "modules/old-curve-sequencer/generate-mode.h"
#include "modules/old-curve-sequencer/step-event.h"

#include <algorithm>
#include <doctest.h>
#include <vector>

namespace test {
namespace curve_sequencer {
namespace step_controller {
using dhe::Latch;
using dhe::PhaseTimer;
using dhe::old_curve_sequencer::AdvanceMode;
using dhe::old_curve_sequencer::GenerateMode;
using dhe::old_curve_sequencer::StepController;
using dhe::old_curve_sequencer::StepEvent;
using dhe::sigmoid::Taper;

static auto constexpr risenGate = Latch{true, true};
static auto constexpr fallenGate = Latch{false, true};
static auto constexpr highGate = Latch{true, false};
static auto constexpr lowGate = Latch{false, false};

std::vector<GenerateMode> const allModes{
    GenerateMode::Curve, GenerateMode::Hold,  GenerateMode::Sustain,
    GenerateMode::Input, GenerateMode::Chase, GenerateMode::Level};

static inline void prepareToGenerate(fake::Controls &controls) {
  controls.curvature = [](int s) -> float { return 0.2F; };
  controls.duration = [](int s) -> float { return 0.2F; };
  controls.input = []() -> float { return 0.2F; };
  controls.level = [](int s) -> float { return 0.2F; };
  controls.set_output = [](float f) {};
  controls.show_progress = [](int step, float progress) {};
  controls.taper = [](int /*s*/) -> Taper const & {
    return dhe::sigmoid::tapers[0];
  };
}

// TODO: advance on time returns generated if timer does not expire
// TODO: advance on time returns completed if timer expires
// TODO: test generate

TEST_CASE("curve_sequencer::StepController") {
  fake::Controls controls{};
  PhaseTimer timer{};

  StepController<fake::Controls> stepController{controls, timer};

  SUBCASE("enter activates step at 0 progress") {
    controls.get_output = []() -> float { return 5.5F; };

    auto constexpr step = 7;
    auto shownStep = int{};
    auto shownProgress = -1.F;
    controls.show_progress = [&](int step, float progress) {
      shownStep = step;
      shownProgress = progress;
    };

    stepController.enter(step);

    CHECK_EQ(shownStep, step);
    CHECK_EQ(shownProgress, 0.F);
  }

  SUBCASE("exit deactivates step") {
    controls.get_output = []() -> float { return 5.5F; };

    auto constexpr step = 1;
    controls.show_progress = [](int step, int progress) {};
    stepController.enter(step);

    auto deactivatedStep = -1;
    controls.show_inactive = [&](int step) { deactivatedStep = step; };

    stepController.exit();

    CHECK_EQ(deactivatedStep, step);
  }

  SUBCASE("advance mode") {
    auto const step = 2;
    controls.get_output = []() -> float { return 0.F; };
    controls.show_progress = [](int step, float progress) {};
    stepController.enter(step);

    SUBCASE("GateIsHigh") {
      controls.condition = [](int step) -> AdvanceMode {
        return AdvanceMode::GateIsHigh;
      };

      SUBCASE("completes if gate is high") {
        controls.show_inactive = [](int step) {};
        std::for_each(allModes.cbegin(), allModes.cend(),
                      [&controls, &stepController](GenerateMode const mode) {
                        controls.mode = [mode](int step) -> GenerateMode {
                          return mode;
                        };
                        CHECK_EQ(stepController.execute(risenGate, 0.F),
                                 StepEvent::Completed);
                        CHECK_EQ(stepController.execute(highGate, 0.F),
                                 StepEvent::Completed);
                      });
      }

      SUBCASE("generates if gate is low") {
        prepareToGenerate(controls);
        std::for_each(allModes.cbegin(), allModes.cend(),
                      [&controls, &stepController](GenerateMode const mode) {
                        controls.mode = [mode](int s) -> GenerateMode {
                          return mode;
                        };
                        CHECK_EQ(stepController.execute(lowGate, 0.F),
                                 StepEvent::Generated);
                        CHECK_EQ(stepController.execute(fallenGate, 0.F),
                                 StepEvent::Generated);
                      });
      }
    }

    SUBCASE("GateIsLow") {
      controls.condition = [](int step) -> AdvanceMode {
        return AdvanceMode::GateIsLow;
      };

      SUBCASE("completes if gate is high") {
        controls.show_inactive = [](int step) {};
        std::for_each(allModes.cbegin(), allModes.cend(),
                      [&controls, &stepController](GenerateMode const mode) {
                        controls.mode = [mode](int step) -> GenerateMode {
                          return mode;
                        };
                        CHECK_EQ(stepController.execute(lowGate, 0.F),
                                 StepEvent::Completed);
                        CHECK_EQ(stepController.execute(fallenGate, 0.F),
                                 StepEvent::Completed);
                      });
      }

      SUBCASE("generates if gate is low") {
        prepareToGenerate(controls);
        std::for_each(allModes.cbegin(), allModes.cend(),
                      [&controls, &stepController](GenerateMode const mode) {
                        controls.mode = [mode](int s) -> GenerateMode {
                          return mode;
                        };
                        CHECK_EQ(stepController.execute(highGate, 0.F),
                                 StepEvent::Generated);
                        CHECK_EQ(stepController.execute(risenGate, 0.F),
                                 StepEvent::Generated);
                      });
      }
    }

    SUBCASE("GateRises") {
      controls.condition = [](int step) -> AdvanceMode {
        return AdvanceMode::GateRises;
      };

      SUBCASE("completes if gate rises") {
        controls.show_inactive = [](int step) {};
        std::for_each(allModes.cbegin(), allModes.cend(),
                      [&controls, &stepController](GenerateMode const mode) {
                        controls.mode = [mode](int step) -> GenerateMode {
                          return mode;
                        };
                        CHECK_EQ(stepController.execute(risenGate, 0.F),
                                 StepEvent::Completed);
                      });
      }

      SUBCASE("generates if gate does not rise") {
        prepareToGenerate(controls);
        std::for_each(allModes.cbegin(), allModes.cend(),
                      [&controls, &stepController](GenerateMode const mode) {
                        controls.mode = [mode](int s) -> GenerateMode {
                          return mode;
                        };
                        CHECK_EQ(stepController.execute(highGate, 0.F),
                                 StepEvent::Generated);
                        CHECK_EQ(stepController.execute(lowGate, 0.F),
                                 StepEvent::Generated);
                        CHECK_EQ(stepController.execute(fallenGate, 0.F),
                                 StepEvent::Generated);
                      });
      }
    }

    SUBCASE("GateFalls") {
      controls.condition = [](int step) -> AdvanceMode {
        return AdvanceMode::GateFalls;
      };

      SUBCASE("completes if gate falls") {
        controls.show_inactive = [](int step) {};
        std::for_each(allModes.cbegin(), allModes.cend(),
                      [&controls, &stepController](GenerateMode const mode) {
                        controls.mode = [mode](int step) -> GenerateMode {
                          return mode;
                        };
                        CHECK_EQ(stepController.execute(fallenGate, 0.F),
                                 StepEvent::Completed);
                      });
      }

      SUBCASE("generates if gate does not fall") {
        prepareToGenerate(controls);
        std::for_each(allModes.cbegin(), allModes.cend(),
                      [&controls, &stepController](GenerateMode const mode) {
                        controls.mode = [mode](int s) -> GenerateMode {
                          return mode;
                        };
                        CHECK_EQ(stepController.execute(highGate, 0.F),
                                 StepEvent::Generated);
                        CHECK_EQ(stepController.execute(lowGate, 0.F),
                                 StepEvent::Generated);
                        CHECK_EQ(stepController.execute(risenGate, 0.F),
                                 StepEvent::Generated);
                      });
      }
    }

    SUBCASE("GateChanges") {
      controls.condition = [](int step) -> AdvanceMode {
        return AdvanceMode::GateChanges;
      };

      SUBCASE("completes if gate changes") {
        controls.show_inactive = [](int step) {};
        std::for_each(allModes.cbegin(), allModes.cend(),
                      [&controls, &stepController](GenerateMode const mode) {
                        controls.mode = [mode](int step) -> GenerateMode {
                          return mode;
                        };
                        CHECK_EQ(stepController.execute(risenGate, 0.F),
                                 StepEvent::Completed);
                        CHECK_EQ(stepController.execute(fallenGate, 0.F),
                                 StepEvent::Completed);
                      });
      }

      SUBCASE("generates if gate does not change") {
        prepareToGenerate(controls);
        std::for_each(allModes.cbegin(), allModes.cend(),
                      [&controls, &stepController](GenerateMode const mode) {
                        controls.mode = [mode](int s) -> GenerateMode {
                          return mode;
                        };
                        CHECK_EQ(stepController.execute(highGate, 0.F),
                                 StepEvent::Generated);
                        CHECK_EQ(stepController.execute(lowGate, 0.F),
                                 StepEvent::Generated);
                      });
      }
    }
  }
}
} // namespace step_controller
} // namespace curve_sequencer
} // namespace test
