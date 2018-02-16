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

  float level() const {
    Interval level_range = level_switch () > 0.1f ? UNIPOLAR_CV : BIPOLAR_CV;
    return level_range.scale(level_knob());
  }

  float duration() const {
    static constexpr float curvature{0.8f}; // Gives ~1s at center position
    static constexpr Interval duration_knob_range{1e-3, 10.0f};

    return duration_knob_range.scale(sigmoid(duration_knob(), curvature));
  }

  float shape() const {
    static constexpr float shape_curvature{-0.65f};

    return sigmoid(BIPOLAR_NORMAL.scale(shape_knob()), shape_curvature);
  }

  float defer_in() const { return inputs[DEFER_INPUT].value; }
  float trigger_in() const { return inputs[TRIG_INPUT].value; }
  float stage_in() const { return inputs[IN_INPUT].value; }


  void send_eoc(float f) { outputs[EOC_OUTPUT].value = f; }
  void send_stage(float f) { outputs[OUT_OUTPUT].value = f; }
  void send_active(float f) { outputs[ACTIVE_OUTPUT].value = f; }

  float sample_time() const { return rack::engineGetSampleTime(); }
  void step() override { controller.step(); }

private:
  StageController<BoosterStageModule> controller;

  float level_knob() const { return params[LEVEL_KNOB].value; }
  float level_cv() const { return inputs[LEVEL_CV].value; }
  float level_switch() const { return params[LEVEL_SWITCH].value; }

  float duration_knob() const { return params[DURATION_KNOB].value; }
  float duration_cv() const { return inputs[DURATION_CV].value; }
  float duration_switch() const { return params[DURATION_SWITCH].value; }

  float shape_knob() const { return params[SHAPE_KNOB].value; }
  float shape_cv() const { return inputs[SHAPE_CV].value; }
  float shape_switch() const { return params[SHAPE_SWITCH].value; }
};
}
#endif
