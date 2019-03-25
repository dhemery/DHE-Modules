#pragma once
#include "components/gate.h"

namespace DHE {

/**
 * Informs the state machine when the module's defer gate signal rises or falls.
 */
template<typename M, typename S>
class DeferGate : public Gate {
public:
  explicit DeferGate(M *module, S *states)
      : module{module},
        states{states} {}

protected:
  auto state_in() const -> bool override { return module->defer_gate_in(); }

  void on_rise() override { states->on_defer_gate_rise(); }

  void on_fall() override { states->on_defer_gate_fall(); }

private:
  M const *const module;
  S *const states;
};
}
