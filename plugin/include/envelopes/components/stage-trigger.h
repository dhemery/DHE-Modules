#pragma once
#include "components/trigger.h"

namespace DHE {

/**
 * Informs the state machine when the module's stage gate signal rises.
 */
template<typename M, typename S>
class StageTrigger : public Trigger {
public:
  explicit StageTrigger(M *module, S *states)
      : module{module},
        states{states} {}

protected:
  auto state_in() const -> bool override { return module->stage_gate_in(); }

  void on_rise() override { states->on_stage_gate_rise(); }

private:
  M const *const module;
  S *const states;
};
}
