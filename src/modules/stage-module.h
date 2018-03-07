#pragma once

#include <engine.hpp>

#include "module.h"
#include "stage-processor.h"
#include "util/controls.h"
#include "util/range.h"

namespace DHE {

struct StageModule : public Module, StageProcessor {
  StageModule()
      : Module(PARAMETER_COUNT, INPUT_COUNT, OUTPUT_COUNT),
        StageProcessor{} {}

  float defer_gate_in() const override {
    return input(DEFER_IN);
  }

  float duration_in() const override {
    return Duration::scaled(param(DURATION_KNOB), Duration::MEDIUM_RANGE);
  }

  float envelope_gate_in() const override {
    return input(TRIGGER_IN);
  }

  float envelope_in() const override {
    return input(ENVELOPE_IN);
  }

  float envelope_out(float phase_0_voltage, float phase) const {
    return scale(taper(phase), phase_0_voltage, level_in());
  }

  float level_in() const {
    return Level::scaled(param(LEVEL_KNOB));
  }

  void step() override {
    StageProcessor::step();
  }

  float taper(float phase) const {
    return Taper::j(phase, param(CURVE_KNOB));
  }

  void send_active_out(bool is_active) override {
    outputs[ACTIVE_OUT].value = UNIPOLAR_SIGNAL_RANGE.scale(is_active);
  }

  void send_envelope_out(float phase_0_voltage, float phase) override {
    outputs[ENVELOPE_OUT].value = envelope_out(phase_0_voltage, phase);
  }

  void send_eoc_out(bool is_pulsing) override {
    outputs[EOC_OUT].value = UNIPOLAR_SIGNAL_RANGE.scale(is_pulsing);
  }

  enum ParameterIIds {
    DURATION_KNOB,
    LEVEL_KNOB,
    CURVE_KNOB,
    PARAMETER_COUNT
  };

  enum InputIds {
    ENVELOPE_IN,
    TRIGGER_IN,
    DEFER_IN,
    INPUT_COUNT
  };

  enum OutputIds {
    ENVELOPE_OUT,
    EOC_OUT,
    ACTIVE_OUT,
    OUTPUT_COUNT
  };
};
}