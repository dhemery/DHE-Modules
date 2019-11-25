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
      if (state != newState) {
        enter(newState);
      }

      switch (state) {
      case Mode::Generate:
        generate(sampleTime);
        break;
      case Mode::TrackLevel:
        levelMode.execute();
        break;
      case Mode::Defer:
        deferMode.execute();
        break;
      case Mode::TrackInput:
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
        return Mode::TrackInput;
      }
      if (trigger.isRise()) {
        return Mode::Generate;
      }
      return state;
    }

    void enter(Mode newState) {
      switch (state) {
      case Mode::Generate:
        generateMode.exit();
        break;
      case Mode::Defer:
        deferMode.exit();
        trigger.clock(false);
        break;
      case Mode::TrackInput:
        inputMode.exit();
        break;
      case Mode::TrackLevel:
        levelMode.exit();
        break;
      default:
        break;
      }

      state = newState;

      switch (state) {
      case Mode::Defer:
        deferMode.enter();
        break;
      case Mode::Generate:
        generateMode.enter();
        break;
      case Mode::TrackInput:
        inputMode.enter();
        break;
      case Mode::TrackLevel:
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
        enter(Mode::TrackLevel);
      }
    }

    PhaseTimer eocTimer{1.F};
    Mode state{Mode::TrackInput};
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
