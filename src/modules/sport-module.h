#pragma once

#include <algorithm>

#include <engine.hpp>

#include "util/interval.h"
#include "controllers/sport-controller.h"
#include "controllers/shape-control.h"

namespace DHE {

struct SportModule : rack::Module {
  enum ParamIds {
    SHAPE_KNOB, SHAPE_SWITCH,
    NUM_PARAMS
  };
  enum InputIds {
    SHAPE_CV, SPORT_IN,
    NUM_INPUTS
  };
  enum OutputIds {
    SPORT_OUT,
    NUM_OUTPUTS
  };
  enum LightIds {
    NUM_LIGHTS
  };

  SportModule()
      : rack::Module{NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS},
        controller{
            ShapeControl{
                [this] { return params[SHAPE_KNOB].value; },
                [this] { return inputs[SHAPE_CV].value; },
                [this] { return params[SHAPE_SWITCH].value; }
            },
            [this] { return inputs[SPORT_IN].value; },
            [this](float f) { outputs[SPORT_OUT].value = f; }
        } {}

  void step() override {
    controller.step();
  }

private:
  SportController controller;
};

}