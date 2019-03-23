#pragma once
#include "components/trigger.h"

namespace DHE {

/**
 * Informs the module when its stage trigger signal rises.
 */
template <typename M> class StageTrigger : public Trigger {
public:
  explicit StageTrigger(M *module) : module{module} {}

protected:
  auto state_in() const -> bool override { return module->stage_trigger_in(); }

  void on_rise() override { module->on_stage_trigger_rise(); }

private:
  M *module;
};
}
