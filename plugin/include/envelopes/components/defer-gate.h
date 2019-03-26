
#pragma once
#include <functional>
#include "components/gate.h"

namespace DHE {

/**
 * Informs the state machine when the module's defer gate signal rises or falls.
 */
template <typename M>
class DeferGate : public Gate {
public:
  explicit DeferGate(M *module, std::function<void()> on_rise, std::function<void()> on_fall)
      : Gate{on_rise, on_fall},
      module{module} {}

protected:
  auto state_in() const -> bool override { return module->defer_gate_in(); }

private:
  M const *const module;
};
}
