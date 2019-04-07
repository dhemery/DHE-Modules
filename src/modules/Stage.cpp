#include <modules/Stage.h>
#include "util/duration.h"
#include "util/signal.h"

namespace DHE {
Stage::Stage() : rack::Module{PARAMETER_COUNT, INPUT_COUNT, OUTPUT_COUNT} {
  state_machine.start();
}
void Stage::step() { state_machine.step(); }
void Stage::forward() { send_out(envelope_in()); }
void Stage::generate(float phase) {
  send_out(scale(taper(phase), start_voltage, level()));
}
void Stage::prepare_to_generate() { start_voltage = envelope_in(); }
auto Stage::defer_gate_is_active() const -> bool {
  return inputs[DEFER_GATE_IN].active;
}
void Stage::set_active(bool active) {
  outputs[ACTIVE_OUT].value = active ? 10.f : 0.f;
}
void Stage::set_eoc(bool eoc) { outputs[EOC_OUT].value = eoc ? 10.f : 0.f; }
auto Stage::defer_gate_in() const -> bool {
  return inputs[DEFER_GATE_IN].value > 0.1;
}
auto Stage::stage_gate_in() const -> bool {
  return inputs[STAGE_TRIGGER_IN].value > 0.1;
}
auto Stage::sample_time() const -> float { return rack::engineGetSampleTime(); }
auto Stage::duration() const -> float {
  auto rotation = params[DURATION_KNOB].value;
  return DHE::duration(rotation);
}
auto Stage::curvature() const -> float {
  auto rotation = params[CURVE_KNOB].value;
  return Sigmoid::curvature(rotation);
}
auto Stage::envelope_in() const -> float { return inputs[ENVELOPE_IN].value; }
auto Stage::level() const -> float {
  auto rotation = params[LEVEL_KNOB].value;
  return Signal::unipolar_range.scale(rotation);
}
void Stage::send_out(float voltage) { outputs[MAIN_OUT].value = voltage; }
auto Stage::taper(float phase) const -> float {
  return Sigmoid::j_taper(phase, curvature());
}
}
