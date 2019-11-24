#pragma once

#include "components/Range.h"

#include <algorithm>

namespace dhe {
namespace stage {

  template <typename Controls> class StageEngine {
  public:
    StageEngine(Controls &controls) : controls{controls} {}

    void process(float sampleTime) {
      auto const newState = identifyState();
      if (state != newState) {
        enter(newState);
      }

      switch (state) {
      case Generating:
        generate(sampleTime);
        break;
      case TrackingLevel:
        controls.output(controls.level());
        break;
      case Deferring:
      case TrackingInput:
        controls.output(controls.input());
      }

      advanceEoc(sampleTime);
      controls.sendActive(state == Deferring || state == Generating);
      controls.sendEoc(isEoc);
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
      if (state == Deferring) {
        resetTrigger();
      }

      if (newState == Generating) {
        resetGenerator();
      }
      state = newState;
    }

    void resetTrigger() { triggerWasHigh = false; }

    auto triggerRise() -> bool {
      auto const isHigh = controls.isTriggered();
      auto const isRise = isHigh && !triggerWasHigh;
      triggerWasHigh = isHigh;
      return isRise;
    }

    void resetGenerator() {
      startVoltage = controls.input();
      stagePhase = 0.F;
    }

    void generate(float sampleTime) {
      if (stagePhase < 1.F) {
        auto const duration = controls.duration();
        auto const phaseDelta = sampleTime / duration;
        stagePhase = std::min(1.F, stagePhase + phaseDelta);

        auto const *taper = controls.taper();
        auto const level = controls.level();
        auto const curvature = controls.curvature();
        auto const taperedPhase = taper->apply(stagePhase, curvature);
        controls.output(scale(taperedPhase, startVoltage, level));
        if (stagePhase == 1.F) {
          finishGenerating();
        }
      }
    }

    void finishGenerating() {
      startEoc();
      enter(TrackingLevel);
    }

    void startEoc() {
      isEoc = true;
      eocPhase = 0.F;
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
    float stagePhase{0.F};
    float startVoltage{0.F};
    State state{TrackingInput};
    bool triggerWasHigh{false};
    Controls &controls;
  };
} // namespace stage
} // namespace dhe
