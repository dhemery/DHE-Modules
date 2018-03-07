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

  float defer_in() const override {
    return input(DEFER_IN);
  }

  float duration_in() const override {
    return Duration::scaled(param(DURATION_KNOB), Duration::MEDIUM_RANGE);
  }

  float envelope_voltage(float held, float phase) const override {
    auto range = Range{held, level_in()};
    return range.scale(taper(phase));
  }

  float level_in() const {
    return Level::scaled(param(LEVEL_KNOB));
  }

  float stage_in() const override {
    return input(STAGE_IN);
  }

  float gate_in() const override {
    return input(TRIG_IN);
  }

  void step() override {
    StageProcessor::step();
  }

  float taper(float phase) const {
    return Taper::j(phase, param(CURVE_KNOB));
  }

  void send_active_out(float f) override {
    outputs[ACTIVE_OUT].value = f;
  }

  void send_eoc_out(float f) override {
    outputs[EOC_OUT].value = f;
  }

  void send_stage_out(float f) override {
    outputs[STAGE_OUT].value = f;
  }

  enum ParameterIIds {
    DURATION_KNOB,
    LEVEL_KNOB,
    CURVE_KNOB,
    PARAMETER_COUNT
  };

  enum InputIds {
    STAGE_IN,
    TRIG_IN,
    DEFER_IN,
    INPUT_COUNT
  };

  enum OutputIds {
    STAGE_OUT,
    EOC_OUT,
    ACTIVE_OUT,
    OUTPUT_COUNT
  };
};
}