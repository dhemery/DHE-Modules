#pragma once
#include "components/trigger.h"
#include <functional>

namespace DHE {

/**
 * Calls on_rise() when the module's stage gate signal rises.
 */

template <typename M> class StageTrigger : public Trigger {
public:
  explicit StageTrigger(M *module, std::function<void()> on_rise)
      : Trigger{on_rise}, module{module} {}

protected:
  auto state_in() const -> bool override { return module->stage_gate_in(); }

private:
  M const *const module;
};
} // namespace DHE
