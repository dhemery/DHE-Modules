#ifndef DHE_MODULES_MODULES_STAGE_MODULE_H
#define DHE_MODULES_MODULES_STAGE_MODULE_H

#include <engine.hpp>
#include "controllers/duration-control.h"
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
        controller{this,
                   LevelControl{[this] { return params[LEVEL_KNOB].value; }},
                   DurationControl{[this] { return params[DURATION_KNOB].value; }},
                   ShapeControl{[this] { return params[SHAPE_KNOB].value; }}} {}

  float defer_in() { return inputs[DEFER_INPUT].value; }
  float trigger_in() { return inputs[TRIG_INPUT].value; }
  float stage_in() { return inputs[IN_INPUT].value; }

  void send_eoc(float f) { outputs[EOC_OUTPUT].value = f; }
  void send_stage(float f) { outputs[OUT_OUTPUT].value = f; }
  void send_active(float f) { outputs[ACTIVE_OUTPUT].value = f; }

  void step() override { controller.step(); }
  float sample_time() const { return rack::engineGetSampleTime(); }

private:
  StageController <StageModule> controller;
};
}
#endif
