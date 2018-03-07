#pragma once

#include "util/controls.h"
#include "module.h"
#include "stage-processor.h"

namespace DHE {
struct BoosterStageModule : Module, StageProcessor {
  BoosterStageModule()
      : Module{PARAMETER_COUNT, INPUT_COUNT, OUTPUT_COUNT},
        StageProcessor{} {}

  float defer_in() const override {
    return std::max(input(DEFER_IN), gate_button(DEFER_BUTTON));
  }

  float duration_in() const override {
    auto rotation = modulated(DURATION_KNOB, DURATION_CV);
    const auto &range = Duration::range(param(DURATION_SWITCH));
    return Duration::scaled(rotation, range);
  }

  float envelope_voltage(float held, float phase) const override {
    auto range = Range{held, level_in()};
    return range.scale(taper(phase));
  }

  float level_in() const {
    const auto &range = Level::range(param(LEVEL_SWITCH));
    auto rotation = modulated(LEVEL_KNOB, LEVEL_CV);
    return Level::scaled(rotation, range);
  }

  float stage_in() const override {
    return input(STAGE_IN);
  }

  float trigger_in() const override {
    return std::max(input(TRIG_IN), gate_button(TRIG_BUTTON));
  }

  bool is_active() const override {
    return param(ACTIVE_BUTTON) > 0.5f || StageProcessor::is_active();
  }

  bool is_end_of_cycle() const override {
    return param(EOC_BUTTON) > 0.5f || StageProcessor::is_end_of_cycle();
  }

  bool is_s_taper() const {
    return param(SHAPE_SWITCH) > 0.5;
  }

  void step() override {
    StageProcessor::step();
  }

  float taper(float phase) const {
    auto rotation = modulated(CURVE_KNOB, CURVE_CV);
    return is_s_taper() ? Taper::s(phase, rotation) : Taper::j(phase, rotation);
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
    TRIG_BUTTON,
    PARAMETER_COUNT
  };

  enum InputIds {
    CURVE_CV,
    DEFER_IN,
    DURATION_CV,
    LEVEL_CV,
    STAGE_IN,
    TRIG_IN,
    INPUT_COUNT
  };

  enum OutputIds {
    ACTIVE_OUT,
    EOC_OUT,
    STAGE_OUT,
    OUTPUT_COUNT
  };
};
}
