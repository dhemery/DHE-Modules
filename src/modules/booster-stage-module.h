#ifndef DHE_MODULES_MODULES_BOOSTER_STAGE_MODULE_H
#define DHE_MODULES_MODULES_BOOSTER_STAGE_MODULE_H

#include <engine.hpp>
#include "controllers/stage-controller.h"

namespace DHE {
struct BoosterStageModule : rack::Module {
  enum ParamIds {
    DURATION_KNOB, LEVEL_KNOB, SHAPE_KNOB,
    LEVEL_SWITCH, SHAPE_SWITCH, DURATION_SWITCH,
    DEFER_BUTTON, TRIG_BUTTON, ACTIVE_BUTTON, EOC_BUTTON,
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
    DEFER_BUTTON_LIGHT, TRIG_BUTTON_LIGHT, ACTIVE_BUTTON_LIGHT, EOC_BUTTON_LIGHT,
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
    static constexpr float curvature{0.8f};

    return duration_range().scale(sigmoid(duration_knob(), curvature));
  }

  Interval duration_range() const {
    static constexpr auto short_duration = Interval{1e-4, 1.f};
    static constexpr auto medium_scale = 10.f;
    static constexpr auto long_scale = 100.f;
    static constexpr auto medium_duration = Interval{short_duration.lower_bound * medium_scale, short_duration.upper_bound * medium_scale};
    static constexpr auto long_duration = Interval{short_duration.lower_bound * long_scale, short_duration.upper_bound * long_scale};
    if (duration_switch() < 0.2f) {
      return short_duration;
    } else if (duration_switch() > 0.8f) {
      return long_duration;
    }
    return medium_duration;
  }

  float shape() const {
    static constexpr float shape_curvature{-0.65f};

    return sigmoid(BIPOLAR_NORMAL.scale(shape_knob()), shape_curvature);
  }

  Interval shape_range() const { return shape_switch() < 0.2f ? NORMAL : BIPOLAR_NORMAL; }

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
