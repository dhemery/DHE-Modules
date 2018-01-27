#include <cmath>
#include "stage.h"

namespace DHE {

Stage::Stage() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS),
                 defer_gate{[this] { return inputs[DEFER_GATE_IN].value; }},
                 end_of_cycle_pulse{1e-3, &rack::engineGetSampleTime},
                 envelope_ramp{[this] { return duration(); }, &rack::engineGetSampleTime},
                 envelope_trigger{[this] { return inputs[TRIGGER_IN].value; }},
                 stage_input_follower{[this] { return stage_input(); }} {
  defer_gate.on_rising_edge([this] { defer(); });
  defer_gate.on_falling_edge([this] { resume(); });

  envelope_trigger.on_rising_edge([this] { start_envelope(); });

  envelope_ramp.on_end_of_cycle([this] { end_of_cycle_pulse.start(); });
}

void Stage::step() {
  defer_gate.step();
  envelope_ramp.step();
  envelope_trigger.step();
  end_of_cycle_pulse.step();

  outputs[STAGE_OUT].value = defer_gate.is_high() ? stage_input_follower.value() : envelope_out();
  outputs[EOC_TRIGGER_OUT].value = UNIPOLAR_VOLTAGE.scale(end_of_cycle_pulse.is_active());
  outputs[ACTIVE_GATE_OUT].value = UNIPOLAR_VOLTAGE.scale(defer_gate.is_high() || envelope_ramp.is_active());
}

void Stage::defer() {
  envelope_trigger.suspend_firing();
  envelope_ramp.stop();
  stage_input_follower.follow();
}

void Stage::resume() {
  stage_input_follower.pause();
  envelope_trigger.resume_firing();
}

void Stage::start_envelope() {
  stage_input_follower.pause();
  envelope_ramp.start();
}

float Stage::duration() const {
  static const Range range{1e-3, 10.0f};
  static constexpr float curvature{0.8f}; // Gives ~1s at center position

  return range.scale(sigmoid(params[DURATION_KNOB].value, curvature));
}

float Stage::shape() const {
  static constexpr float curvature{-0.65f};

  return sigmoid(BIPOLAR_NORMAL.scale(params[SHAPE_KNOB].value), curvature);
}

float Stage::envelope_out() const {
  float shaped = sigmoid(envelope_ramp.phase(), shape());
  return Range::scale(shaped, stage_input_follower.value(), level());
}

float Stage::level() const { return UNIPOLAR_VOLTAGE.scale(params[LEVEL_KNOB].value); }

float Stage::stage_input() const { return inputs[STAGE_IN].value; }
} // namespace DHE
