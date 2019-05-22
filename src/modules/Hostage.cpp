#include <utility>

#include "modules/Hostage.h"
#include "modules/controls/ModulatedKnob.h"
#include "modules/controls/RackControls.h"
#include "modules/params/DurationParams.h"

namespace DHE {
Hostage::Hostage()
    : state_machine{[this]() -> bool { return defer_gate_is_active(); },
                    [this]() -> bool { return defer_gate_in(); },
                    [this]() -> bool { return stage_gate_in(); },
                    [this]() -> bool { return is_sustain_mode(); },
                    [this]() -> float { return duration(); },
                    [this](float) { forward(); },
                    [this](bool active) { set_active(active); },
                    [this](bool eoc) { set_eoc(eoc); }} {
  config(PARAMETER_COUNT, INPUT_COUNT, OUTPUT_COUNT);

  auto durationKnob = ModulatedKnob{params[DURATION_KNOB], inputs[DURATION_CV],
                                    constantFullyRotatedKnobParam};

  auto getDurationRange = [this]() -> Range const * { return durationRange; };
  Duration::configKnob(this, DURATION_KNOB, getDurationRange);
  Duration::configSwitch(this, DURATION_RANGE_SWITCH, getDurationRange);

  auto getDurationRotation = [durationKnob]() -> float {
    return durationKnob.rotation();
  };

  duration = Duration::from(getDurationRotation, getDurationRange);

  configParam(HOSTAGE_MODE_SWITCH, 0.f, 1.f, 0.f, "Mode");

  state_machine.start();
}

void Hostage::process(const ProcessArgs &args) {
  state_machine.step(args.sampleTime);
}

void Hostage::forward() { send_out(envelope_in()); }

auto Hostage::defer_gate_is_active() const -> bool {
  return inputs[DEFER_GATE_IN].active;
}

auto Hostage::is_sustain_mode() -> bool {
  return params[HOSTAGE_MODE_SWITCH].getValue() > 0.5f;
}

void Hostage::set_active(bool active) {
  const auto voltage = active ? 10.f : 0.f;
  outputs[ACTIVE_OUT].setVoltage(voltage);
}

void Hostage::set_eoc(bool eoc) {
  const auto voltage = eoc ? 10.f : 0.f;
  outputs[EOC_OUT].setVoltage(voltage);
}

auto Hostage::defer_gate_in() -> bool {
  return inputs[DEFER_GATE_IN].getVoltage() > 0.1f;
}

auto Hostage::stage_gate_in() -> bool {
  return inputs[STAGE_GATE_IN].getVoltage() > 0.1f;
}

auto Hostage::envelope_in() -> float { return inputs[MAIN_IN].getVoltage(); }

void Hostage::send_out(float voltage) { outputs[MAIN_OUT].setVoltage(voltage); }
} // namespace DHE
