#pragma once

#include "util/mode.h"
#include "components/trigger.h"

namespace DHE {

/**
 * A stage module in following mode emits its target voltage at its output port.
 */
template <typename M> class FollowingMode : public Mode {
public:
  explicit FollowingMode(M *module, Trigger *stage_trigger)
      : module{module}, stage_trigger{stage_trigger} {}

  void enter() override { module->set_active(false); }

  void step() override {
    module->send_level();
    stage_trigger->step();
  }

private:
  M *module;
  Trigger *stage_trigger;
};
} // namespace DHE
