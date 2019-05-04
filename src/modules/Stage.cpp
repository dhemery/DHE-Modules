#include <utility>

#include "modules/Stage.h"

#include "util/duration.h"
#include "util/signal.h"

namespace DHE {
Stage::Stage()
    : state_machine{[this]() -> bool { return defer_gate_is_active(); },
                    [this]() -> bool { return defer_gate_in(); },
                    [this]() -> bool { return stage_gate_in(); },
                    [this]() -> float { return duration(); },
                    [this](float) { forward(); },
                    [this]() { prepare_to_generate(); },
                    [this](float phase) { generate(phase); },
                    [this](bool active) { set_active(active); },
                    [this](bool eoc) { set_eoc(eoc); }} {
  config(PARAMETER_COUNT, INPUT_COUNT, OUTPUT_COUNT);

  duration.config(&params[DURATION_KNOB]);

  configParam(DURATION_KNOB, 0.f, 1.f, 0.5f, "Duration");
  configParam(LEVEL_KNOB, 0.f, 1.f, 0.5f, "Level", " V", 0.f, 10.f, 0.f);
  configParam(CURVE_KNOB, 0.f, 1.f, 0.5f, "Curvature", "%", 0.f, 200.f, -100.f);

  state_machine.start();
}

auto Stage::defer_gate_in() const -> bool {
  return inputs[DEFER_GATE_IN].value > 0.1;
}

auto Stage::defer_gate_is_active() const -> bool {
  return inputs[DEFER_GATE_IN].active;
}

auto Stage::curvature() const -> float {
  auto rotation = params[CURVE_KNOB].value;
  return Sigmoid::curvature(rotation);
}

auto Stage::envelope_in() const -> float { return inputs[ENVELOPE_IN].value; }

void Stage::forward() { send_out(envelope_in()); }

void Stage::generate(float phase) {
  send_out(scale(taper(phase), start_voltage, level()));
}

auto Stage::level() const -> float {
  auto rotation = params[LEVEL_KNOB].value;
  return Signal::unipolar_range.scale(rotation);
}

void Stage::prepare_to_generate() { start_voltage = envelope_in(); }

void Stage::send_out(float voltage) { outputs[MAIN_OUT].value = voltage; }

void Stage::set_active(bool active) {
  outputs[ACTIVE_OUT].value = active ? 10.f : 0.f;
}

void Stage::set_eoc(bool eoc) { outputs[EOC_OUT].value = eoc ? 10.f : 0.f; }

auto Stage::stage_gate_in() const -> bool {
  return inputs[STAGE_TRIGGER_IN].value > 0.1;
}

void Stage::process(const ProcessArgs &args) {
  state_machine.step(args.sampleTime);
}

auto Stage::taper(float phase) const -> float {
  return Sigmoid::j_taper(phase, curvature());
}
} // namespace DHE
