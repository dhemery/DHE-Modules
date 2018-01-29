//
// Created by Dale Emery on 1/28/18.
//

#ifndef DHE_UPSTAGE_UPSTAGE_H
#define DHE_UPSTAGE_UPSTAGE_H

#include <engine.hpp>
#include "util/range.h"

namespace DHE {

struct Upstage : rack::Module {
  static constexpr char const *SLUG{"Upstage"};

  enum ParamIds {
    LEVEL_KNOB, TRIG_BUTTON, WAIT_BUTTON, NUM_PARAMS
  };
  enum InputIds {
    TRIG_IN, WAIT_GATE_IN, NUM_INPUTS
  };
  enum OutputIds {
    TRIG_OUT, ENVELOPE_OUT, NUM_OUTPUTS
  };

  enum LightIds {
    TRIG_LIGHT, WAIT_LIGHT, NUM_LIGHTS
  };

  Upstage();
  void step() override;

private:

  bool is_sending_triggers() const { return wait_cv_in() < 1.0f && wait_button_in() < 1.0f; }
  float level_knob_in() const { return this->params[LEVEL_KNOB].value; }
  float level_knob_voltage() const { return UNIPOLAR_VOLTAGE.scale(level_knob_in()); }
  float trigger_button_in() const { return params[TRIG_BUTTON].value; }
  float trigger_button_voltage() const { return UNIPOLAR_VOLTAGE.scale(trigger_button_in()); }
  float trigger_cv_in() const { return inputs[TRIG_IN].value; }
  float trigger_out_voltage() const { return std::max(trigger_cv_in(), trigger_button_voltage()); }
  float wait_button_in() const { return params[WAIT_BUTTON].value; }
  float wait_cv_in() const { return inputs[WAIT_GATE_IN].value; }
};

}
#endif
