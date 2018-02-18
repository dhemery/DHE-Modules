#ifndef DHE_MODULES_MODULES_UPSTAGE_MODULE_H
#define DHE_MODULES_MODULES_UPSTAGE_MODULE_H

#include <algorithm>

#include <engine.hpp>
#include "util/interval.h"
#include "controllers/upstage-controller.h"
#include "level-control.h"

namespace DHE {

struct UpstageModule : rack::Module {
  UpstageModule()
      : rack::Module{NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS},
        level{this, LEVEL_KNOB, LEVEL_CV_INPUT},
        controller{this} {}

  enum ParamIds {
    LEVEL_KNOB, TRIG_BUTTON, WAIT_BUTTON,
    NUM_PARAMS
  };
  enum InputIds {
    TRIG_INPUT, WAIT_INPUT, LEVEL_CV_INPUT,
    NUM_INPUTS
  };
  enum OutputIds {
    TRIG_OUTPUT, OUT_OUTPUT,
    NUM_OUTPUTS
  };
  enum LightIds {
    TRIG_BUTTON_LIGHT, WAIT_BUTTON_LIGHT,
    NUM_LIGHTS
  };

  ModulatedLevelControl<UpstageModule> level;

  void send_trigger(float f) { outputs[TRIG_OUTPUT].value = f; }
  void send_level(float f) { outputs[OUT_OUTPUT].value = f; }

  float wait_in() const { return inputs[WAIT_INPUT].value; }
  float trigger_in() const { return inputs[TRIG_INPUT].value; }

  bool trigger_button_is_pressed() const { return params[TRIG_BUTTON].value > 0.0f; }
  bool wait_button_is_pressed() const { return params[WAIT_BUTTON].value > 0.0f; }

  void step() override {
    controller.step();
    set_trigger_button_light();
    set_wait_button_light();
  }

private:
  UpstageController<UpstageModule> controller;
  void set_trigger_button_light() { lights[TRIG_BUTTON_LIGHT].value = UNIPOLAR_CV.scale(trigger_button_is_pressed()); }
  void set_wait_button_light() { lights[WAIT_BUTTON_LIGHT].value = UNIPOLAR_CV.scale(wait_button_is_pressed()); }
};

}
#endif
