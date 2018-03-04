#pragma once

#include <algorithm>

#include <engine.hpp>
#include "util/interval.h"
#include "controllers/upstage-controller.h"
#include "controllers/level-control.h"

namespace DHE {

struct UpstageModule : rack::Module {
  enum ParamIds {
    LEVEL_KNOB, TRIG_BUTTON, WAIT_BUTTON,
    LEVEL_SWITCH,
    NUM_PARAMS
  };
  enum InputIds {
    TRIG_IN, WAIT_IN, LEVEL_CV_INPUT,
    NUM_INPUTS
  };
  enum OutputIds {
    TRIG_OUT, STAGE_OUT,
    NUM_OUTPUTS
  };
  enum LightIds {
    NUM_LIGHTS
  };

  UpstageModule()
      : rack::Module{NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS},
        controller{
            LevelControl{
                [this] { return params[LEVEL_KNOB].value; },
                [this] { return inputs[LEVEL_CV_INPUT].value; },
                [this] { return params[LEVEL_SWITCH].value; }
            },
            InputPortControl{
                [this] { return inputs[WAIT_IN].value; },
                ButtonControl{[this] { return params[WAIT_BUTTON].value; }}
            },
            InputPortControl{
                [this] { return inputs[TRIG_IN].value; },
                ButtonControl {[this] { return params[TRIG_BUTTON].value; }}
            },
            OutputPortControl{[this](float f) { outputs[TRIG_OUT].value = f; }},
            [this](float f) { outputs[STAGE_OUT].value = f; }
        } {}

  void step() override {
    controller.step();
  }

private:
  UpstageController controller;
};

}