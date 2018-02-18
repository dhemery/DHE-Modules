#ifndef DHE_MODULES_MODULES_STAGE_MODULE_H
#define DHE_MODULES_MODULES_STAGE_MODULE_H

#include <engine.hpp>
#include "controllers/stage-controller.h"
#include "controllers/level-control.h"

namespace DHE {
struct StageModule : rack::Module {
  enum ParamIds {
    DURATION_KNOB, LEVEL_KNOB, SHAPE_KNOB,
    NUM_PARAMS
  };
  enum InputIds {
    IN_INPUT, TRIG_INPUT, DEFER_INPUT,
    NUM_INPUTS
  };
  enum OutputIds {
    OUT_OUTPUT, EOC_OUTPUT, ACTIVE_OUTPUT,
    NUM_OUTPUTS
  };

  enum LightIds {
    NUM_LIGHTS
  };

  StageModule()
      : Module{NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS},
        level{[this] { return params[LEVEL_KNOB].value; }},
        controller{this} {}


  LevelControl level;

  float duration() const {
    static constexpr float curvature{0.8f}; // Gives ~1s at center position
    static constexpr Interval duration_knob_range{1e-3, 10.0f};

    return duration_knob_range.scale(sigmoid(duration_knob(), curvature));
  }

  float shape() const {
    static constexpr float shape_curvature{-0.65f};

    return sigmoid(BIPOLAR_NORMAL.scale(shape_knob()), shape_curvature);
  }

  Interval shape_range() const { return NORMAL; }

  float defer_in() { return inputs[DEFER_INPUT].value; }
  float trigger_in() { return inputs[TRIG_INPUT].value; }
  float stage_in() { return inputs[IN_INPUT].value; }

  void send_eoc(float f) { outputs[EOC_OUTPUT].value = f; }
  void send_stage(float f) { outputs[OUT_OUTPUT].value = f; }
  void send_active(float f) { outputs[ACTIVE_OUTPUT].value = f; }

  void step() override { controller.step(); }
  float sample_time() const { return rack::engineGetSampleTime(); }

private:
  StageController<StageModule> controller;
  float duration_knob() const { return params[DURATION_KNOB].value; }
  float shape_knob() const { return params[SHAPE_KNOB].value; }
};
}
#endif
