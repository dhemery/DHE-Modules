#ifndef DHE_MODULES_MODULES_STAGE_MODULE_H
#define DHE_MODULES_MODULES_STAGE_MODULE_H

#include <engine.hpp>
#include "controllers/duration-control.h"
#include "controllers/input-port-control.h"
#include "controllers/level-control.h"
#include "controllers/shape-control.h"
#include "controllers/stage-controller.h"

namespace DHE {
struct StageModule : rack::Module {
  enum ParamIds {
    DURATION_KNOB, LEVEL_KNOB, SHAPE_KNOB,
    NUM_PARAMS
  };
  enum InputIds {
    STAGE_IN, TRIG_IN, DEFER_IN,
    NUM_INPUTS
  };
  enum OutputIds {
    STAGE_OUT, EOC_OUT, ACTIVE_OUT,
    NUM_OUTPUTS
  };

  enum LightIds {
    NUM_LIGHTS
  };

  StageModule()
      : Module{NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS},
        controller{
            [] { return rack::engineGetSampleTime(); },
            LevelControl{[this] { return params[LEVEL_KNOB].value; }},
            DurationControl{[this] { return params[DURATION_KNOB].value; }},
            ShapeControl{[this] { return params[SHAPE_KNOB].value; }},
            InputPortControl{[this] { return inputs[DEFER_IN].value; }},
            InputPortControl{[this] { return inputs[TRIG_IN].value; }},
            InputPortControl{[this] { return inputs[STAGE_IN].value; }},
            OutputPortControl{[this](float f) { outputs[ACTIVE_OUT].value = f; }},
            OutputPortControl{[this](float f) { outputs[EOC_OUT].value = f; }},
            OutputPortControl{[this](float f) { outputs[STAGE_OUT].value = f; }}
        } {}

  void step() override { controller.step(); }

private:
  StageController controller;
};
}
#endif
