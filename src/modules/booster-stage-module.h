#ifndef DHE_MODULES_MODULES_BOOSTER_STAGE_MODULE_H
#define DHE_MODULES_MODULES_BOOSTER_STAGE_MODULE_H

#include <engine.hpp>
#include <controllers/duration-control.h>
#include "controllers/level-control.h"
#include "controllers/input-port-control.h"
#include <controllers/shape-control.h>
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
        controller{
            this,
            LevelControl{
                [this] { return params[LEVEL_KNOB].value; },
                [this] { return inputs[LEVEL_CV].value; },
                [this] { return params[LEVEL_SWITCH].value; }
            },
            DurationControl{
                [this] { return params[DURATION_KNOB].value; },
                [this] { return inputs[DURATION_CV].value; },
                [this] { return params[DURATION_SWITCH].value; }
            },
            ShapeControl{
                [this] { return params[SHAPE_KNOB].value; },
                [this] { return inputs[SHAPE_CV].value; },
                [this] { return params[SHAPE_SWITCH].value; }
            },
            InputPortControl{
                [this] { return inputs[DEFER_INPUT].value; },
                [this] { return params[DEFER_BUTTON].value; },
                [this] (float f) { lights[DEFER_BUTTON_LIGHT].value = f; }
            },
            InputPortControl{
                [this] { return inputs[TRIG_INPUT].value; },
                [this] { return params[TRIG_BUTTON].value; },
                [this] (float f) { lights[TRIG_BUTTON_LIGHT].value = f; }
            },
            InputPortControl{[this] { return inputs[IN_INPUT].value; }}
        } {}

  float stage_in() const { return inputs[IN_INPUT].value; }

  void send_eoc(float f) { outputs[EOC_OUTPUT].value = f; }
  void send_stage(float f) { outputs[OUT_OUTPUT].value = f; }
  void send_active(float f) { outputs[ACTIVE_OUTPUT].value = f; }

  float sample_time() const { return rack::engineGetSampleTime(); }
  void step() override { controller.step(); }

private:
  StageController<BoosterStageModule> controller;
};
}
#endif
