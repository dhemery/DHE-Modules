#pragma once

#include <memory>

#include "dsp/digital.hpp"
#include "rack.hpp"

#include "util/d-flip-flop.h"
#include "util/ramp.h"
#include "util/follower.h"

namespace DHE {
struct Stage : rack::Module {
  static constexpr char const *SLUG{"Stage"};
  static constexpr char const *NAME{SLUG};

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

  float stageIn() const;
  float duration() const;
  float level() const;
  float shape() const;

  void defer();
  void resume();

  void startEnvelope();
  float envelopeOut() const;
};
} // namespace DHE
