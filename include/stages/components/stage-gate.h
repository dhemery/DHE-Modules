#pragma once
#include "components/gate.h"
#include <functional>

namespace DHE {

/**
 * Calls on_rise() when the module's stage gate signal rises, and on_fall() when
 * it falls.
 */
template <typename M> class StageGate : public Gate {
public:
  explicit StageGate(M *module, const std::function<void()>& on_rise,
                     const std::function<void()>& on_fall)
      : Gate{on_rise, on_fall}, module{module} {}

protected:
  auto state_in() const -> bool override { return module->stage_gate_in(); }

private:
  M const *const module;
};
} // namespace DHE
