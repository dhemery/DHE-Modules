#pragma once

#include <memory>

#include "dsp/digital.hpp"
#include "rack.hpp"
#include "util/DFlipFlop.hpp"
#include "util/Ramp.hpp"
#include "util/Follower.hpp"

namespace DHE {
struct Stage : rack::Module {
  static constexpr char const *SLUG {"Stage"};
  static constexpr char const *NAME {SLUG};

  enum ParamIds {
    DURATION_KNOB, LEVEL_KNOB, SHAPE_KNOB, NUM_PARAMS
  };
  enum InputIds {
    STAGE_IN, TRIGGER_IN, DEFER_GATE_IN, NUM_INPUTS
  };
  enum OutputIds {
    STAGE_OUT, EOC_TRIGGER_OUT, ACTIVE_GATE_OUT, NUM_OUTPUTS
  };
  enum LightIds {
    NUM_LIGHTS
  };

  Stage();

  void step() override;

private:
  DFlipFlop deferGate;
  Ramp endOfCyclePulse;
  Ramp envelopeRamp;
  DFlipFlop envelopeTrigger;
  Follower stageInputFollower;

  void defer();
  void resume();
  float duration() const;
  float stageIn() const;
  void startEnvelope();
  float envelopeOut() const;
};
} // namespace DHE
