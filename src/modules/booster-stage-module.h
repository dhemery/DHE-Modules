#pragma once

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
    STAGE_IN, TRIG_IN, DEFER_IN,
    LEVEL_CV, DURATION_CV, SHAPE_CV,
    NUM_INPUTS
  };
  enum OutputIds {
    STAGE_OUT, EOC_OUT, ACTIVE_OUT,
    NUM_OUTPUTS
  };

  enum LightIds {
    DEFER_BUTTON_LIGHT, TRIG_BUTTON_LIGHT, ACTIVE_BUTTON_LIGHT, EOC_BUTTON_LIGHT,
    NUM_LIGHTS
  };

  BoosterStageModule()
      : Module{NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS},
        controller{
            [] { return rack::engineGetSampleTime(); },
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
                [this] { return inputs[DEFER_IN].value; },
                ButtonControl{
                    [this] { return params[DEFER_BUTTON].value; },
                    [this](float f) { lights[DEFER_BUTTON_LIGHT].value = f; }
                }
            },
            InputPortControl{
                [this] { return inputs[TRIG_IN].value; },
                ButtonControl{
                    [this] { return params[TRIG_BUTTON].value; },
                    [this](float f) { lights[TRIG_BUTTON_LIGHT].value = f; }
                }
            },
            [this] { return inputs[STAGE_IN].value; },
            OutputPortControl{
                [this](float f) { outputs[ACTIVE_OUT].value = f; },
                ButtonControl{
                    [this] { return params[ACTIVE_BUTTON].value; },
                    [this](float f) { lights[ACTIVE_BUTTON_LIGHT].value = f; }
                }
            },
            OutputPortControl{
                [this](float f) { outputs[EOC_OUT].value = f; },
                ButtonControl{
                    [this] { return params[EOC_BUTTON].value; },
                    [this](float f) { lights[EOC_BUTTON_LIGHT].value = f; }
                }
            },
            [this](float f) { outputs[STAGE_OUT].value = f; }
        } {}

  void step() override { controller.step(); }

private:
  StageController controller;
};
}
