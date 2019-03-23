#pragma once

#include "util/mode.h"
#include "components/phase-accumulator.h"
#include "components/trigger.h"

namespace DHE {

/**
 * A stage module in generating mode advances its phase, converts the phase into
 * a voltage, and emits the resulting voltage at its output port.
 */
template<typename M> class GeneratingMode : public Mode {
public:
  explicit GeneratingMode(M *module, PhaseAccumulator *stage_generator,
                          Trigger *stage_trigger)
      : module{module}, stage_trigger{stage_trigger}, stage_generator{
      stage_generator} {}

  void enter() override {
    module->hold_input();
    module->set_active(true);
    start();
  }

  void step() override {
    stage_generator->step();
    module->send_stage();
    stage_trigger->step();
  }

  void start() { stage_generator->start(); }

private:
  M *module;
  Trigger *stage_trigger;
  PhaseAccumulator *stage_generator;
};

} // namespace DHE
