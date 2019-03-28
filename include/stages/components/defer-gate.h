
#pragma once
#include "components/gate.h"
#include <functional>

namespace DHE {

/**
 * Informs the state machine when the module's defer gate signal rises or falls.
 */
template <typename M> class DeferGate : public Gate {
public:
  explicit DeferGate(M *module, const std::function<void()> &on_rise,
                     const std::function<void()> &on_fall)
      : Gate{on_rise, on_fall}, module{module} {}

protected:
  auto state_in() const -> bool override { return module->defer_gate_in(); }

private:
  M const *const module;
};
} // namespace DHE
