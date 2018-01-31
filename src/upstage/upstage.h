//
// Created by Dale Emery on 1/28/18.
//

#ifndef DHE_UPSTAGE_UPSTAGE_H
#define DHE_UPSTAGE_UPSTAGE_H

#include <algorithm>

#include "engine.hpp"
#include "util/range.h"

namespace DHE {

struct Upstage : rack::Module {
  enum ParamIds {
    LEVEL_KNOB, TRIG_BUTTON, WAIT_BUTTON, NUM_PARAMS
  };
  enum InputIds {
    TRIG_INPUT, WAIT_INPUT, LEVEL_CV_INPUT, NUM_INPUTS
  };
  enum OutputIds {
    TRIG_OUTPUT, OUT_OUTPUT, NUM_OUTPUTS
  };
  enum LightIds {
    TRIG_BUTTON_LIGHT, WAIT_BUTTON_LIGHT, NUM_LIGHTS
  };

  Upstage() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS) {}

  void step() override {
    lights[TRIG_BUTTON_LIGHT].value = trigger_button_is_pressed();
    lights[WAIT_BUTTON_LIGHT].value = wait_button_is_pressed();

    outputs[TRIG_OUTPUT].value = trigger_out_voltage();
    outputs[OUT_OUTPUT].value = level_voltage();
  }

private:
  bool is_sending_triggers() const { return wait_port_in() < 1.0f and not wait_button_is_pressed(); }
  float level_cv_in() const { return inputs[LEVEL_CV_INPUT].value; }
  float level_knob_rotation() const { return params[LEVEL_KNOB].value; }
  float level_knob_voltage() const { return UNIPOLAR_VOLTAGE.scale(level_knob_rotation()); }
  float level_voltage() const { return UNIPOLAR_VOLTAGE.clamp(level_knob_voltage() + level_cv_in()); }
  bool trigger_button_is_pressed() const { return params[TRIG_BUTTON].value > 0.0f; }
  float trigger_button_voltage() const { return UNIPOLAR_VOLTAGE.scale(trigger_button_is_pressed()); }
  float trigger_port_in() const { return inputs[TRIG_INPUT].value; }
  float trigger_out_voltage() const { return is_sending_triggers() ? trigger_voltage() : 0.0f; }
  float trigger_voltage() const { return std::max(trigger_port_in(), trigger_button_voltage()); }
  bool wait_button_is_pressed() const { return params[WAIT_BUTTON].value > 0.0f; }
  float wait_port_in() const { return inputs[WAIT_INPUT].value; }
};

}
#endif
