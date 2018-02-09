#include <cmath>

#include <engine.hpp>

#include "stage.h"

namespace DHE {

Stage::Stage(std::function<float()> sample_time_supplier)
    : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS),
                 defer_gate{[this] { return inputs[DEFER_INPUT].value; }},
                 end_of_cycle_pulse{1e-3, sample_time_supplier},
                 envelope_ramp{[this] { return duration(); }, sample_time_supplier},
                 envelope_trigger{[this] { return inputs[TRIG_INPUT].value; }},
                 stage_input_follower{[this] { return inputs[IN_INPUT].value; }} {
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

  outputs[OUT_OUTPUT].value = out_voltage();
  outputs[EOC_OUTPUT].value = eoc_out_voltage();
  outputs[ACTIVE_OUTPUT].value = active_out_voltage();
}

void Stage::defer() {
  envelope_trigger.suspend_firing();
  envelope_ramp.stop();
  stage_input_follower.resume();
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
  static const Interval range{1e-3, 10.0f};
  static constexpr float curvature{0.8f}; // Gives ~1s at center position

  return range.scale(sigmoid(duration_knob_rotation(), curvature));
}

float Stage::shape() const {
  static constexpr float shape_curvature{-0.65f};

  return sigmoid(shape_position(), shape_curvature);
}

float Stage::envelope_voltage() const {
  auto shaped{sigmoid(envelope_ramp.phase(), shape())};

  return Interval::scale(shaped, stage_input_follower.value(), level_knob_voltage());
}
} // namespace DHE
