#pragma once

#include "Event.h"
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
      case Generating:
        generate(sampleTime);
        break;
      case TrackingLevel:
        levelMode.execute();
        break;
      case Deferring:
        deferMode.execute();
        break;
      case TrackingInput:
        inputMode.execute();
        break;
      }

      eocTimer.advance(sampleTime / 1e-3F);
      controls.showEoc(eocTimer.inProgress());
    }

  private:
    enum State {
      Deferring,
      Generating,
      TrackingInput,
      TrackingLevel,
    };

    // TODO: Check for TRIG rise before checking for DEFER fall, so that we start generating as soon as DEFER falls.
    auto identifyState() -> StageEngine::State {
      if (defer.isHigh()) {
        return Deferring;
      }
      if (defer.isFall()) {
        return TrackingInput;
      }
      if (trigger.isRise()) {
        return Generating;
      }
      return state;
    }

    void enter(State newState) {
      switch (state) {
      case Generating:
        generateMode.exit();
        break;
      case Deferring:
        deferMode.exit();
        trigger.clock(false);
        break;
      case TrackingInput:
        inputMode.exit();
        break;
      case TrackingLevel:
        levelMode.exit();
        break;
      }

      state = newState;

      switch (state) {
      case Deferring:
        deferMode.enter();
        break;
      case Generating:
        generateMode.enter();
        break;
      case TrackingInput:
        inputMode.enter();
        break;
      case TrackingLevel:
        levelMode.enter();
        break;
      }
    }

    void generate(float sampleTime) {
      auto const event = generateMode.execute(trigger, sampleTime);
      if (event == Event::Completed) {
        eocTimer.reset();
        enter(TrackingLevel);
      }
    }

    PhaseTimer eocTimer{1.F};
    State state{TrackingInput};
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
