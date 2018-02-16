#ifndef DHE_MODULES_MODULES_BOOSTER_STAGE_MODULE_H
#define DHE_MODULES_MODULES_BOOSTER_STAGE_MODULE_H

#include <engine.hpp>
#include "controllers/stage-controller.h"

namespace DHE {
struct BoosterStageModule : rack::Module {
  enum ParamIds {
    DURATION_KNOB, LEVEL_KNOB, SHAPE_KNOB,
    LEVEL_SWITCH, SHAPE_SWITCH, DURATION_SWITCH,
    NUM_PARAMS
  };
  enum InputIds {
    IN_INPUT, TRIG_INPUT, DEFER_INPUT,
    LEVEL_CV, DURATION_CV, SHAPE_CV,
    NUM_INPUTS
  };
  enum OutputIds {
    OUT_OUTPUT, EOC_OUTPUT, ACTIVE_OUTPUT,
    NUM_OUTPUTS
  };

  enum LightIds {
    NUM_LIGHTS
  };

  BoosterStageModule()
      : Module{NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS},
        controller{this} {}

  void step() override {
    controller.step();
  }

  float level() const { return UNIPOLAR_CV.scale(level_knob_rotation()); }

  float duration() const {
    static const Interval range{1e-3, 10.0f};
    static constexpr float curvature{0.8f}; // Gives ~1s at center position

    return range.scale(sigmoid(duration_knob_rotation(), curvature));
  }

  float shape() const {
    static constexpr float shape_curvature{-0.65f};

    return sigmoid(shape_position(), shape_curvature);
  }

  float defer_in() { return inputs[DEFER_INPUT].value; }
  float trigger_in() { return inputs[TRIG_INPUT].value; }
  float stage_in() { return inputs[IN_INPUT].value; }

  void send_eoc(float f) {
    outputs[EOC_OUTPUT].value = f;
  }

  void send_stage(float f) {
    outputs[OUT_OUTPUT].value = f;
  }

  void send_active(float f) {
    outputs[ACTIVE_OUTPUT].value = f;
  }

  float sample_time() const { return rack::engineGetSampleTime(); }

private:
  StageController<BoosterStageModule> controller;
  float duration_knob_rotation() const { return params[DURATION_KNOB].value; }
  float level_knob_rotation() const { return params[LEVEL_KNOB].value; }
  float shape_knob_rotation() const { return params[SHAPE_KNOB].value; }
  float shape_position() const { return BIPOLAR_NORMAL.scale(shape_knob_rotation()); }
};
} // namespace DHE
#endif
