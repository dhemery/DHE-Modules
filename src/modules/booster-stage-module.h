#pragma once

#include "util/controls.h"
#include "module.h"
#include "stage-processor.h"

namespace DHE {
struct BoosterStageModule : Module, StageProcessor {
  BoosterStageModule()
      : Module{PARAMETER_COUNT, INPUT_COUNT, OUTPUT_COUNT},
        StageProcessor{} {}

  float defer_gate_in() const override {
    return std::max(input(DEFER_IN), gate_button(DEFER_BUTTON));
  }

  float duration_in() const override {
    auto rotation = modulated(DURATION_KNOB, DURATION_CV);
    const auto &range = Duration::range(param(DURATION_SWITCH));
    return Duration::scaled(rotation, range);
  }

  float level_in() const {
    const auto &range = Level::range(param(LEVEL_SWITCH));
    auto rotation = modulated(LEVEL_KNOB, LEVEL_CV);
    return Level::scaled(rotation, range);
  }

  float envelope_gate_in() const override {
    return std::max(input(TRIGGER_IN), gate_button(TRIGGER_BUTTON));
  }

  float envelope_in() const override {
    return input(ENVELOPE_IN);
  }

  float envelope_out(float phase_0_voltage, float phase) const {
    return scale(taper(phase), phase_0_voltage, level_in());
  }

  bool is_s_taper() const {
    return param(SHAPE_SWITCH) > 0.5;
  }

  void send_active_out(bool is_active) override {
    outputs[ACTIVE_OUT].value = UNIPOLAR_SIGNAL_RANGE.scale(is_active || param(ACTIVE_BUTTON) > 0.5f);
  }

  void send_envelope_out(float phase_0_voltage, float phase) override {
    outputs[ENVELOPE_OUT].value = envelope_out(phase_0_voltage, phase);
  }

  void send_eoc_out(bool is_pulsing) override {
    outputs[EOC_OUT].value = UNIPOLAR_SIGNAL_RANGE.scale(is_pulsing || param(EOC_BUTTON) > 0.5f);
  }

  void step() override {
    StageProcessor::step();
  }

  float taper(float phase) const {
    auto rotation = modulated(CURVE_KNOB, CURVE_CV);
    return is_s_taper() ? Taper::s(phase, rotation) : Taper::j(phase, rotation);
  }

  enum ParameterIds {
    ACTIVE_BUTTON,
    CURVE_KNOB,
    DEFER_BUTTON,
    DURATION_KNOB,
    DURATION_SWITCH,
    EOC_BUTTON,
    LEVEL_KNOB,
    LEVEL_SWITCH,
    SHAPE_SWITCH,
    TRIGGER_BUTTON,
    PARAMETER_COUNT
  };

  enum InputIds {
    CURVE_CV,
    DEFER_IN,
    DURATION_CV,
    LEVEL_CV,
    ENVELOPE_IN,
    TRIGGER_IN,
    INPUT_COUNT
  };

  enum OutputIds {
    ACTIVE_OUT,
    EOC_OUT,
    ENVELOPE_OUT,
    OUTPUT_COUNT
  };
};
}
