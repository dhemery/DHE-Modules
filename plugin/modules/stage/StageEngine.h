#pragma once

#include "Event.h"
#include "components/Range.h"

#include <algorithm>
#include <iostream>

namespace dhe {
namespace stage {

  template <typename Controls, typename GenerateMode> class StageEngine {
  public:
    StageEngine(Controls &controls, GenerateMode &generateMode) : controls{controls}, generateMode{generateMode} {}

    void process(float sampleTime) {
      auto const newState = identifyState();
      if (state != newState) {
        enter(newState);
      }

      switch (state) {
      case Generating:
        std::cerr << "executing generating mode" << std::endl;
        generate(sampleTime);
        break;
      case TrackingLevel:
        controls.output(controls.level());
        break;
      case Deferring:
        controls.output(controls.input());
        break;
      case TrackingInput:
        controls.output(controls.input());
        break;
      }

      advanceEoc(sampleTime);
      controls.showActive(state == Deferring || state == Generating);
      controls.showEoc(isEoc);
    }

  private:
    enum State {
      Deferring,
      Generating,
      TrackingInput,
      TrackingLevel,
    };

    auto identifyState() -> StageEngine::State {
      if (controls.isDeferring()) {
        return Deferring;
      }
      if (state == Deferring) {
        return TrackingInput;
      }
      if (triggerRise()) {
        enter(Generating);
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
        resetTrigger();
        break;
      case TrackingInput:
        break;
      case TrackingLevel:
        break;
      }

      state = newState;

      switch (state) {
      case Generating:
        generateMode.enter(controls.input());
        break;
      case Deferring:
        break;
      case TrackingInput:
        break;
      case TrackingLevel:
        break;
      }
    }

    void resetTrigger() { triggerWasHigh = false; }

    auto triggerRise() -> bool {
      auto const isHigh = controls.isTriggered();
      auto const isRise = isHigh && !triggerWasHigh;
      triggerWasHigh = isHigh;
      return isRise;
    }

    void generate(float sampleTime) {
      auto const event = generateMode.execute(sampleTime);
      if (event == Event::Completed) {
        isEoc = true;
        eocPhase = 0.F;
        enter(TrackingLevel);
      }
    }

    void advanceEoc(float sampleTime) {
      if (eocPhase < 1.F) {
        eocPhase = std::min(1.F, eocPhase + sampleTime / 1e-3F);
        if (eocPhase == 1.F) {
          finishEoc();
        }
      }
    }

    void finishEoc() { isEoc = false; }

    float eocPhase{1.F};
    bool isEoc{false};
    State state{TrackingInput};
    bool triggerWasHigh{false};
    Controls &controls;
    GenerateMode &generateMode;
  };
} // namespace stage
} // namespace dhe
