//
// Created by Dale Emery on 1/28/18.
//

#include <algorithm>
#include "util/range.h"
#include "upstage.h"

namespace DHE {

Upstage::Upstage() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS) {}

void Upstage::step() {
  outputs[OUT].value = UNIPOLAR_VOLTAGE.scale(params[LEVEL_KNOB].value);
  outputs[TRIG_OUT].value = std::max(inputs[TRIG_IN].value, UNIPOLAR_VOLTAGE.scale(params[TRIG_BUTTON].value));
}
}