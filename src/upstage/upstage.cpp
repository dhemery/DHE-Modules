//
// Created by Dale Emery on 1/28/18.
//

#include <algorithm>
#include "upstage.h"

namespace DHE {

Upstage::Upstage() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS) {}

void Upstage::step() {
  lights[TRIG_LIGHT].value = trigger_button_in();
  lights[WAIT_LIGHT].value = wait_button_in();

  outputs[TRIG_OUT].value = is_sending_triggers() ? trigger_port_out() : 0.0f;
  outputs[ENVELOPE_OUT].value = level_knob_voltage();
}
}