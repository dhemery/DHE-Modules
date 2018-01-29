//
// Created by Dale Emery on 1/28/18.
//

#include <algorithm>
#include "util/range.h"
#include "upstage.h"

namespace DHE {

Upstage::Upstage() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS) {}

void Upstage::step() {
  if (inputs[LEVEL_CV].active) {
    outputs[OUT].value = inputs[LEVEL_CV].value * BIPOLAR_NORMAL.scale(params[LEVEL_KNOB].value);
  } else {
    outputs[OUT].value = UNIPOLAR_VOLTAGE.scale(params[LEVEL_KNOB].value);
  }

  if (inputs[WAIT_GATE_IN].value >= 1.0f || params[WAIT_BUTTON].value >= 1.0f)
    return;

  outputs[TRIG_OUT].value = std::max(inputs[TRIG_IN].value, UNIPOLAR_VOLTAGE.scale(params[TRIG_BUTTON].value));
}
}