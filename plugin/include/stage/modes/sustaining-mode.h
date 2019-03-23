#pragma once

#include "util/mode.h"
#include "components/gate.h"

namespace DHE {

/**
 * A stage module in sustaining mode emits its sustain voltage at its output
 * port. The sustain voltage is the voltage that the module saw at its input
 * port at the moment when it entered sustaining mode.
 */
template <typename M> class SustainingMode : public Mode {
public:
  explicit SustainingMode(M *module, Gate *sustain_gate)
      : module{module}, sustain_gate{sustain_gate} {}

  void enter() override {
    module->hold_input();
    module->set_active(true);
  }

  void step() override {
    module->send_held();
    sustain_gate->step();
  }

private:
  M *module;
  Gate *sustain_gate;
};
} // namespace DHE
