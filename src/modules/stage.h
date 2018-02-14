#ifndef DHE_STAGE_STAGE_H
#define DHE_STAGE_STAGE_H

#include <memory>

#include <engine.hpp>

#include "util/d-flip-flop.h"
#include "util/ramp.h"
#include "util/follower.h"

namespace DHE {
struct Stage : rack::Module {
  enum ParamIds {
    DURATION_KNOB, LEVEL_KNOB, SHAPE_KNOB, NUM_PARAMS
  };
  enum InputIds {
    IN_INPUT, TRIG_INPUT, DEFER_INPUT, NUM_INPUTS
  };
  enum OutputIds {
    OUT_OUTPUT, EOC_OUTPUT, ACTIVE_OUTPUT, NUM_OUTPUTS
  };

  enum LightIds {
    NUM_LIGHTS
  };

  Stage() : Stage([] { return rack::engineGetSampleTime(); }) {}

  Stage(std::function<float()> sample_time_supplier);

  void step() override;

private:
  DFlipFlop defer_gate;
  Ramp end_of_cycle_pulse;
  Ramp envelope_ramp;
  DFlipFlop envelope_trigger;
  Follower stage_input_follower;

  float duration() const;
  float shape() const;

  void defer();
  void resume();

  void start_envelope();
  float envelope_voltage() const;

  float active_out_voltage() const { return UNIPOLAR_CV.scale(is_active()); }
  float duration_knob_rotation() const { return params[DURATION_KNOB].value; }
  float eoc_out_voltage() const { return UNIPOLAR_CV.scale(end_of_cycle_pulse.is_active()); }
  bool is_active() const { return defer_gate.is_high() || envelope_ramp.is_active(); }
  float level_knob_rotation() const { return params[LEVEL_KNOB].value; }
  float level_knob_voltage() const { return UNIPOLAR_CV.scale(level_knob_rotation()); }
  float out_voltage() const { return defer_gate.is_high() ? stage_input_follower.value() : envelope_voltage(); }
  float shape_knob_rotation() const { return params[SHAPE_KNOB].value; }
  float shape_position() const { return BIPOLAR_NORMAL.scale(shape_knob_rotation()); }
};
} // namespace DHE
#endif
