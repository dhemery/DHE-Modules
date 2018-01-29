//
// Created by Dale Emery on 1/28/18.
//

#ifndef DHE_UPSTAGE_UPSTAGE_H
#define DHE_UPSTAGE_UPSTAGE_H

#include <engine.hpp>
namespace DHE {

struct Upstage : rack::Module {
  static constexpr char const *SLUG{"Upstage"};
  static constexpr char const *NAME{SLUG};

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
    NUM_LIGHTS
  };

  Upstage();
  void step() override;
};

}
#endif
