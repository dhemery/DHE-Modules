//
// Created by Dale Emery on 1/28/18.
//

#include <algorithm>
#include "util/range.h"
#include "upstage.h"

namespace DHE {

Upstage::Upstage() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS) {}

void Upstage::step() {
  outputs[ENVELOPE_OUT].value = UNIPOLAR_VOLTAGE.scale(params[LEVEL_KNOB].value);
  lights[TRIG_LIGHT].value = params[TRIG_BUTTON].value;
  lights[WAIT_LIGHT].value = params[WAIT_BUTTON].value;

  if (inputs[WAIT_GATE_IN].value >= 1.0f || params[WAIT_BUTTON].value >= 1.0f)
    outputs[TRIG_OUT].value = 0.0f;
  else
    outputs[TRIG_OUT].value = std::max(
        UNIPOLAR_VOLTAGE.clamp(inputs[TRIG_IN].value),
        UNIPOLAR_VOLTAGE.scale(params[TRIG_BUTTON].value));
}
}