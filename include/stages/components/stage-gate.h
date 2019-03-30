#pragma once
#include "components/gate.h"
#include <functional>

namespace DHE {

/**
 * Calls functions when the stage gate signal rises and falls.
 */
template <typename M> class StageGate : public Gate {
public:
  explicit StageGate(M *module, std::function<void()> on_rise,
                     std::function<void()> on_fall)
      : Gate{on_rise, on_fall}, module{module} {}

protected:
  auto state_in() const -> bool override { return module->stage_gate_in(); }

private:
  M const *const module;
};
} // namespace DHE
