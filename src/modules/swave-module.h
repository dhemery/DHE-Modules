#pragma once

#include <algorithm>

#include <engine.hpp>

#include "util/interval.h"
#include <controllers/attenuverter.h>
#include "controllers/swave-controller.h"
#include "controllers/shape-control.h"

namespace DHE {

struct SwaveModule : rack::Module {
  enum ParamIds {
    CURVE_KNOB, SHAPE_SWITCH,
    TRIM_KNOB,
    NUM_PARAMS
  };
  enum InputIds {
    CURVE_CV, SWAVE_IN,
    NUM_INPUTS
  };
  enum OutputIds {
    SWAVE_OUT,
    NUM_OUTPUTS
  };
  enum LightIds {
    NUM_LIGHTS
  };

  SwaveModule()
      : rack::Module{NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS},
        controller{
            ShapeControl{
                [this] { return params[CURVE_KNOB].value; },
                Attenuverter{
                  [this] { return inputs[CURVE_CV].value; },
                  [this] { return params[TRIM_KNOB].value; }
                },
                [this] { return params[SHAPE_SWITCH].value; }
            },
            [this] { return inputs[SWAVE_IN].value; },
            [this](
                float f
            ) { outputs[SWAVE_OUT].value = f; }
        } {}

  void step() override {
    controller.step();
  }

private:
  SwaveController controller;
};

}