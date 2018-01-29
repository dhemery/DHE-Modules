#ifndef DHE_STAGE_STAGE_H
#define DHE_STAGE_STAGE_H

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
    ENVELOPE_IN, TRIGGER_IN, DEFER_GATE_IN, NUM_INPUTS
  };
  enum OutputIds {
    ENVELOPE_OUT, EOC_TRIGGER_OUT, ACTIVE_GATE_OUT, NUM_OUTPUTS
  };

  enum LightIds {
    NUM_LIGHTS
  };

  Stage();

  void step() override;

private:
  DFlipFlop defer_gate;
  Ramp end_of_cycle_pulse;
  Ramp envelope_ramp;
  DFlipFlop envelope_trigger;
  Follower stage_input_follower;

  float stage_input() const;
  float duration() const;
  float level() const;
  float shape() const;

  void defer();
  void resume();

  void start_envelope();
  float envelope_out() const;
};
} // namespace DHE
#endif
