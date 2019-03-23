#pragma once
#include "components/gate.h"

namespace DHE {

/**
 * Informs the module when its defer gate signal rises or falls.
 */
template<typename M> class DeferGate : public Gate {
public:
  explicit DeferGate(M *module) : module{module} {}

protected:
  auto state_in() const -> bool override { return module->defer_gate_in(); }

  void on_rise() override { module->on_defer_gate_rise(); }

  void on_fall() override { module->on_defer_gate_fall(); }

private:
  M *module;
};
}
