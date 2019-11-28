#pragma once

#include "Event.h"
#include "Mode.h"
#include "components/Latch.h"
#include "components/PhaseTimer.h"
#include "components/Range.h"

#include <algorithm>

namespace dhe {
namespace stage {

  template <typename Controls, typename DeferMode, typename InputMode, typename GenerateMode, typename LevelMode>
  class StageEngine {
  public:
    StageEngine(Controls &controls, DeferMode &deferMode, InputMode &inputMode, GenerateMode &generateMode,
                LevelMode &levelMode) :
        controls{controls},
        deferMode{deferMode},
        inputMode{inputMode},
        generateMode{generateMode},
        levelMode{levelMode} {}

    void process(float sampleTime) {
      defer.clock(controls.defer());
      trigger.clock(controls.trigger());

      auto const newState = identifyState();
      if (mode != newState) {
        enter(newState);
      }

      switch (mode) {
      case Mode::Generate:
        generate(sampleTime);
        break;
      case Mode::Level:
        levelMode.execute();
        break;
      case Mode::Defer:
        deferMode.execute();
        break;
      case Mode::Input:
        inputMode.execute();
        break;
      default:
        break;
      }

      eocTimer.advance(sampleTime / 1e-3F);
      controls.showEoc(eocTimer.inProgress());
    }

  private:
    // TODO: Check for TRIG rise before checking for DEFER fall, so that we start generating as soon as DEFER falls.
    auto identifyState() -> Mode {
      if (defer.isHigh()) {
        return Mode::Defer;
      }
      if (defer.isFall()) {
        return Mode::Input;
      }
      if (trigger.isRise()) {
        return Mode::Generate;
      }
      return mode;
    }

    void enter(Mode newState) {
      switch (mode) {
      case Mode::Generate:
        generateMode.exit();
        break;
      case Mode::Defer:
        deferMode.exit();
        trigger.clock(false);
        break;
      case Mode::Input:
        inputMode.exit();
        break;
      case Mode::Level:
        levelMode.exit();
        break;
      default:
        break;
      }

      mode = newState;

      switch (mode) {
      case Mode::Defer:
        deferMode.enter();
        break;
      case Mode::Generate:
        generateMode.enter();
        break;
      case Mode::Input:
        inputMode.enter();
        break;
      case Mode::Level:
        levelMode.enter();
        break;
      default:
        break;
      }
    }

    void generate(float sampleTime) {
      auto const event = generateMode.execute(trigger, sampleTime);
      if (event == Event::Completed) {
        eocTimer.reset();
        enter(Mode::Level);
      }
    }

    PhaseTimer eocTimer{1.F};
    Mode mode{Mode::Input};
    Latch defer{};
    Latch trigger{};
    Controls &controls;
    DeferMode &deferMode;
    InputMode &inputMode;
    GenerateMode &generateMode;
    LevelMode &levelMode;
  };
} // namespace stage
} // namespace dhe
